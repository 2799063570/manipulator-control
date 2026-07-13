# Fishbot 模型显示工作空间

这是一个轻量的 ROS 2 机器人建模练习工作空间，当前只有 `fishbot_description` 功能包，用于学习 URDF/Xacro 模型展开和 RViz 可视化。

## 功能包

| 功能包 | 作用 | 主要 launch |
| --- | --- | --- |
| `fishbot_description` | Fishbot 的 URDF/Xacro、mesh 和 RViz 配置 | `dispaly_robot.launch.py` 用 URDF 显示；`dispaly_robot_xacro.launch.py` 先展开 Xacro 再显示 |

> 文件名中的 `dispaly` 是现有包内的拼写，启动时必须按该实际名称输入。

## 构建与运行

```bash
cd ~/cpp_practice/robot_move_control
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash

# 推荐：从 Xacro 展开后在 RViz 显示
ros2 launch fishbot_description dispaly_robot_xacro.launch.py
```

这个工作空间目前没有 Gazebo、MoveIt 或真机控制器；它的定位是验证模型层。模型显示正常后，再把已验证的 link、joint、坐标系和 mesh 经验迁移到更完整的机械臂工程。
