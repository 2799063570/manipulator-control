# Manipulator Control：机器人仿真、控制与真机部署

这是一个 ROS 2 Jazzy 学习与工程实践仓库。内容覆盖从 C++ 控制算法、ROS 2 通信与 TF、URDF/Xacro 建模，到 Gazebo/MoveIt 仿真，以及 AUBO i5 的 `ros2_control` 真机接口。

当前工程按“独立 ROS 2 工作空间”组织。请在**目标工作空间根目录**构建和 `source install/setup.bash`，不要在仓库根目录直接执行 `colcon build`。

## 工作空间总览

| 工作空间 | 定位 | 适合做什么 |
| --- | --- | --- |
| [`aubo_i5_ros2_control`](aubo_i5_ros2_control/README.md) | AUBO i5 主工程 | Gazebo + MoveIt 仿真、模型维护、接入 AUBO 真机 |
| [`robot_control_ros2`](robot_control_ros2/README.md) | ROS 2 与二维臂学习工程 | 控制算法、topic/service/TF、二维臂仿真和 MoveIt |
| [`robot_move_control`](robot_move_control/README.md) | Fishbot 建模练习 | URDF/Xacro 展开与 RViz 模型显示 |

## 推荐学习与使用路径

```text
robot_move_control
  └─ URDF/Xacro、link、joint、RViz

robot_control_ros2
  ├─ ROS 2 topic / service / parameter / timer / TF
  ├─ 插补、Jacobian、轨迹缓冲、安全限幅
  └─ 二自由度机械臂 Gazebo + MoveIt

aubo_i5_ros2_control
  ├─ AUBO i5 机器人描述
  ├─ Gazebo + ros2_control + MoveIt
  └─ RPC/RTDE 真机硬件接口（先只读、后低速运动）
```

## 快速开始

所有命令默认在 Ubuntu 24.04 + ROS 2 Jazzy 中执行。

### AUBO i5 仿真

```bash
cd ~/cpp_practice/aubo_i5_ros2_control
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
ros2 launch aubo_i5_gazebo demo_gazebo.launch.py
```

### 二自由度机械臂仿真

```bash
cd ~/cpp_practice/robot_control_ros2
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
ros2 launch simple_arm_gazebo demo_gazebo.launch.py
```

### Fishbot 模型显示

```bash
cd ~/cpp_practice/robot_move_control
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
ros2 launch fishbot_description dispaly_robot_xacro.launch.py
```

## AUBO i5 真机安全约定

`aubo_i5_ros2_control` 中的真机接口使用厂家 AUBO SDK，经 RPC/RTDE 连接控制柜。SDK 放在：

```text
aubo_i5_ros2_control/src/aubo_i5_hardware/third_party/aubo_sdk/
```

真机启动必须先使用只读模式：

```bash
ros2 launch aubo_i5_hardware aubo_i5_real.launch.py \
  robot_ip:=控制柜IP enable_motion:=false
```

确认完整六轴 `/joint_states`、关节方向、单位、零位和安全状态均正确后，才能在完成现场安全检查的前提下显式设置 `enable_motion:=true`。不要同时运行多个 Gazebo/MoveIt/真机 launch，避免多个状态发布者或 `/controller_manager` 相互干扰。

## Obsidian 教程（推荐作为主学习手册）

[`docs/`](docs/README.md) 是本仓库配套的中文 Obsidian Vault。它不是单篇说明，而是一套从代码盘点到真机验收的教程；用 Obsidian 直接打开该目录可保留双向链接、提示块和流程图。

推荐按以下顺序阅读：

1. [01-项目盘点与目标架构](docs/01-项目盘点与目标架构.md) 与 [02-环境与工作区复现](docs/02-环境与工作区复现.md)：了解仓库和复现环境。
2. [10-实验0：ROS2基础与现有控制代码复现](docs/10-实验0-ROS2基础与现有控制代码复现.md) → [12-实验2：二维机械臂 Gazebo、ros2_control、MoveIt 闭环](docs/12-实验2-二维机械臂Gazebo-ros2_control-MoveIt闭环.md)：完成 ROS 2 与仿真基础。
3. [13-实验3：AUBO i5 建模、Gazebo 与 MoveIt 配置](docs/13-实验3-AUBO-i5建模-Gazebo与MoveIt配置.md)：进入 i5 仿真。
4. [30-AUBO参考驱动：仿真到真机实施手册](docs/30-AUBO参考驱动-仿真到真机实施手册.md)：SDK、硬件接口、真机只读验证和安全验收。

教程还包含功能包/工具链说明、CMake 与 `package.xml`、MoveIt 与 `ros2_control` 执行链路、QoS、排障字典、ROS1→Jazzy 迁移及移动机器人/Nav2 专题。真机操作以教程中的安全门和逐级验收要求为准。

## 其他文档

- [AUBO i5 工作空间说明](aubo_i5_ros2_control/README.md)
- [ROS 2 控制学习工作空间说明](robot_control_ros2/README.md)
- [Fishbot 工作空间说明](robot_move_control/README.md)
- [教程首页](docs/README.md)：Obsidian 教程导航、实验手册与参考资料。

## 仓库结构

```text
manipulator-control/
├── aubo_i5_ros2_control/                 # AUBO i5 主工作空间
├── robot_control_ros2/                   # 控制算法与二维臂学习工作空间
├── robot_move_control/                   # Fishbot 建模工作空间
├── docs/                                  # 中文 Obsidian 教程、部署手册与学习笔记
└── cpp_test/                              # 独立 C++ 练习代码
```
