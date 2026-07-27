# `wheeltec_gazebo`

Gazebo Sim（ROS 2 Jazzy / Harmonic）中的 Wheeltec `mini_mec` 仿真包。它复用 `wheeltec_description` 的 URDF，不启动串口驱动，也不依赖 `ros2_control`；Gazebo Sim 内置的 `MecanumDrive` 系统负责四轮麦克纳姆运动学。

## 与真机保持一致的 ROS 接口

| 接口 | 真机 | Gazebo |
|---|---|---|
| 控制 | Nav2 → `/cmd_vel` → `wheeltec_base_driver` → 串口 | Nav2 → `/cmd_vel` → `ros_gz_bridge` → Gazebo `MecanumDrive` |
| 里程计 | `wheeltec_base_driver` 发布 `/odom` 与 `odom → base_footprint` | Gazebo 插件发布后由桥接转为相同接口 |
| 激光 | 实物雷达驱动发布 `/scan` | GPU lidar 发布后由桥接转为相同接口 |
| 车体模型 | `wheeltec_description` | 同一份 `wheeltec_description` |

`/cmd_vel`、`/odom`、`/scan`、`odom → base_footprint → laser` 的名字和语义一致，因此同一份 `wheeltec_navigation` 配置可用于建图、AMCL 与 Nav2。

## 构建与运行

安装依赖：

```bash
sudo apt install ros-jazzy-ros-gz ros-jazzy-ros-gz-sim
cd ~/robot_move_control
colcon build --packages-select wheeltec_description wheeltec_gazebo
source install/setup.bash
```

仅仿真：

```bash
ros2 launch wheeltec_gazebo simulation.launch.py
```

仿真建图：

```bash
ros2 launch wheeltec_gazebo mapping.launch.py
```

保存地图后仿真导航：

```bash
ros2 launch wheeltec_gazebo navigation.launch.py map_file:=$HOME/maps/wheeltec_lab.yaml
```

导航启动文件会先启动 Gazebo，默认等待 5 秒再启动 AMCL/Nav2，并自动使用保存地图中的 `(0, 0, 0)` 初始位姿。较慢的机器可以增加 `nav_start_delay`，例如 `nav_start_delay:=8.0`。

Nav2 导航节点使用带状态检查和重试的 lifecycle helper 启动；如果 `planner_server` 首次配置 global costmap 时响应慢半拍，启动流程会继续确认节点是否已经进入目标状态，而不是直接中止 bringup。

同一时间只运行一个 `wheeltec_world` 仿真实例。多个 Gazebo 实例会同时发布同名 `/world/wheeltec_world/clock`，造成 ROS 仿真时间倒跳、TF 被清空，并表现为 RViz 中定位跳变。启动文件使用独立的 Gazebo Transport 分区和 `/tmp/wheeltec_world.lock` 单实例锁；旧实例未退出时，新启动会直接提示 `Another wheeltec_world simulation is still running`。日志出现 `Moved backwards in time` 时，先关闭所有旧的 Gazebo/launch 终端，再重新启动一次。

## 验证顺序

```bash
ros2 topic echo /scan --once
ros2 topic echo /odom --once
ros2 run tf2_ros tf2_echo odom base_footprint
ros2 run tf2_ros tf2_echo base_footprint laser
```

然后低速发布 `linear.x`、`linear.y` 和 `angular.z`，确认仿真车能前后横移并原地转向。仿真通过不代表实车尺寸、轮子打滑和雷达外参已经准确，真实车仍需单独标定。
