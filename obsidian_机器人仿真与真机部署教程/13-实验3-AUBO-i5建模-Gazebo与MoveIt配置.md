---
title: 实验 3：AUBO i5 建模、Gazebo 与 MoveIt 配置
tags: [实验, AUBO, URDF, MoveIt2, Gazebo]
---

# 实验 3：AUBO i5 建模、Gazebo 与 MoveIt 配置

## 实验目标

将现有 `robot_description` 从视觉网格模型完善为运动学、碰撞、控制、规划一致的六轴系统。关节限制、质量、负载等数值必须以官方手册/CAD 为来源，未知参数只可标为 TBD，不能带入真机。

## 步骤 1：模型真值表与 URDF 检查

建立参数表：关节名、父/子 link、轴方向、零位、位置/速度限制、质量、质心、惯量、来源页码。现有本体在 `urdf/arm.xacro`，总装配在 `aubo_i5.xacro`，其中已把 `world` 固定到 `base_link` 并连接夹爪。

```bash
cd ~/manipulator-control/aubo_i5_ros2_control
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 run xacro xacro src/robot_description/urdf/aubo_i5.xacro > /tmp/aubo_i5.urdf
check_urdf /tmp/aubo_i5.urdf
```

每个 link 应分别有 visual、简化 collision、inertial。惯量矩阵必须对称正定，单位是 kg*m^2；用高面数视觉网格做 collision 会显著拖慢 MoveIt/Gazebo，错误质量/惯量会导致仿真抖动或飞走。

## 步骤 2：先 fake hardware 再 Gazebo

用 MoveIt Setup Assistant 导入 URDF：创建固定 virtual joint（world 到 base）、六轴 planning group、夹爪 end effector、home/ready pose、自碰撞矩阵和 FollowJointTrajectory controller。

```bash
ros2 launch moveit_setup_assistant setup_assistant.launch.py
```

先在 `mock_components/GenericSystem` 下运行 RViz/MoveIt，反复执行 home 和 ready，核查关节名、零位、运动方向。之后仿照二维臂添加 `aubo_i5.ros2_control.xacro`，Gazebo 使用 `gz_ros2_control/GazeboSimSystem`，为 6 轴配置 position command、position/velocity state 和 trajectory controller。

## 步骤 3：渐进式验证

1. `check_urdf` 和 RViz：无 mesh 路径错误。
2. fake hardware：每轴正负 0.1 rad，确认方向。
3. Gazebo：controller active，单轴低幅 action。
4. 6 轴 home 往返 20 次，记录控制器 error。
5. 将 `table.xacro` 同时加入 Gazebo 和 MoveIt planning scene，确认不穿桌。
6. 最后接夹爪、相机；夹爪 TCP 与法兰关系必须先实测验证。

## 关键知识点

SRDF 不是 URDF 的替代品：URDF 描述物理树和关节，SRDF 描述规划组、末端、默认姿态、自碰撞禁用。MoveIt 的碰撞模型、Gazebo 的物理模型、真机负载模型都是不同层，需保持几何与坐标一致。

## 验收

- [ ] 6 轴关节名、方向、limit 均有来源。
- [ ] 每轴小幅运动正确，home/ready 20 次无报警。
- [ ] 加桌子后规划不会穿过桌面。
- [ ] 保存 10 个 TCP FK 点，为真机对照建立基线。

