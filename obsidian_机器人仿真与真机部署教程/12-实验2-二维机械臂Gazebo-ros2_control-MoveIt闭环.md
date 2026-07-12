---
title: 实验 2：二维机械臂 Gazebo、ros2_control、MoveIt 闭环
tags: [实验, Gazebo, MoveIt2, ros2_control]
---

# 实验 2：二维机械臂 Gazebo、ros2_control、MoveIt 闭环

## 实验目标

使现有 `simple_2dof_arm` 在 Gazebo Sim 中由 `JointTrajectoryController` 执行轨迹，再由 MoveIt 2 规划。严格按层排查：先 Gazebo/硬件接口，再 action，最后 MoveIt。

## 知识点：五层执行链

RViz/MoveIt 收到目标，`move_group` 完成 IK、碰撞检测、OMPL 路径规划与时间参数化；随后向 `JointTrajectoryController` 发送 `FollowJointTrajectory` action；控制器写 position command interface；`gz_ros2_control` 将命令交给 Gazebo，Gazebo 返回 position/velocity state。状态最终变成 `/joint_states` 和 TF。任何一层的关节名、模型或时间不一致都会断链。

## 先读当前文件

1. `simple_2dof_arm.ros2_control.xacro`：`hardware_type=gazebo` 时加载 `gz_ros2_control/GazeboSimSystem`。
2. `ros2_controllers.yaml`：100 Hz 的 manager、状态广播器与 `arm_controller`。
3. `simple_arm_gazebo/launch/gazebo.launch.py`：启动 GZ、桥接 `/clock`、spawn、5 秒后加载 controller。
4. `demo_gazebo.launch.py`：向 MoveIt 和 RViz 注入同一 robot description。

> [!note] Jazzy 适配检查
> 本仓库的 launch 使用 `ros_gz_sim`、`ros_gz_bridge` 和 `gz_ros2_control`，这与 Jazzy + Gazebo Harmonic 的方向一致。首次运行前不要猜 plugin 是否已加载；用 `ros2 pkg prefix gz_ros2_control` 确认包存在，并在 Gazebo 启动日志中确认 `gz_ros2_control::GazeboSimROS2ControlPlugin` 成功创建。若报 plugin filename 或 ABI 错误，先统一卸载/避免混用 Classic 与非 Harmonic 的 Gazebo 包，再检查本机 Jazzy 文档对应的插件名称。

## 步骤 1：只启动 Gazebo 与控制器

```bash
cd ~/manipulator-control/robot_control_ros2
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch simple_arm_gazebo gazebo.launch.py
```

另开终端等待 5 秒后：

```bash
source ~/manipulator-control/robot_control_ros2/install/setup.bash
ros2 control list_hardware_interfaces
ros2 control list_controllers
ros2 topic echo /clock --once
ros2 topic echo /joint_states --once
```

预期：`joint_state_broadcaster`、`arm_controller` 都是 active；位置 command interface 被 controller claimed；clock 和 joint_states 有数据。若失败，优先检查 Gazebo 插件安装、YAML 绝对/展开路径、URDF 关节名与 YAML 的 joint 名。

## 步骤 2：直接调用 action

该步骤隔离规划层。先查询 action 的真实名字：

```bash
ros2 action list -t
ros2 action info /arm_controller/follow_joint_trajectory
```

常见调用：

```bash
ros2 action send_goal --feedback /arm_controller/follow_joint_trajectory \
 control_msgs/action/FollowJointTrajectory \
 "{trajectory: {joint_names: [joint1, joint2], points: [
 {positions: [0.2, -0.2], velocities: [0.0, 0.0], time_from_start: {sec: 2}},
 {positions: [0.0, 0.0], velocities: [0.0, 0.0], time_from_start: {sec: 4}}
 ]}}"
```

记录 feedback 的 actual、desired、error。desired 变而 actual 不变，查控制器到 Gazebo；actual 变而 RViz 不变，查 joint_states/TF；action 被拒绝，查关节顺序、时间递增和 limit。

## 步骤 3：接入 MoveIt

```bash
ros2 launch simple_arm_gazebo demo_gazebo.launch.py
```

在 RViz MotionPlanning 中先选择 group `arm`，用 Joint 目标而非 Pose 目标开始，Plan 后 Execute。二维臂工作空间有限，Pose 无 IK 解属于正常结果。检查：

```bash
ros2 param get /move_group robot_description_kinematics
ros2 action list | grep arm_controller
ros2 topic echo /display_planned_path --once
```

## 步骤 4：限速与障碍物实验

将 `joint_limits.yaml` 的最大速度降至 20%，重新 build/source 后规划相同目标，记录轨迹总时长变长。说明 URDF limit、MoveIt limit、controller/驱动保护应层层收紧。

在 RViz Planning Scene 添加 box 并阻挡直线路径，比较加障碍物前后的轨迹。随后在 Gazebo world 添加同尺寸物体；两边都要有，否则会出现视觉上有桌子但规划器穿过的错误。

## 验收表

| 用例 | 预期 |
|---|---|
| 小幅 action | 2 秒内到位，无持续抖动 |
| 超限目标 | 拒绝或规划失败 |
| 取消 action | 停止/保持并可恢复 |
| 障碍物 | 避障或判定不可达 |
| 20 次重复 | controller 无崩溃且成功率有记录 |
