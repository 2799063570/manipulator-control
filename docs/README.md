# Project Documentation Index

这里按章节整理项目文档。根目录 `README.md` 负责快速理解和复现，`docs/` 负责阶段复盘、技术链路、简历和面试表达。

## 章节

1. [第 1 周：纯 C++ 控制基础库](week1_control_library.md)
2. [第 2 周：Eigen、雅可比求解与项目化](week2_jacobian_projectization.md)
3. [第 3 周：ROS2 控制节点与通信链路](week3_ros2_control_nodes.md)
4. [第 4 周：URDF、TF 与 RViz2 可视化闭环](week4_visualization_closure.md)
5. [简历描述与面试讲解稿](resume_and_interview.md)

## 当前项目主线

```text
纯 C++ 控制库
  -> ROS2 控制节点
  -> 关节命令 topic
  -> /joint_states
  -> robot_state_publisher
  -> TF / robot_description
  -> RViz2 可视化
```

当前阶段的价值不在于模型复杂度，而在于能讲清楚机器人软件系统中“命令、状态、模型、坐标变换、可视化”的闭环关系。
