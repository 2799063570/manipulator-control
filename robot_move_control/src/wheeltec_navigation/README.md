# `wheeltec_navigation`

这是 Wheeltec 移动底盘的 ROS 2 Jazzy 导航配置包。它不重复实现 Nav2、AMCL 或 SLAM 算法，而是将标准 ROS 2 包的接口和底盘驱动统一为一套可落地的启动方式。

## 运行架构

```text
wheeltec_base_driver ──> /odom、odom → base_footprint TF
雷达驱动               ──> /scan、base_footprint → laser TF
slam_toolbox            ──> /map、map → odom TF               （建图）
map_server + AMCL       ──> /map、map → odom TF               （定位）
Nav2                    ──> /cmd_vel ──> wheeltec_base_driver （导航）
```

一个时刻只能有一个节点发布 `map → odom`：建图时由 `slam_toolbox` 发布；静态地图导航时由 AMCL 发布。不要将建图启动和 AMCL 定位启动在同一 ROS 图中同时运行。

## 前置条件

1. `wheeltec_base_driver` 已运行，并稳定提供 `/odom` 与 `odom → base_footprint`。
2. 雷达驱动已运行，提供 `/scan`，并存在 `base_footprint → laser` 的静态 TF。
3. 安装 Jazzy 运行依赖：

```bash
sudo apt install ros-jazzy-navigation2 ros-jazzy-nav2-bringup ros-jazzy-slam-toolbox
```

4. 构建工作区：

```bash
cd ~/robot_move_control
colcon build --packages-select wheeltec_navigation
source install/setup.bash
```

## 模式一：手动建图

终端 1：启动底盘与雷达驱动。终端 2：

```bash
ros2 launch wheeltec_navigation mapping.launch.py
```

然后使用手柄或低速键盘遥控建图；此模式不启动 Nav2，不会自行发布导航速度。保存地图：

```bash
ros2 run nav2_map_server map_saver_cli -f ~/maps/wheeltec_lab
```

该命令生成 `wheeltec_lab.yaml` 和 `wheeltec_lab.pgm`。

## 模式二：静态地图定位

```bash
ros2 launch wheeltec_navigation localization.launch.py \
  map_file:=$HOME/maps/wheeltec_lab.yaml
```

在 RViz 中使用 **2D Pose Estimate** 设置机器人初始位置。真机启动时不会再默认假设机器人位于地图原点；收到初始位姿前没有 `map → odom` 属于正常状态。此模式仅完成地图加载与 AMCL 定位，不会接收导航目标。

## 模式三：自主导航

```bash
ros2 launch wheeltec_navigation navigation.launch.py \
  map_file:=$HOME/maps/wheeltec_lab.yaml
```

该启动文件加载地图、AMCL 和完整 Nav2。启动后先使用 **2D Pose Estimate** 设置初始位置，再发送 **Nav2 Goal**；Nav2 通过 `/cmd_vel` 控制底盘。

导航节点由 `nav2_lifecycle_bringup.py` 逐个配置和激活。它会在 lifecycle 服务响应偏慢时查询节点真实状态并重试，避免 `planner_server` 的 global costmap 首次加载地图稍慢就让整套 Nav2 bringup abort。

## 首次调参顺序

1. 在 RViz 确认 `/scan` 方向、车体模型和 TF 完全重合。
2. 量取实车外接矩形，先更新 `robot_radius`；不要让代价地图小于真实车体。
3. 设置低速限制：`max_vel_x/y/theta`、`max_velocity` 和底盘驱动的速度上限应一致。
4. 再调 `inflation_radius`、DWB 局部规划参数和 AMCL 噪声参数。

Gazebo 教学模型使用麦克纳姆运动学：AMCL 使用 `OmniMotionModel`，局部控制器使用 DWB，并允许 `linear.y` 横移。真实车如果横移打滑明显，需要重新标定轮子摩擦、里程计模型、AMCL 噪声和 Nav2 的 y 向速度限制。

## 常见排障顺序

```bash
ros2 topic echo /scan --once
ros2 topic echo /odom --once
ros2 run tf2_ros tf2_echo odom base_footprint
ros2 run tf2_ros tf2_echo map odom
ros2 topic echo /cmd_vel
```

按照 `/scan` → `/odom` → TF → `/map` → AMCL 位姿 → 全局路径 → 局部路径 → `/cmd_vel` 的顺序排查。真实车首次导航请保持急停可用。
