# `wheeltec_description`

共享的 Wheeltec `mini_mec` URDF/Xacro 模型。真实车和 Gazebo 仿真都从这份模型获得相同的 link、joint、传感器坐标系和基础尺寸。

## 坐标系与几何约定

```text
odom → base_footprint → base_link → laser
                                  └→ imu_link
```

轮子名称为 `front_left_wheel_joint`、`front_right_wheel_joint`、`rear_left_wheel_joint`、`rear_right_wheel_joint`。模型尺寸来自现有 Wheeltec `mini_mec_robot.urdf`：轮径 75 mm、前后轮中心距 170 mm、左右轮中心距 195 mm。

这些尺寸用于建立“真机与仿真一致”的接口基线；在用 Nav2 配置真实车前，仍应实测车体外廓和雷达外参，并更新代价地图半径与碰撞几何。

显示模型：

```bash
ros2 launch wheeltec_description display.launch.py
```
