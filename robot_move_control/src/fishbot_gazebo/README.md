# Fishbot：Gazebo Sim、SLAM 与 Nav2

`fishbot_gazebo` 是 Fishbot 差速底盘的 ROS 2 Jazzy 仿真功能包。它复用
`fishbot_description` 中已经验证过的底盘、车轮、相机和雷达 link/joint，新增
Gazebo Sim 控制、二维激光桥接、SLAM Toolbox 建图和 Nav2 定位导航的启动入口。

> 原 `assemble.xacro` 中的 `libgazebo_ros_diff_drive.so` 和
> `libgazebo_ros_laser.so` 是 Gazebo Classic 插件。本包不会加载它们，而是使用
> `gz_ros2_control`、Gazebo Sim 的传感器系统以及 `ros_gz_bridge`，因此两套插件
> 不会在同一次仿真中冲突。

## 功能与文件

| 位置 | 用途 |
| --- | --- |
| `urdf/fishbot.urdf.xacro` | 复用 Fishbot 模型，声明左右轮速度接口、`gz_ros2_control` 和 Gazebo Sim 激光雷达。 |
| `config/controllers.yaml` | `diff_drive_controller` 的轮半径、轮距、速度限制和 `/odom` 发布设置。 |
| `config/twist_mux.yaml` | 遥控与导航速度源的优先级配置。 |
| `worlds/nav2_world.sdf` | 带边界墙和静态障碍物的建图/导航练习场景。 |
| `launch/simulation.launch.py` | 仅启动 Gazebo、机器人、控制器、时钟/激光桥接和速度仲裁。 |
| `launch/slam.launch.py` | 在仿真基础上启动 SLAM Toolbox 在线建图。 |
| `launch/navigation.launch.py` | 在仿真基础上加载已有地图并启动 AMCL + Nav2。 |

## 数据链路

```text
cmd_vel_teleop ─┐
                ├─> twist_mux ─> /cmd_vel ─> base_controller ─> Gazebo Sim
cmd_vel_nav ────┘                                      │
                                                       ├─> /odom
Gazebo lidar ─> /scan (gz) ─> ros_gz_bridge ─> /scan (ROS)
robot_state_publisher ─> base_footprint -> base_link -> laser

SLAM Toolbox / AMCL ─> map -> odom
base_controller ──────> odom -> base_footprint
```

`map -> odom` 只能由 SLAM Toolbox 或 AMCL 二选一发布；`odom ->
base_footprint` 则由本包的 `diff_drive_controller` 发布。启动了导航时不要同时启动
`slam.launch.py`，否则会有两个节点竞争 `map -> odom`。

## 前置条件

在 Ubuntu 24.04 + ROS 2 Jazzy 环境中安装以下组件：

```bash
sudo apt install \
  ros-jazzy-ros-gz-sim ros-jazzy-ros-gz-bridge ros-jazzy-gz-ros2-control \
  ros-jazzy-ros2-controllers ros-jazzy-slam-toolbox ros-jazzy-nav2-bringup \
  ros-jazzy-twist-mux ros-jazzy-teleop-twist-keyboard
```

构建 Fishbot 工作空间：

```bash
cd ~/cpp_practice/robot_move_control
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## 1. 先验证底盘仿真

启动仿真：

```bash
ros2 launch fishbot_gazebo simulation.launch.py
```

另开一个已 `source install/setup.bash` 的终端，启动键盘遥控。速度会通过
`cmd_vel_teleop` 进入 `twist_mux`，再由 `/cmd_vel` 控制底盘：

```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args \
  -r cmd_vel:=cmd_vel_teleop
```

先低速验证前进、后退和原地旋转；若方向反了，先检查左右轮 joint 名称与 joint
轴方向，不要通过 Nav2 参数“补偿”。完成后检查：

```bash
ros2 topic echo /scan --once
ros2 topic echo /odom --once
ros2 run tf2_ros tf2_echo odom base_footprint
ros2 run tf2_ros tf2_echo base_footprint laser
```

应能看到 `/scan` 的 `frame_id` 为 `laser`，且 TF 链路完整。

## 2. 在线建图

启动 SLAM：

```bash
ros2 launch fishbot_gazebo slam.launch.py
```

在 RViz 中添加 `Map`、`LaserScan` 和 `TF` 显示，固定坐标系设为 `map`。再运行上节的
键盘遥控，沿墙壁和障碍物慢速完整行驶一圈。建图结束后保存地图：

```bash
ros2 run nav2_map_server map_saver_cli -f ~/fishbot_maps/nav2_world
```

这会创建 `~/fishbot_maps/nav2_world.yaml` 和同名图像文件。保存前请确认目录已经存在。

## 3. 使用已有地图导航

停止 SLAM 后，带保存的 YAML 启动导航：

```bash
ros2 launch fishbot_gazebo navigation.launch.py \
  map:=$HOME/fishbot_maps/nav2_world.yaml
```

在 RViz 中将固定坐标系设为 `map`，依次执行：

1. 使用 **2D Pose Estimate** 设置机器人初始位姿；
2. 等激光点云与地图重合；
3. 使用 **Nav2 Goal** 发送目标点。

导航期间，Nav2 的速度应使用 `cmd_vel_nav`，而遥控使用 `cmd_vel_teleop`。遥控优先级
更高，适合人工接管或紧急停止；所有速度命令超时后会自动归零。

## 标定值与调参顺序

当前仿真模型的几何值来自 `fishbot_description/urdf/mycar/base_link.xacro`：

- `wheel_radius: 0.0325` m
- `wheel_separation: 0.20` m（轮中心位于 `y = ±0.10` m）

若模型或真机尺寸改变，应先同步修改 `config/controllers.yaml`。直线距离不准时校正轮半径；
原地旋转角度不准时校正轮距。推荐调参顺序为：激光外参/时间戳 → 轮半径与轮距 → odom →
SLAM/AMCL → costmap → Nav2 速度与控制器。

## 常见问题

- **没有 `/scan`**：确认 Gazebo 已加载 `gz-sim-sensors-system`，再检查
  `ros2 topic list | grep scan`；Gazebo 和 ROS 两侧都必须有 `/scan`。
- **机器人不动**：检查 `ros2 control list_controllers` 中 `base_controller` 是否是
  `active`，并确认速度发到了 `/cmd_vel`。
- **RViz 报 TF 缺失**：检查是否恰好只有一条 `odom -> base_footprint`；建图时由
  SLAM 发布 `map -> odom`，导航时由 AMCL 发布它。
- **Nav2 无法避障**：先确认 `/scan` 的 `frame_id=laser`，再检查 RViz 中激光与实体墙是否
  重合，最后才调整 costmap 和 inflation 参数。
