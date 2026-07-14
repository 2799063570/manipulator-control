---
title: 实验 5：差速小车 Gazebo、里程计、SLAM、Nav2
tags: [实验, 移动机器人, Gazebo, SLAM, Nav2]
---

# 实验 5：差速小车 Gazebo、里程计、SLAM、Nav2

## 实验目标

基于 `fishbot_description` 的底盘、雷达和相机，建立从 `/cmd_vel` 到轮子、从轮子到 `/odom`、从激光到地图、从地图到 Nav2 目标点的闭环。重点是每条 TF 和每份数据的物理含义正确。

## 1. 知识点：差速运动学

左右轮有效半径 r、轮中心距 L。线速度等于 r 乘左右轮角速度之和的一半；角速度等于 r/L 乘右轮减左轮角速度。反过来，给定线速度 v 和角速度 w，右轮角速度为 `(v + L*w/2)/r`，左轮为 `(v - L*w/2)/r`。

用 r=0.05 m、L=0.32 m 验算：v=0.2 m/s、w=0 时两轮均 4 rad/s；v=0、w=0.5 rad/s 时右轮 1.6 rad/s、左轮 -1.6 rad/s。实际部署必须使用实测有效半径和轮中心距。

## 2. 实验 A：模型与 TF

先只在 RViz 打开小车模型，验证 `base_link`、左右轮、`laser`、相机的 parent-child。轮子应围绕正确轴转，传感器不得跟随轮子转。

```bash
ros2 run tf2_tools view_frames
ros2 run tf2_ros tf2_echo base_link laser
```

坐标遵循 REP-103：x 前、y 左、z 上；逆时针 yaw 为正。先统一约定，后面所有正负号才不会乱。

## 3. 实验 B：diff_drive 与 odom

创建 `fishbot_gazebo`，让底盘通过 `gz_ros2_control` 接入 `diff_drive_controller`。启动后：

```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard
ros2 topic echo /cmd_vel
ros2 topic echo /odom
```

进行三项测试：直行 2 m（x 单调增加，y 漂移小）；原地转一圈（yaw 接近 2*pi）；方形路径（四段直线四次 90 度后回近起点）。轮径主要校正直线距离，轮距主要校正旋转角；一次只改一个参数，才可归因。

## 4. 实验 C：激光雷达

现有 `radius_simulator.xacro` 使用 Gazebo Classic 的 `libgazebo_ros_laser.so`。迁移到 Gazebo Sim 时先确认 GZ ray sensor 自己有数据，再用 `ros_gz_bridge` 变成 ROS LaserScan。随后：

```bash
ros2 topic hz /scan
ros2 topic echo /scan --once
```

在 RViz 先以 `laser` 为 Fixed Frame 看扫描形状，再切 `odom` 检查它随车运动是否正确。墙放正前方时，ranges 中间读数应减小；反向通常是 sensor yaw 或 frame 错。

## 5. 实验 D：EKF、SLAM 与 Nav2

`odom` 连续但会漂移，`map` 全局一致但会校正跳变。通常 EKF 发布 `odom -> base_link`，SLAM/AMCL 发布 `map -> odom`，同一 TF 只能一个发布者。先用编码器和 IMU 建 `robot_localization` EKF，再低速遥控运行 `slam_toolbox` 建图：

```bash
ros2 run nav2_map_server map_saver_cli -f ~/maps/lab
```

保存 yaml 和图像这对地图文件。使用 AMCL + Nav2 时，先在 RViz 设 initial pose，再发 goal；逐项观察 `/plan`、local costmap、`/cmd_vel` 和 TF，导航失败不要只盯着车是否移动。

## 验收

- [ ] cmd_vel 正方向正确，停止后轮速归零。
- [ ] odom、scan 稳定，只有一条 odom 到 base_link TF。
- [ ] 方形测试的误差有量化记录。
- [ ] 建图无明显重影，重启后 AMCL 可定位。
- [ ] 连续 20 个目标导航成功率与最小障碍距离有记录。

