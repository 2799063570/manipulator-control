# `wheeltec_base_driver`

这是一个不依赖 `ros2_control` 的 ROS 2 Jazzy Wheeltec 底盘驱动。它保留原 ROS 1 `wheeltec_robot_node` 的串口帧协议，并把 Nav2 或其他上层节点的 `/cmd_vel` 直接转交给底盘控制板。

```text
Nav2 controller_server / 手柄 / 跟随节点
                 │
                 ▼
              /cmd_vel
                 │
                 ▼
       wheeltec_base_driver_node
                 │  11-byte serial command frame
                 ▼
          Wheeltec STM32 controller
                 │  24-byte serial state frame
                 ▼
  /odom, /imu, /PowerVoltage, odom → base_footprint TF
```

## 提供的 ROS 接口

| 类型 | 名称 | 说明 |
|---|---|---|
| 订阅 | `/cmd_vel` (`geometry_msgs/msg/Twist`) | 线速度 x/y 与 z 轴角速度。差速底盘请设置 `accept_lateral_velocity: false`。 |
| 发布 | `/odom` (`nav_msgs/msg/Odometry`) | 根据底盘返回速度积分得到的轮式里程计。 |
| 发布 | `/imu` (`sensor_msgs/msg/Imu`) | 控制板原始 IMU 数据。该节点不伪造姿态，`orientation_covariance[0] = -1`。 |
| 发布 | `/PowerVoltage` (`std_msgs/msg/Float32`) | 电池电压，单位 V。 |
| 发布 | `odom → base_footprint` | 默认启用，可通过 `publish_tf` 关闭。 |

## 构建与启动

在 Jetson Nano 的 ROS 2 Jazzy 环境中执行：

```bash
cd ~/robot_move_control
colcon build --packages-select wheeltec_base_driver
source install/setup.bash
ros2 launch wheeltec_base_driver wheeltec_base.launch.py
```

配置文件是 `config/wheeltec_base_driver.yaml`。首次使用前确认 `serial_port` 与 udev 规则一致；默认值是 `/dev/wheeltec_controller`。

## 上车前验证

先架空车轮或确保急停可用，然后：

```bash
ros2 topic echo /odom
ros2 topic echo /imu
ros2 topic pub --once /cmd_vel geometry_msgs/msg/Twist \
  '{linear: {x: 0.05, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.0}}'
```

节点会对速度限幅；当超过 `command_timeout_sec` 没有收到 `/cmd_vel` 时，会自动下发一次零速度命令。不要同时运行旧的 ROS 1 `wheeltec_robot_node` 或其他占用同一串口的驱动。

## Nav2 对接

该包是 Nav2 底盘层，不是 `ros2_control` 插件。Nav2 的控制器发布 `/cmd_vel` 后即可被本节点接收；仍需另行提供 `/scan`、地图/定位与完整 TF 树。

如将其与 AUBO 机械臂组合：底盘保持本包的 `/cmd_vel` 链路，机械臂仍由 `aubo_i5_hardware` 的 `ros2_control` 与 MoveIt 轨迹控制链路负责。两者不应共用控制话题。
