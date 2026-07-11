---
title: ROS 2 机器人仿真与真机部署教程
tags: [ROS2, Gazebo, MoveIt2, ros2_control, 机械臂, 移动机器人]
---

# ROS 2 机器人仿真与真机部署教程

> [!abstract] 适用范围
> 本笔记基于当前仓库的真实代码编写，目标是将已有的控制算法、二维机械臂、AUBO i5 描述和小车描述，逐步发展为可验证的 Gazebo 仿真系统，并最终安全地接入真机。默认平台为 Ubuntu 22.04 + ROS 2 Humble；若使用 Jazzy，请以对应发行版的软件包名为准。

## 从当前仓库出发

| 现有资产 | 位置 | 当前价值 | 下一步 |
|---|---|---|---|
| 纯 C++ 控制库 | `robot_control_ros2/src/robot_control` | 状态、限幅、三/五次插值、轨迹缓冲、雅可比伪逆/DLS | 接入标准控制器或硬件接口 |
| ROS 2 命令节点 | `robot_control_ros2/src/robot_control_ros2` | 速度命令转位置轨迹、`/joint_states` 可视化链路 | 修正接口并作为上层速度控制原型 |
| 二维机械臂 | `simple_arm_description`、`simple_arm_gazebo`、`simple_arm_moveit_config` | 已有 `gz_ros2_control`、MoveIt 2、OMPL、轨迹控制器 | 作为第一套端到端仿真样机 |
| AUBO i5 模型 | `aubo_i5_ros2_control/src/robot_description` | 六轴本体、夹爪、相机、桌子模型 | 补全惯量、碰撞、ros2_control 与厂商驱动 |
| 移动底盘模型 | `robot_move_control/src/fishbot_description` | 底盘、相机、雷达，雷达已有 Gazebo Classic 插件 | 迁移至 Gazebo Sim，接入 diff drive、Nav2、SLAM |

## 笔记导航

1. [[01-项目盘点与目标架构]]：理解代码、确定分层和里程碑。
2. [[02-环境与工作区复现]]：安装、编译、启动、验收命令。
3. [[03-机械臂Gazebo与MoveIt2仿真]]：把二维臂扩成可控、可规划、可抓取的仿真系统。
4. [[04-AUBO-i5仿真到真机部署]]：六轴模型、厂商驱动、校准与安全上线。
5. [[05-移动机器人仿真与Nav2]]：差速底盘、传感器、SLAM、导航闭环。
6. [[06-移动机器人真机部署]]：电机、MCU、里程计、雷达和室内导航上线。
7. [[07-工程化-测试-排障与安全]]：仿真到真机的质量门禁与常见错误。
8. [[08-核心知识点与面试八股]]：可背诵回答、追问与项目表达。
9. [[09-项目实施清单]]：按周推进的任务、验收标准和交付物。

## 实验手册（按此顺序动手）

10. [[10-实验0-ROS2基础与现有控制代码复现]]：从自定义消息、节点到 RViz 的已有码实验。
11. [[11-实验1-轨迹规划-限幅与雅可比控制]]：逐个验证 C++ 控制库的数学含义和测试方法。
12. [[12-实验2-二维机械臂Gazebo-ros2_control-MoveIt闭环]]：可执行的仿真、控制器与规划实验。
13. [[13-实验3-AUBO-i5建模-Gazebo与MoveIt配置]]：把现有 AUBO 模型补全为可仿真的六轴系统。
14. [[14-实验4-机械臂真机接口-标定与安全验证]]：真机接入前后的逐级操作规程。
15. [[15-实验5-差速小车-Gazebo-里程计-SLAM-Nav2]]：从轮子到自主导航的完整实验。
16. [[16-实验6-移动机器人真机-MCU与传感器融合]]：控制板、编码器、EKF、地图与导航验收。

## 参考手册（实验时随时查）

17. [[17-功能包与工具链详解]]：当前工程与后续部署所需 ROS 2 功能包、职责、接口和使用顺序。
18. [[18-当前代码类与ROS2接口逐项解析]]：控制库类、自定义消息、节点和 launch 的逐项说明。
19. [[19-高频易错点与系统排障字典]]：URDF、Gazebo、MoveIt、TF、Nav2、真机接口的错误模式。
20. [[20-面试八股题库-背诵版]]：按主题编排的标准回答、追问和项目化表达。
21. [[21-CMakeLists与package.xml从入门到当前工程]]：ROS 2 包的构建、依赖、安装与常见错误。

## 总体数据流

```mermaid
flowchart LR
  A[上层任务/遥操作/MoveIt2/Nav2] --> B[规划或控制节点]
  B --> C[ros2_control 控制器]
  C --> D{硬件接口}
  D -->|仿真| E[Gazebo Sim]
  D -->|真机| F[驱动器/MCU/机器人控制柜]
  E --> G[/joint_states、传感器]
  F --> G
  G --> H[TF / robot_state_publisher / 状态估计]
  H --> A
```

> [!important] 先后顺序
> **先让状态可信，再让命令可控，最后接入规划。** 不要把 MoveIt、导航和真机驱动同时调试。每一步都先使用 `ros2 topic echo`、RViz 和小幅度、低速度命令验证。

## 使用方式

将整个 `obsidian_机器人仿真与真机部署教程` 文件夹作为 Obsidian Vault 打开。笔记中的相对路径均相对于仓库根目录；命令中的 `~/manipulator-control` 替换为你的实际 Linux 路径。
