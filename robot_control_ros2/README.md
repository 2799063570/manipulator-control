# ROS 2 控制与二维机械臂学习工作空间

这是用于学习 ROS 2 通信、TF、运动学、轨迹插补、控制循环，以及二维机械臂 Gazebo/MoveIt 流程的工作空间。它是 AUBO i5 项目的前置实验环境，不包含真机驱动。

## 功能包

| 功能包 | 作用 | 主要程序或 launch |
| --- | --- | --- |
| `simple_arm_description` | 二自由度机械臂模型 | URDF/Xacro、mesh；`simple_2dof_arm_display.launch.py` 在 RViz 显示模型 |
| `simple_arm_gazebo` | 二维臂 Gazebo 仿真 | `gazebo.launch.py`、`demo_gazebo.launch.py`；用于控制器和关节状态学习 |
| `simple_arm_moveit_config` | 二维臂 MoveIt 配置 | `demo.launch.py`、`planning_execution.launch.py`、`moveit_cpp_demo`、`planner_comparison` |
| `robot_control` | 与 ROS 无关的控制算法库 | 机器人状态、三次插补、Jacobian 求解、安全限幅、轨迹缓冲；带对应 `*_test` 和 `control_loop_demo` |
| `robot_control_ros2` | ROS 2 基础节点示例 | topic、service、parameter、timer、TF、速度转位置等节点；含 `vel_to_pos_node.launch.py`、`tf_broadcaster.launch.py` |
| `other_package` | 自定义消息与库测试 | `self_msg_test_publisher`、`self_msg_test_subscriber`、`lib_test`、`jacobian_lib_test` |
| `turtlesim_exercise` | turtlesim 与 TF 练习 | `turtlesim_follow.launch.py`、`turtle_escort.launch.py`；含海龟生成、TF 广播与跟随控制节点 |

## 构建

```bash
cd ~/cpp_practice/robot_control_ros2
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## 推荐学习顺序

1. `robot_control_ros2`：先运行 topic、service、parameter、timer 和 TF 节点，理解 ROS 2 图通信。
2. `turtlesim_exercise`：把 TF 从静态概念变为可观察的坐标变换。
3. `robot_control`：独立运行算法测试，理解轨迹、Jacobian 和速度/位置限制。
4. `simple_arm_description`：检查模型、link、joint 与坐标系。
5. `simple_arm_gazebo`：观察 `ros2_control`、控制器和 `/joint_states`。
6. `simple_arm_moveit_config`：学习从规划到轨迹执行的完整链路。

## 常用命令

```bash
# 二维臂仿真与 MoveIt
ros2 launch simple_arm_gazebo demo_gazebo.launch.py

# 仅显示机器人模型
ros2 launch simple_arm_description simple_2dof_arm_display.launch.py

# turtlesim 跟随练习
ros2 launch turtlesim_exercise turtlesim_follow.launch.py

# 速度命令积分为位置命令
ros2 launch robot_control_ros2 vel_to_pos_node.launch.py
```

## pluginlib 动态插件入门示例

`robot_control_ros2` 中包含一个与 AUBO 硬件接口加载方式相似的最小插件示例：

- `PrinterBase` 是所有插件共同继承的抽象基类；
- `ChinesePrinter` 和 `EnglishPrinter` 是两个派生类；
- 两个派生类编译到 `libprinter_plugins.so`，并通过导出宏注册；
- `printer_plugins.xml` 建立插件名称、C++ 类型和动态库之间的映射；
- `pluginlib_demo` 只依赖基类，运行时才按字符串名称加载具体插件。

构建并运行：

```bash
cd ~/cpp_practice/robot_control_ros2
source /opt/ros/jazzy/setup.bash
colcon build --packages-up-to robot_control_ros2
source install/setup.bash
ros2 run robot_control_ros2 pluginlib_demo
```

预期输出：

```text
准备加载插件：robot_control_ros2/ChinesePrinter
你好，我是运行时加载的中文打印插件！
插件调用完成。
```

不重新编译主程序，直接改用英文插件：

```bash
ros2 run robot_control_ros2 pluginlib_demo robot_control_ros2/EnglishPrinter
```

还可以检查主程序没有直接链接插件库：

```bash
ldd install/robot_control_ros2/lib/robot_control_ros2/pluginlib_demo | grep printer_plugins
```

正常情况下该命令没有输出。插件库不是主程序的固定链接依赖，而是 `pluginlib`
读取 XML 后通过 `dlopen()` 按需加载。这正是它和普通 `target_link_libraries()` 动态链接的区别。

每次只保留一套仿真/控制 launch 在运行，避免遗留节点同时发布 `/joint_states` 或占用同名 `/controller_manager`。
