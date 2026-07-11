---
title: 机械臂 Gazebo 与 MoveIt 2 仿真
tags: [机械臂, Gazebo, MoveIt2, ros2_control]
---

# 机械臂 Gazebo 与 MoveIt 2 仿真

## 1. 当前二维臂为何能仿真

`simple_2dof_arm.ros2_control.xacro` 根据 `hardware_type` 选择硬件插件：`fake` 使用 `mock_components/GenericSystem`，`gazebo` 使用 `gz_ros2_control/GazeboSimSystem`。当 `hardware_type:=gazebo` 时，同一 Xacro 展开 `<gazebo>` 插件并加载 `ros2_controllers.yaml`。这正是“模型不变、硬件后端可切换”的关键。

`simple_arm_gazebo/launch/gazebo.launch.py` 完成了：启动 `gz sim`、桥接 `/clock`、发布 robot description、生成模型、延迟加载 `joint_state_broadcaster` 和 `arm_controller`。

## 2. `ros2_control` 的配置原则

控制器 YAML 中关节名必须与 URDF 的可动关节一致：

```yaml
controller_manager:
  ros__parameters:
    update_rate: 100
    joint_state_broadcaster:
      type: joint_state_broadcaster/JointStateBroadcaster
    arm_controller:
      type: joint_trajectory_controller/JointTrajectoryController

arm_controller:
  ros__parameters:
    joints: [joint1, joint2]
    command_interfaces: [position]
    state_interfaces: [position, velocity]
```

控制频率、物理步长和轨迹点时间必须协调：控制器 100 Hz 不意味着 Gazebo 一定 100 Hz；以实际 `/clock` 和控制器日志为准。低频或抖动时，先减小仿真负担，不能直接提高 PID。

## 3. 用 MoveIt 2 执行

当前 `demo_gazebo.launch.py` 同时启动 Gazebo、`move_group` 与 RViz，并加载 OMPL。RViz 中选择规划组 `arm`，拖动交互标记，点击 Plan 后再 Execute。接口校验：

```bash
ros2 param get /move_group robot_description_semantic
ros2 action list | grep follow_joint_trajectory
ros2 topic echo /display_planned_path
```

> [!note] MoveIt 和控制器的职责
> MoveIt 负责碰撞检测、IK、路径规划和时间参数化；`joint_trajectory_controller` 负责按时间执行关节轨迹；Gazebo/真机硬件负责状态反馈。不要让 MoveIt 直接发布自定义位置 topic 来代替控制器。

## 4. 增加夹爪与抓取物体

实施顺序：

1. 在 URDF 添加 `tool0 -> gripper_base` 固定关节和两个夹爪关节；给每个 link 写 `visual`、简化后的 `collision`、`inertial`。
2. 在 SRDF 增加 `gripper` group、预定义 `open/close` 状态，并把末端 link 设为 end effector。
3. 在 ros2_control YAML 添加 `gripper_controller`。平行夹爪优先使用 `parallel_gripper_action_controller`；初学也可用一个 `JointTrajectoryController`。
4. 在 Gazebo world 放置带碰撞的方块；在 MoveIt PlanningScene 添加同名 collision object。
5. 先只验证开合，再验证接近、附着（attach）、抬升、释放。物理抓取与 MoveIt 的 attach 是两件事：前者决定接触，后者决定规划碰撞模型。

最小 `inertial` 模板（单位 kg、m、kg·m²）：

```xml
<inertial>
  <origin xyz="0 0 0" rpy="0 0 0"/>
  <mass value="0.12"/>
  <inertia ixx="0.0001" ixy="0" ixz="0" iyy="0.0001" iyz="0" izz="0.0001"/>
</inertial>
```

## 5. 用自有算法替换上层速度控制

你的 `JacobianSolver::solveDampedLeastSquares` 可作为笛卡尔速度控制的数值核心：

\[
\dot q=J^T(JJ^T+\lambda^2I)^{-1}\dot x
\]

实现节点时订阅末端速度 `TwistStamped`，读取真实 `/joint_states`，由 FK/Jacobian 计算 `qdot`，经过 `SafetyLimiter` 后发布给 `joint_trajectory_controller` 的速度接口或 MoveIt Servo。优先复用 **MoveIt Servo**：它已处理奇异性、碰撞缩放、时间戳与停止。自写版本只用于理解或非安全关键实验。

## 6. 仿真验收

- 从任意合法初始姿态，在 30 次随机目标中规划成功率 ≥ 95%。
- 计划轨迹不与桌子、自碰撞，执行后末端误差记录在可接受范围。
- 把控制速度缩至额定的 10%，验证软限位与控制器取消动作。
- 对模型质量、负载、摩擦、延迟扰动进行测试，不能只在理想参数下演示。

