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

## 验证顺序

```bash
ros2 topic echo /scan --once
ros2 topic echo /odom --once
ros2 run tf2_ros tf2_echo odom base_footprint
ros2 run tf2_ros tf2_echo base_footprint laser
```

然后低速发布 `linear.y`，确认仿真车能横移；这一步验证麦克纳姆轮的方向、roller 摩擦方向与 Gazebo 控制器配置。仿真通过不代表实车尺寸、轮子打滑和雷达外参已经准确，真实车仍需单独标定。
