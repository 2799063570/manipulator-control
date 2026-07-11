---
title: AUBO i5 仿真到真机部署
tags: [AUBO, 机械臂, 真机, 标定, 安全]
---

# AUBO i5 仿真到真机部署

> [!danger] 真机安全边界
> 本章的驱动接入、使能和运动命令必须在厂商手册允许范围内实施。真机首次测试应空载、低速、隔离区域、实体急停可达，且由熟悉控制柜操作的人员在场。不要从未经验证的规划结果直接执行。

## 1. 当前模型与缺口

`aubo_i5_ros2_control/src/robot_description` 已有 AUBO i5 六轴视觉网格、碰撞网格、夹爪、相机和桌子 Xacro。`aubo_i5.xacro` 通过 `world2base_link` 固定基座，`ee_link_joint` 连接腕部与夹爪。它还不是完整可控仿真包：需要补充/核对关节 limit、每节 `inertial`、传动/控制接口、Gazebo 插件、MoveIt SRDF/YAML 和启动文件。

## 2. 建议新增包结构

```text
src/
  aubo_i5_description/       # 仅 URDF、mesh、RViz
  aubo_i5_moveit_config/     # SRDF、kinematics、OMPL、controllers
  aubo_i5_gazebo/            # world、spawn、gz_ros2_control
  aubo_i5_hardware/          # 只封装真机通信的 SystemInterface
  aubo_i5_bringup/           # simulation/real 两套 launch
```

不要把 IP、密码、标定偏置写进 `description` 包。把设备参数放在未提交的 `config/robot.local.yaml`，并提供 `robot.example.yaml`。

## 3. 从模型到 Gazebo 的实现步骤

1. 用 `xacro` 展开并检查：

```bash
ros2 run xacro xacro urdf/aubo_i5.xacro > /tmp/aubo_i5.urdf
check_urdf /tmp/aubo_i5.urdf
```

2. 在每个运动关节写 `limit`（位置、速度、努力），单位分别为 rad、rad/s、N·m；数值必须来自厂商手册，绝不凭网格猜测。
3. 用 CAD 质量属性或厂家数据填写每个 link 的质心、质量、惯量。碰撞网格应明显简化，避免把高面数视觉 DAE/STL 当作碰撞体。
4. 仿照二维臂创建 `aubo_i5.ros2_control.xacro`，先用 `mock_components/GenericSystem` 启动 MoveIt，再切到 `gz_ros2_control/GazeboSimSystem`。
5. 创建 6 轴 `JointTrajectoryController`，关节名称与 URDF、SRDF、厂商驱动返回值严格一致。
6. MoveIt Setup Assistant 生成配置后逐项审查：planning group、base link、tip link、碰撞禁用对、joint limits、IK 插件。保存生成配置后再手动纳入版本控制。

## 4. 真机硬件接口的两种路线

### 路线 A：厂商官方 ROS 2 驱动（优先）

若 AUBO 提供与目标 ROS 发行版兼容的官方驱动，优先使用。核对其是否提供：状态反馈、`FollowJointTrajectory`、急停/保护停状态、IO、标定支持和文档化的 TCP/UDP 协议。你的上层 MoveIt 配置只需将 controller action 指向驱动暴露的 action。

### 路线 B：自写 `ros2_control` 硬件插件

仅在官方驱动缺失或需要特殊设备时做。实现 `hardware_interface::SystemInterface`：

```cpp
on_init(info)       // 读取 joint、IP、控制周期和标定参数
on_configure(...)   // 建立连接，读取一次状态
on_activate(...)    // 校验反馈、发送安全保持命令
read(time, period)  // 从控制柜读 q/dq/effort，写入 state interfaces
write(time, period) // 读 command interfaces，限幅后发给控制柜
on_deactivate(...)  // 停止轨迹，发送安全 hold，关闭连接
```

`read()` 与 `write()` 绝不能动态分配大量内存、阻塞等待或打印高频日志。通信协议线程与控制循环之间用预分配缓冲/无锁队列隔离；通信超时必须使控制器进入安全停止，而不是继续使用旧命令。

## 5. 真机上线流程

1. **网络**：PC 和控制柜使用固定 IP；`ping` 只是连通性，仍要验证协议端口和状态读取。配置时间同步（chrony/PTP，视厂商要求）。
2. **只读验证**：启动硬件驱动但不激活轨迹控制器，检查 6 轴 joint name、单位、零位、速度符号、状态频率。
3. **单轴低速**：在无负载下，每次仅移动一个关节，最大速度设为厂商额定值的 5%–10%。确认正方向和软限位。
4. **TCP/负载**：标定工具中心点（TCP）和末端负载质心；错误 TCP 会造成路径偏移，错误负载会影响动力学保护。
5. **坐标系**：确认 `world -> base_link -> ... -> tool0`，并通过不少于三点的实测对照验证。相机抓取还要完成手眼标定。
6. **轨迹**：先执行关节空间 home→home；再执行远离奇异位形的短笛卡尔轨迹；最后才启用 MoveIt 自动规划。

## 6. 必备安全状态机

```text
DISCONNECTED -> CONNECTED -> READY -> ENABLED -> RUNNING
RUNNING --(超限/通信超时/保护停/急停)--> FAULT -> SAFE_HOLD
SAFE_HOLD --(人工确认和复位)--> READY
```

上层规划节点不能越过状态机直接使能。所有状态变化记录时间、操作者、故障码和关节状态，便于事故追溯。

