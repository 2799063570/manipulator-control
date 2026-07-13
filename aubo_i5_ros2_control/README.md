# AUBO i5：仿真、MoveIt 与真机控制工作空间

这是 AUBO i5 的 ROS 2 Jazzy 工作空间。它以同一套机器人模型为基础，提供 Gazebo 仿真、MoveIt 规划，以及通过 `ros2_control` 接入 AUBO 控制柜的真机硬件插件。

## 目录与功能包

| 功能包 | 作用 | 主要内容/程序 |
| --- | --- | --- |
| `robot_description` | AUBO i5 的模型源 | URDF/Xacro、mesh、关节和夹爪定义；`dispaly_robot_xacro.launch.py` 用于 RViz 模型显示 |
| `aubo_i5_moveit_config` | MoveIt 配置 | SRDF、运动学、关节限位、控制器和 RViz 配置；`demo.launch.py` 启动 MoveIt 演示 |
| `aubo_i5_gazebo` | Gazebo 仿真 | 世界、模型生成、`gz_ros2_control`、时钟桥接；`demo_gazebo.launch.py` 是完整仿真入口 |
| `aubo_i5_hardware` | 真机 `ros2_control` 硬件插件 | `AuboI5System` 通过 AUBO RPC/RTDE 读取六轴状态、在安全门满足时写入 Servo 命令；`aubo_i5_real.launch.py` 是真机入口 |
| `aubo_msgs` | AUBO ROS 接口定义 | 与机器人控制相关的消息和服务类型 |
| `aubo_dashboard_msgs` | AUBO Dashboard 接口定义 | 控制柜模式、状态和 Dashboard 服务类型 |

## 构建

在 Ubuntu、ROS 2 Jazzy 环境中：

```bash
cd ~/cpp_practice/aubo_i5_ros2_control
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## 常用入口

### Gazebo + MoveIt 仿真

```bash
source install/setup.bash
ros2 launch aubo_i5_gazebo demo_gazebo.launch.py
```

启动前，`GZ_SIM_RESOURCE_PATH` 必须能找到 `robot_description` 的 `share` 目录；该环境变量已在当前 Gazebo launch 中设置。仿真时应只有一个 `/controller_manager`、一个 `joint_state_broadcaster` 和一份非空的 `/joint_states`。

### 仅启动 MoveIt

```bash
source install/setup.bash
ros2 launch aubo_i5_moveit_config demo.launch.py
```

### 真机：只读连接（默认、安全）

先确认 `aubo_i5_hardware/third_party/aubo_sdk/` 内含匹配架构的厂家 SDK，且已构建硬件包。随后：

```bash
source install/setup.bash
export AUBO_ROBOT_USERNAME='你的用户名'
export AUBO_ROBOT_PASSWORD='你的密码'
ros2 launch aubo_i5_hardware aubo_i5_real.launch.py \
  robot_ip:=控制柜IP enable_motion:=false
```

此模式只启动状态发布器，不会加载轨迹控制器，也不会发送 Servo 写入。先确认六轴 `/joint_states` 完整、单位和方向正确，再评审低速运动测试。

### 真机：允许运动

仅在急停、限位、现场安全措施和只读验收均通过后，才显式使用：

```bash
ros2 launch aubo_i5_hardware aubo_i5_real.launch.py \
  robot_ip:=控制柜IP enable_motion:=true
```

不要在启动多个旧 launch、多个控制器管理器或状态源不完整时执行此命令。

## 进一步文档

- `obsidian_机器人仿真与真机部署教程/30-AUBO参考驱动-仿真到真机实施手册.md`：硬件接口、SDK 迁移和真机验收步骤。
- `aubo_i5_hardware/third_party/README.md`：厂家 SDK 的目录约定、许可证与迁移说明。
