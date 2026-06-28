# 简历描述与面试讲解稿

本文档用于把前 4 周项目成果整理成简历和面试表达。

## 简历描述

### 简洁版

```text
实现基于 ROS2 的机械臂控制与可视化 demo，包含纯 C++ 控制基础库、ROS2 控制节点、URDF 模型、JointState 状态发布、TF 坐标变换与 RViz2 可视化，完成从关节命令到机器人模型姿态显示的基础闭环。
```

### 工程版

```text
设计并实现 ROS2 机械臂控制基础项目：使用 C++ 封装 RobotState、轨迹插值、轨迹缓存、安全限幅和雅可比求解模块；基于 rclcpp 实现关节命令发布、位置命令到 JointState 的转换节点，并通过 URDF、robot_state_publisher、TF 和 RViz2 完成 2 自由度机械臂可视化闭环验证。
```

### 关键词版

```text
基于 ROS2 / C++ 实现机械臂控制与可视化闭环，完成 URDF / xacro 机器人建模、/joint_states 状态发布、robot_state_publisher TF 广播、RViz2 模型显示，以及控制节点与纯 C++ 轨迹插值、限幅、雅可比求解模块的集成。
```

## 1 分钟面试讲解稿

```text
这个项目我目前完成的是一个 ROS2 机械臂控制与可视化的基础闭环。

我先把核心控制逻辑从 ROS2 里拆出来，做成纯 C++ 控制库，包括 RobotState 状态管理、安全限幅、三次插值轨迹生成、轨迹缓存，以及基于 Eigen 的雅可比求解。这样控制算法可以单独测试，也方便后续复用。

然后我用 ROS2 C++ 节点把这些模块接入系统。比如 robot_position_cmd 节点发布关节位置命令，joint_position_cmd_to_states 节点订阅位置命令，内部用状态、速度限制和插值生成平滑的关节状态，并发布到标准的 /joint_states。

最后我用 xacro 写了一个 2 自由度机械臂 URDF，通过 robot_state_publisher 读取 robot_description 和 /joint_states，发布 base_link、link1、link2 之间的 TF，RViz2 再根据 TF 和模型描述显示机械臂姿态。

所以这个阶段的重点不是单纯打开 RViz2，而是跑通了“控制命令 -> 关节状态 -> TF -> RViz2 可视化”的闭环。当前限制是模型还比较简单，状态来自模拟节点，还没有真实动力学、Gazebo、MoveIt2 和 ros2_control，后续可以继续往标准机械臂控制栈扩展。
```

## 常见面试问题

### ROS2 节点和 C++ 控制库怎么分工？

```text
纯 C++ 控制库负责算法和数据结构，比如状态表示、插值、限幅、轨迹缓存和雅可比求解。ROS2 节点负责通信和系统集成，比如订阅命令、发布状态、读取参数、启动 timer 控制循环。这样做可以降低控制算法和 ROS2 API 的耦合，后续更容易测试和复用。
```

### `/joint_states` 有什么作用？

```text
/joint_states 是机器人关节状态的标准话题，里面包含关节名称、位置、速度等信息。robot_state_publisher 会结合 URDF 和 /joint_states 计算各个 link 的 TF，RViz2 再根据 TF 显示机器人当前姿态。
```

### robot_state_publisher 做了什么？

```text
robot_state_publisher 不是控制器，它主要负责根据 robot_description 中的 URDF 结构，以及 /joint_states 中的关节值，计算并发布机器人各个 link 之间的 TF 坐标变换。
```

### 为什么说 RViz2 只是可视化终点？

```text
因为 RViz2 本身不负责生成控制命令，也不负责计算机器人状态。它只是订阅 robot_description、TF 等信息并显示出来。真正的链路是控制节点产生命令，状态节点发布 /joint_states，robot_state_publisher 发布 TF，最后 RViz2 显示结果。
```

### 当前项目和真实机械臂控制还差什么？

```text
当前项目还没有真实硬件反馈、动力学模型、轨迹控制器和标准 ros2_control 接口。现在主要验证的是 ROS2 控制节点、关节状态、URDF、TF 和 RViz2 的基础闭环。后续可以接 MoveIt2 做规划，再接 ros2_control 和 JointTrajectoryController，让规划轨迹进入标准控制器链路。
```
