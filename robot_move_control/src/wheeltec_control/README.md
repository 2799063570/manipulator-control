# `wheeltec_control`

This package keeps velocity sources separate and sends the selected command to
the base on `/cmd_vel`.

```text
/cmd_vel_teleop -- manual priority --+
                                   +--> wheeltec_cmd_mux --> /cmd_vel
/cmd_vel_smoothed - Nav2 after speed limiting -+
```

During mapping, start the simulator and SLAM normally, then run this in a
second terminal:

```bash
ros2 run wheeltec_control keyboard_teleop
```

Use `w` / `s` for forward and backward, `q` / `e` for rotation, and Space to
stop. Commands time out after 0.5 seconds, so
the robot stops if the terminal closes or key input stops.

`wheeltec_gazebo navigation.launch.py` enables Nav2 automatically. Manual
keyboard commands have higher priority and temporarily take control. To toggle
Nav2 while it is running:

```bash
ros2 param set /wheeltec_cmd_mux navigation_enabled false
ros2 param set /wheeltec_cmd_mux navigation_enabled true
```
