<div align='center'>
<h1 align='center'>Aubo Sdk RTDE Recipe </h1>
</div>

## 输入菜单
| 名称 | 数据类型 | 说明 |
| :----:| :----: | :----: |
|set_recipe|RtdeRecipe|Set |
|input_bit_registers0_to_31|int|General purpose bits This range of the boolean input registers is reserved forFieldBus/PLC interface usage.|
|input_bit_registers32_to_63|int|General purpose bits This range of the boolean input registers is reserved forFieldBus/PLC interface usage.|
|input_bit_registers64_to_127|int64_t|64 general purpose bits X: [64..127] - The upper range of the boolean input registers canbe used by external RTDE clients (i.e AUBOCAPS).|
|input_int_registers_0|int|48 general purpose integer registers X: [0..23] - The lower range of the integer input registers isreserved for FieldBus/PLC interface usage. X: [24..47] - Theupper range of the integer input registers can be used byexternal RTDE clients (i.e AUBOCAPS).|
|input_float_registers_0|float|48 general purpose integer registers X: [0..23] - The lower range of the integer input registers isreserved for FieldBus/PLC interface usage. X: [24..47] - Theupper range of the integer input registers can be used byexternal RTDE clients (i.e AUBOCAPS).|
|input_double_registers_0|double|48 general purpose double registersX: [0..23]  - The lower range of the double input registers is reserved for FieldBus/PLC interface usage.X: [24..47] - The upper range of the double input registers can be used by external RTDE clients (i.e AUBOCAPS).|
|input_int16_registers_0|double||
|input_int16_registers0_to_63|std::vector<double>||
|R1_speed_slider_mask|double|0 = don't change speed slider with this input <br> = use speed_slider_fraction to set speed slider value|
|R1_speed_slider_fraction|double|new speed slider value |
|R1_standard_digital_output_mask|uint32_t|Standard digital outputs |
|R1_configurable_digital_output_mask|uint32_t|Configurable digital outputs|
|R1_standard_digital_output|uint32_t|Standard digital outputs |
|R1_configurable_digital_output|uint32_t|Configurable digital outputs|
|R1_tool_digital_output|uint32_t|Tool digital outputs <br>its 0-1: output state, remaining bits are reserved for future use|
|R1_standard_analog_output_type|std::vector<int>|Output domain {0=current[A], 1=voltage[V]} <br>Bits 0-1: standard_analog_output_0 | standard_analog_output_1|
|R1_standard_analog_output_mask|uint32_t|Standard analog output 0 (ratio) [0..1] |
|R1_standard_analog_output|std::vector<double>|Standard analog output 1 (ratio) [0..1] |
|R1_debug|uint32_t|Debug for internal use |
|R1_tool_digital_output_mask|uint32_t|Tool digital outputs mask|
|R1_rtde_input_max|int||
|R2_speed_slider_mask|double|0 = don't change speed slider with this input <br> = use speed_slider_fraction to set speed slider value|
|R2_speed_slider_fraction|double|new speed slider value |
|R2_standard_digital_output_mask|uint32_t|Standard digital outputs |
|R2_configurable_digital_output_mask|uint32_t|Configurable digital outputs|
|R2_standard_digital_output|uint32_t|Standard digital outputs |
|R2_configurable_digital_output|uint32_t|Configurable digital outputs|
|R2_tool_digital_output|uint32_t|Tool digital outputs <br>its 0-1: output state, remaining bits are reserved for future use|
|R2_standard_analog_output_type|std::vector<int>|Output domain {0=current[A], 1=voltage[V]} <br>Bits 0-1: standard_analog_output_0 | standard_analog_output_1|
|R2_standard_analog_output_mask|uint32_t|Standard analog output 0 (ratio) [0..1] |
|R2_standard_analog_output|std::vector<double>|Standard analog output 1 (ratio) [0..1] |
|R2_debug|uint32_t|Debug for internal use |
|R2_tool_digital_output_mask|uint32_t|Tool digital outputs mask|
|R2_rtde_input_max|int||
|R3_speed_slider_mask|double|0 = don't change speed slider with this input <br> = use speed_slider_fraction to set speed slider value|
|R3_speed_slider_fraction|double|new speed slider value |
|R3_standard_digital_output_mask|uint32_t|Standard digital outputs |
|R3_configurable_digital_output_mask|uint32_t|Configurable digital outputs|
|R3_standard_digital_output|uint32_t|Standard digital outputs |
|R3_configurable_digital_output|uint32_t|Configurable digital outputs|
|R3_tool_digital_output|uint32_t|Tool digital outputs <br>its 0-1: output state, remaining bits are reserved for future use|
|R3_standard_analog_output_type|std::vector<int>|Output domain {0=current[A], 1=voltage[V]} <br>Bits 0-1: standard_analog_output_0 | standard_analog_output_1|
|R3_standard_analog_output_mask|uint32_t|Standard analog output 0 (ratio) [0..1] |
|R3_standard_analog_output|std::vector<double>|Standard analog output 1 (ratio) [0..1] |
|R3_debug|uint32_t|Debug for internal use |
|R3_tool_digital_output_mask|uint32_t|Tool digital outputs mask|
|R3_rtde_input_max|int||
|R4_speed_slider_mask|double|0 = don't change speed slider with this input <br> = use speed_slider_fraction to set speed slider value|
|R4_speed_slider_fraction|double|new speed slider value |
|R4_standard_digital_output_mask|uint32_t|Standard digital outputs |
|R4_configurable_digital_output_mask|uint32_t|Configurable digital outputs|
|R4_standard_digital_output|uint32_t|Standard digital outputs |
|R4_configurable_digital_output|uint32_t|Configurable digital outputs|
|R4_tool_digital_output|uint32_t|Tool digital outputs <br>its 0-1: output state, remaining bits are reserved for future use|
|R4_standard_analog_output_type|std::vector<int>|Output domain {0=current[A], 1=voltage[V]} <br>Bits 0-1: standard_analog_output_0 | standard_analog_output_1|
|R4_standard_analog_output_mask|uint32_t|Standard analog output 0 (ratio) [0..1] |
|R4_standard_analog_output|std::vector<double>|Standard analog output 1 (ratio) [0..1] |
|R4_debug|uint32_t|Debug for internal use |
|R4_tool_digital_output_mask|uint32_t|Tool digital outputs mask|
|R4_rtde_input_max|int||
## 输出菜单
| 名称 | 数据类型 | 说明 |
| :----:| :----: | :----: |
|timestamp|double|Time elapsed since the controller was started [s]|
|line_number|int|line number set by setPlanContext|
|runtime_state|RuntimeState|Program state|
|output_bit_registers_0_to_63|int64_t|64 [000..063] General purpose bits|
|output_bit_registers_64_to_127|int64_t|64 [064..127] general purpose bits|
|output_int_registers_0|int|48 general purpose integer registersX: [0..23] - The lower range of the integer output registers isreserved for FieldBus/PLC interface usage. X: [24..47] - The upper range of the integer output registers can be used by external RTDE clients (i.e AUBOCAPS).|
|output_float_registers_0|int|48 general purpose integer registersX: [0..23] - The lower range of the integer output registers isreserved for FieldBus/PLC interface usage. X: [24..47] - The upper range of the integer output registers can be used by external RTDE clients (i.e AUBOCAPS).|
|output_double_registers_0|double|48 general purpose double registersX: [0..23] - The lower range of the double output registers isreserved for FieldBus/PLC interface usage. X: [24..47] - The upper range of the double output registers can be used by external RTDE clients (i.e AUBOCAPS).|
|input_bit_registers_r0_to_63|int64_t|[0..63] General purpose bits This range of the boolean outputregisters is reserved for FieldBus/PLC interface usage.|
|input_bit_registers_r64_to_127|int64_t|64 [64..127] general purpose bits|
|input_int_registers_r0|int|([0 .. 48]) 48 general purpose integer registersX: [0..23] - The lower range of the integer input registers isreserved for FieldBus/PLC interface usage. X: [24..47] - Theupper range of the integer input registers can be used byexternal RTDE clients (i.e AUBOCAPS).|
|input_float_registers_r0|int|([0 .. 48]) 48 general purpose integer registersX: [0..23] - The lower range of the integer input registers isreserved for FieldBus/PLC interface usage. X: [24..47] - Theupper range of the integer input registers can be used byexternal RTDE clients (i.e AUBOCAPS).|
|input_double_registers_r0|double|([0 .. 48]) 48 general purpose double registersX: [0..23] - The lower range of the double input registers isreserved for FieldBus/PLC interface usage. X: [24..47] - Theupper range of the double input registers can be used byexternal RTDE clients (i.e AUBOCAPS).|
|modbus_signals|std::vector<int>|Modbus signals from connected modbus slaves|
|modbus_signals_errors|std::vector<int>|Modbus signals request status from connected modbus slaves|
|input_int16_registers_r0|int16_t|input_int16_registers_r0|
|input_int16_registers_0_to_63|std::vector<int16_t>| |
|runtime_context|std::vector<int>| |
|R1_message|RobotMsg|Robot message from controller|
|R1_target_q|std::vector<double>|Target joint positions|
|R1_target_qd|std::vector<double>|Target joint velocities|
|R1_target_qdd|std::vector<double>|Target joint accelerations|
|R1_target_current|std::vector<double>|Target joint currents|
|R1_target_moment|std::vector<double>|Target joint moments (torques)|
|R1_actual_q|std::vector<double>|Actual joint positions|
|R1_actual_qd|std::vector<double>|Actual joint velocities|
|R1_actual_current|std::vector<double>|Actual joint currents|
|R1_joint_control_output|std::vector<double>|Joint control currents|
|R1_joint_temperatures|std::vector<double>|Temperature of each joint in degrees Celsius|
|R1_actual_joint_voltage|std::vector<double>|Actual joint voltages|
|R1_joint_mode|std::vector<JointStateType>|Joint control modes Please see Remote Control Via TCP/IP - 16496|
|R1_actual_execution_time|double|Controller real-time thread execution time|
|R1_robot_mode|RobotModeType|Robot mode Please see Remote Control Via TCP/IP - 16496|
|R1_safety_mode|SafetyModeType|Safety mode Please see Remote Control Via TCP/IP - 16496|
|R1_safety_status|unknown|Safety ststus|
|R1_robot_status_bits|unknown|Bits 0-3: Is power on | Is program running | Isteach button pressed | Is power button pressed|
|R1_safety_status_bits|unknown|Bits 0-10: Is normal mode | Is reduced mode | | Is protectivestopped | Is recovery mode | Is safeguard stopped | Is systememergency stopped | Is robot emergency stopped | Is emergencystopped | Is violation | Is fault | Is stopped due to safety|
|R1_speed_scaling|double|Speed scaling of the trajectory limiter|
|R1_target_speed_fraction|double|Target speed fraction|
|R1_actual_TCP_pose|std::vector<double>|Actual Cartesian coordinates of the tool:(x,y,z,rx,ry,rz), where rx, ry and rz is a rotationvector representation of the tool orientation|
|R1_actual_TCP_speed|std::vector<double>|Actual speed of the tool given in Cartesian coordinates|
|R1_actual_TCP_force|std::vector<double>|Generalized forces in the TCP|
|R1_target_TCP_pose|std::vector<double>|Target Cartesian coordinates of the tool:(x,y,z,rx,ry,rz), where rx, ry and rz is a rotationvector representation of the tool orientation|
|R1_target_TCP_speed|std::vector<double>|Target speed of the tool given in Cartesian coordinates|
|R1_elbow_position|std::vector<double>|Position of robot elbow in Cartesian Base Coordinates|
|R1_elbow_velocity|std::vector<double>|Velocity of robot elbow in Cartesian Base Coordinates|
|R1_actual_momentum|std::vector<double>|Norm of Cartesian linear momentum|
|R1_tcp_force_scalar|std::vector<double>|TCP force scalar [N]|
|R1_future_path_points|std::vector<std::vector<double>>|Get future path points joint|
|R1_actual_main_voltage|unknown|Safety Control Board: Main voltage|
|R1_actual_robot_voltage|unknown|Safety Control Board: Robot voltage (48V)|
|R1_actual_robot_current|unknown|Safety Control Board: Robot current|
|R1_joint_torque_sensor|std::vector<double>|Joint torque sensor|
|R1_operationalModeSelectorInput|OperationalModeType|Current state of the operational mode selector input|
|R1_threePositionEnablingDeviceInput|unknown||
|R1_masterboard_temperature|unknown||
|R1_standard_digital_input_bits|uint64_t|Current state of the standard digital inputs.|
|R1_tool_digital_input_bits|uint64_t|Current state of the tool digital inputs and outputs.|
|R1_configurable_digital_input_bits|uint64_t|Current state of the safety inputs.|
|R1_link_digital_input_bits|uint64_t|Current state of the link digital inputs.|
|R1_standard_digital_output_bits|uint64_t|Current state of the standard digital outputs.|
|R1_tool_digital_output_bits|uint64_t|Current state of the tool digital inputs and outputs.|
|R1_configurable_digital_output_bits|uint64_t|Current state of the safety outputs.|
|R1_link_digital_output_bits|uint64_t|Current state of the link digital outputs.|
|R1_standard_analog_input_values|std::vector<double>|Current values of the standard analog inputs.|
|R1_tool_analog_input_values|std::vector<double>|Current values of the tool analog inputs.|
|R1_standard_analog_output_values|std::vector<double>|Current values of the standard analog outputs.|
|R1_tool_analog_output_values|std::vector<double>|Current values of the tool analog outputs.|
|R1_is_simulation_enabled|bool||
|R1_collision_level|int||
|R1_master_io_current|unknown|I/O current [A]|
|R1_euromap67_input_bits|unknown|Euromap67 input bits|
|R1_euromap67_output_bits|unknown|Euromap67 output bits|
|R1_euromap67_24V_voltage|unknown|Euromap 24V voltage [V]|
|R1_euromap67_24V_current|unknown|Euromap 24V current [A]|
|R1_tool_mode|unknown|Tool mode Please see Remote Control Via TCP/IP - 16496|
|R1_tool_output_mode|unknown|The current output mode|
|R1_tool_output_voltage|unknown|Tool output voltage [V]|
|R1_tool_output_current|unknown|Tool current [A]|
|R1_tool_voltage_48V|unknown||
|R1_tool_current|unknown||
|R1_tool_temperature|unknown|Tool temperature in degrees Celsius|
|R1_actual_tool_accelerometer|unknown|Tool x, y and z accelerometer values|
|R1_motion_progress|unknown|Trajectory running progress|
|R1_actual_qdd|unknown|Actual joint accelerations|
|R1_controlbox_humidity|double|Controbox humidity|
|R1_actual_tool_pose|std::vector<double>|Actual Cartesian coordinates of the tool(without TCP)|
|R1_rtde_output_max|int||
|R1_actual_TCP_force_sensor|std::vector<double>|TCP force sensor|
|R1_fc_cond_fullfiled|bool||
|R1_actual_payload|Payload|Actual payload|
|R1_tool_button_status|bool|Tool button status|
|R2_message|RobotMsg|Robot message from controller|
|R2_target_q|std::vector<double>|Target joint positions|
|R2_target_qd|std::vector<double>|Target joint velocities|
|R2_target_qdd|std::vector<double>|Target joint accelerations|
|R2_target_current|std::vector<double>|Target joint currents|
|R2_target_moment|std::vector<double>|Target joint moments (torques)|
|R2_actual_q|std::vector<double>|Actual joint positions|
|R2_actual_qd|std::vector<double>|Actual joint velocities|
|R2_actual_current|std::vector<double>|Actual joint currents|
|R2_joint_control_output|std::vector<double>|Joint control currents|
|R2_joint_temperatures|std::vector<double>|Temperature of each joint in degrees Celsius|
|R2_actual_joint_voltage|std::vector<double>|Actual joint voltages|
|R2_joint_mode|std::vector<JointStateType>|Joint control modes Please see Remote Control Via TCP/IP - 16496|
|R2_actual_execution_time|double|Controller real-time thread execution time|
|R2_robot_mode|RobotModeType|Robot mode Please see Remote Control Via TCP/IP - 16496|
|R2_safety_mode|SafetyModeType|Safety mode Please see Remote Control Via TCP/IP - 16496|
|R2_safety_status|unknown|Safety ststus|
|R2_robot_status_bits|unknown|Bits 0-3: Is power on | Is program running | Isteach button pressed | Is power button pressed|
|R2_safety_status_bits|unknown|Bits 0-10: Is normal mode | Is reduced mode | | Is protectivestopped | Is recovery mode | Is safeguard stopped | Is systememergency stopped | Is robot emergency stopped | Is emergencystopped | Is violation | Is fault | Is stopped due to safety|
|R2_speed_scaling|double|Speed scaling of the trajectory limiter|
|R2_target_speed_fraction|double|Target speed fraction|
|R2_actual_TCP_pose|std::vector<double>|Actual Cartesian coordinates of the tool:(x,y,z,rx,ry,rz), where rx, ry and rz is a rotationvector representation of the tool orientation|
|R2_actual_TCP_speed|std::vector<double>|Actual speed of the tool given in Cartesian coordinates|
|R2_actual_TCP_force|std::vector<double>|Generalized forces in the TCP|
|R2_target_TCP_pose|std::vector<double>|Target Cartesian coordinates of the tool:(x,y,z,rx,ry,rz), where rx, ry and rz is a rotationvector representation of the tool orientation|
|R2_target_TCP_speed|std::vector<double>|Target speed of the tool given in Cartesian coordinates|
|R2_elbow_position|std::vector<double>|Position of robot elbow in Cartesian Base Coordinates|
|R2_elbow_velocity|std::vector<double>|Velocity of robot elbow in Cartesian Base Coordinates|
|R2_actual_momentum|std::vector<double>|Norm of Cartesian linear momentum|
|R2_tcp_force_scalar|std::vector<double>|TCP force scalar [N]|
|R2_future_path_points|std::vector<std::vector<double>>|Get future path points joint|
|R2_actual_main_voltage|unknown|Safety Control Board: Main voltage|
|R2_actual_robot_voltage|unknown|Safety Control Board: Robot voltage (48V)|
|R2_actual_robot_current|unknown|Safety Control Board: Robot current|
|R2_joint_torque_sensor|std::vector<double>|Joint torque sensor|
|R2_operationalModeSelectorInput|OperationalModeType|Current state of the operational mode selector input|
|R2_threePositionEnablingDeviceInput|unknown||
|R2_masterboard_temperature|unknown||
|R2_standard_digital_input_bits|uint64_t|Current state of the standard digital inputs.|
|R2_tool_digital_input_bits|uint64_t|Current state of the tool digital inputs and outputs.|
|R2_configurable_digital_input_bits|uint64_t|Current state of the safety inputs.|
|R2_link_digital_input_bits|uint64_t|Current state of the link digital inputs.|
|R2_standard_digital_output_bits|uint64_t|Current state of the standard digital outputs.|
|R2_tool_digital_output_bits|uint64_t|Current state of the tool digital inputs and outputs.|
|R2_configurable_digital_output_bits|uint64_t|Current state of the safety outputs.|
|R2_link_digital_output_bits|uint64_t|Current state of the link digital outputs.|
|R2_standard_analog_input_values|std::vector<double>|Current values of the standard analog inputs.|
|R2_tool_analog_input_values|std::vector<double>|Current values of the tool analog inputs.|
|R2_standard_analog_output_values|std::vector<double>|Current values of the standard analog outputs.|
|R2_tool_analog_output_values|std::vector<double>|Current values of the tool analog outputs.|
|R2_is_simulation_enabled|bool||
|R2_collision_level|int||
|R2_master_io_current|unknown|I/O current [A]|
|R2_euromap67_input_bits|unknown|Euromap67 input bits|
|R2_euromap67_output_bits|unknown|Euromap67 output bits|
|R2_euromap67_24V_voltage|unknown|Euromap 24V voltage [V]|
|R2_euromap67_24V_current|unknown|Euromap 24V current [A]|
|R2_tool_mode|unknown|Tool mode Please see Remote Control Via TCP/IP - 16496|
|R2_tool_output_mode|unknown|The current output mode|
|R2_tool_output_voltage|unknown|Tool output voltage [V]|
|R2_tool_output_current|unknown|Tool current [A]|
|R2_tool_voltage_48V|unknown||
|R2_tool_current|unknown||
|R2_tool_temperature|unknown|Tool temperature in degrees Celsius|
|R2_actual_tool_accelerometer|unknown|Tool x, y and z accelerometer values|
|R2_motion_progress|unknown|Trajectory running progress|
|R2_actual_qdd|unknown|Actual joint accelerations|
|R2_controlbox_humidity|double|Controbox humidity|
|R2_actual_tool_pose|std::vector<double>|Actual Cartesian coordinates of the tool(without TCP)|
|R2_rtde_output_max|int||
|R2_actual_TCP_force_sensor|std::vector<double>|TCP force sensor|
|R2_fc_cond_fullfiled|bool||
|R2_actual_payload|Payload|Actual payload|
|R2_tool_button_status|bool|Tool button status|
|R3_message|RobotMsg|Robot message from controller|
|R3_target_q|std::vector<double>|Target joint positions|
|R3_target_qd|std::vector<double>|Target joint velocities|
|R3_target_qdd|std::vector<double>|Target joint accelerations|
|R3_target_current|std::vector<double>|Target joint currents|
|R3_target_moment|std::vector<double>|Target joint moments (torques)|
|R3_actual_q|std::vector<double>|Actual joint positions|
|R3_actual_qd|std::vector<double>|Actual joint velocities|
|R3_actual_current|std::vector<double>|Actual joint currents|
|R3_joint_control_output|std::vector<double>|Joint control currents|
|R3_joint_temperatures|std::vector<double>|Temperature of each joint in degrees Celsius|
|R3_actual_joint_voltage|std::vector<double>|Actual joint voltages|
|R3_joint_mode|std::vector<JointStateType>|Joint control modes Please see Remote Control Via TCP/IP - 16496|
|R3_actual_execution_time|double|Controller real-time thread execution time|
|R3_robot_mode|RobotModeType|Robot mode Please see Remote Control Via TCP/IP - 16496|
|R3_safety_mode|SafetyModeType|Safety mode Please see Remote Control Via TCP/IP - 16496|
|R3_safety_status|unknown|Safety ststus|
|R3_robot_status_bits|unknown|Bits 0-3: Is power on | Is program running | Isteach button pressed | Is power button pressed|
|R3_safety_status_bits|unknown|Bits 0-10: Is normal mode | Is reduced mode | | Is protectivestopped | Is recovery mode | Is safeguard stopped | Is systememergency stopped | Is robot emergency stopped | Is emergencystopped | Is violation | Is fault | Is stopped due to safety|
|R3_speed_scaling|double|Speed scaling of the trajectory limiter|
|R3_target_speed_fraction|double|Target speed fraction|
|R3_actual_TCP_pose|std::vector<double>|Actual Cartesian coordinates of the tool:(x,y,z,rx,ry,rz), where rx, ry and rz is a rotationvector representation of the tool orientation|
|R3_actual_TCP_speed|std::vector<double>|Actual speed of the tool given in Cartesian coordinates|
|R3_actual_TCP_force|std::vector<double>|Generalized forces in the TCP|
|R3_target_TCP_pose|std::vector<double>|Target Cartesian coordinates of the tool:(x,y,z,rx,ry,rz), where rx, ry and rz is a rotationvector representation of the tool orientation|
|R3_target_TCP_speed|std::vector<double>|Target speed of the tool given in Cartesian coordinates|
|R3_elbow_position|std::vector<double>|Position of robot elbow in Cartesian Base Coordinates|
|R3_elbow_velocity|std::vector<double>|Velocity of robot elbow in Cartesian Base Coordinates|
|R3_actual_momentum|std::vector<double>|Norm of Cartesian linear momentum|
|R3_tcp_force_scalar|std::vector<double>|TCP force scalar [N]|
|R3_future_path_points|std::vector<std::vector<double>>|Get future path points joint|
|R3_actual_main_voltage|unknown|Safety Control Board: Main voltage|
|R3_actual_robot_voltage|unknown|Safety Control Board: Robot voltage (48V)|
|R3_actual_robot_current|unknown|Safety Control Board: Robot current|
|R3_joint_torque_sensor|std::vector<double>|Joint torque sensor|
|R3_operationalModeSelectorInput|OperationalModeType|Current state of the operational mode selector input|
|R3_threePositionEnablingDeviceInput|unknown||
|R3_masterboard_temperature|unknown||
|R3_standard_digital_input_bits|uint64_t|Current state of the standard digital inputs.|
|R3_tool_digital_input_bits|uint64_t|Current state of the tool digital inputs and outputs.|
|R3_configurable_digital_input_bits|uint64_t|Current state of the safety inputs.|
|R3_link_digital_input_bits|uint64_t|Current state of the link digital inputs.|
|R3_standard_digital_output_bits|uint64_t|Current state of the standard digital outputs.|
|R3_tool_digital_output_bits|uint64_t|Current state of the tool digital inputs and outputs.|
|R3_configurable_digital_output_bits|uint64_t|Current state of the safety outputs.|
|R3_link_digital_output_bits|uint64_t|Current state of the link digital outputs.|
|R3_standard_analog_input_values|std::vector<double>|Current values of the standard analog inputs.|
|R3_tool_analog_input_values|std::vector<double>|Current values of the tool analog inputs.|
|R3_standard_analog_output_values|std::vector<double>|Current values of the standard analog outputs.|
|R3_tool_analog_output_values|std::vector<double>|Current values of the tool analog outputs.|
|R3_is_simulation_enabled|bool||
|R3_collision_level|int||
|R3_master_io_current|unknown|I/O current [A]|
|R3_euromap67_input_bits|unknown|Euromap67 input bits|
|R3_euromap67_output_bits|unknown|Euromap67 output bits|
|R3_euromap67_24V_voltage|unknown|Euromap 24V voltage [V]|
|R3_euromap67_24V_current|unknown|Euromap 24V current [A]|
|R3_tool_mode|unknown|Tool mode Please see Remote Control Via TCP/IP - 16496|
|R3_tool_output_mode|unknown|The current output mode|
|R3_tool_output_voltage|unknown|Tool output voltage [V]|
|R3_tool_output_current|unknown|Tool current [A]|
|R3_tool_voltage_48V|unknown||
|R3_tool_current|unknown||
|R3_tool_temperature|unknown|Tool temperature in degrees Celsius|
|R3_actual_tool_accelerometer|unknown|Tool x, y and z accelerometer values|
|R3_motion_progress|unknown|Trajectory running progress|
|R3_actual_qdd|unknown|Actual joint accelerations|
|R3_controlbox_humidity|double|Controbox humidity|
|R3_actual_tool_pose|std::vector<double>|Actual Cartesian coordinates of the tool(without TCP)|
|R3_rtde_output_max|int||
|R3_actual_TCP_force_sensor|std::vector<double>|TCP force sensor|
|R3_fc_cond_fullfiled|bool||
|R3_actual_payload|Payload|Actual payload|
|R3_tool_button_status|bool|Tool button status|
|R4_message|RobotMsg|Robot message from controller|
|R4_target_q|std::vector<double>|Target joint positions|
|R4_target_qd|std::vector<double>|Target joint velocities|
|R4_target_qdd|std::vector<double>|Target joint accelerations|
|R4_target_current|std::vector<double>|Target joint currents|
|R4_target_moment|std::vector<double>|Target joint moments (torques)|
|R4_actual_q|std::vector<double>|Actual joint positions|
|R4_actual_qd|std::vector<double>|Actual joint velocities|
|R4_actual_current|std::vector<double>|Actual joint currents|
|R4_joint_control_output|std::vector<double>|Joint control currents|
|R4_joint_temperatures|std::vector<double>|Temperature of each joint in degrees Celsius|
|R4_actual_joint_voltage|std::vector<double>|Actual joint voltages|
|R4_joint_mode|std::vector<JointStateType>|Joint control modes Please see Remote Control Via TCP/IP - 16496|
|R4_actual_execution_time|double|Controller real-time thread execution time|
|R4_robot_mode|RobotModeType|Robot mode Please see Remote Control Via TCP/IP - 16496|
|R4_safety_mode|SafetyModeType|Safety mode Please see Remote Control Via TCP/IP - 16496|
|R4_safety_status|unknown|Safety ststus|
|R4_robot_status_bits|unknown|Bits 0-3: Is power on | Is program running | Isteach button pressed | Is power button pressed|
|R4_safety_status_bits|unknown|Bits 0-10: Is normal mode | Is reduced mode | | Is protectivestopped | Is recovery mode | Is safeguard stopped | Is systememergency stopped | Is robot emergency stopped | Is emergencystopped | Is violation | Is fault | Is stopped due to safety|
|R4_speed_scaling|double|Speed scaling of the trajectory limiter|
|R4_target_speed_fraction|double|Target speed fraction|
|R4_actual_TCP_pose|std::vector<double>|Actual Cartesian coordinates of the tool:(x,y,z,rx,ry,rz), where rx, ry and rz is a rotationvector representation of the tool orientation|
|R4_actual_TCP_speed|std::vector<double>|Actual speed of the tool given in Cartesian coordinates|
|R4_actual_TCP_force|std::vector<double>|Generalized forces in the TCP|
|R4_target_TCP_pose|std::vector<double>|Target Cartesian coordinates of the tool:(x,y,z,rx,ry,rz), where rx, ry and rz is a rotationvector representation of the tool orientation|
|R4_target_TCP_speed|std::vector<double>|Target speed of the tool given in Cartesian coordinates|
|R4_elbow_position|std::vector<double>|Position of robot elbow in Cartesian Base Coordinates|
|R4_elbow_velocity|std::vector<double>|Velocity of robot elbow in Cartesian Base Coordinates|
|R4_actual_momentum|std::vector<double>|Norm of Cartesian linear momentum|
|R4_tcp_force_scalar|std::vector<double>|TCP force scalar [N]|
|R4_future_path_points|std::vector<std::vector<double>>|Get future path points joint|
|R4_actual_main_voltage|unknown|Safety Control Board: Main voltage|
|R4_actual_robot_voltage|unknown|Safety Control Board: Robot voltage (48V)|
|R4_actual_robot_current|unknown|Safety Control Board: Robot current|
|R4_joint_torque_sensor|std::vector<double>|Joint torque sensor|
|R4_operationalModeSelectorInput|OperationalModeType|Current state of the operational mode selector input|
|R4_threePositionEnablingDeviceInput|unknown||
|R4_masterboard_temperature|unknown||
|R4_standard_digital_input_bits|uint64_t|Current state of the standard digital inputs.|
|R4_tool_digital_input_bits|uint64_t|Current state of the tool digital inputs and outputs.|
|R4_configurable_digital_input_bits|uint64_t|Current state of the safety inputs.|
|R4_link_digital_input_bits|uint64_t|Current state of the link digital inputs.|
|R4_standard_digital_output_bits|uint64_t|Current state of the standard digital outputs.|
|R4_tool_digital_output_bits|uint64_t|Current state of the tool digital inputs and outputs.|
|R4_configurable_digital_output_bits|uint64_t|Current state of the safety outputs.|
|R4_link_digital_output_bits|uint64_t|Current state of the link digital outputs.|
|R4_standard_analog_input_values|std::vector<double>|Current values of the standard analog inputs.|
|R4_tool_analog_input_values|std::vector<double>|Current values of the tool analog inputs.|
|R4_standard_analog_output_values|std::vector<double>|Current values of the standard analog outputs.|
|R4_tool_analog_output_values|std::vector<double>|Current values of the tool analog outputs.|
|R4_is_simulation_enabled|bool||
|R4_collision_level|int||
|R4_master_io_current|unknown|I/O current [A]|
|R4_euromap67_input_bits|unknown|Euromap67 input bits|
|R4_euromap67_output_bits|unknown|Euromap67 output bits|
|R4_euromap67_24V_voltage|unknown|Euromap 24V voltage [V]|
|R4_euromap67_24V_current|unknown|Euromap 24V current [A]|
|R4_tool_mode|unknown|Tool mode Please see Remote Control Via TCP/IP - 16496|
|R4_tool_output_mode|unknown|The current output mode|
|R4_tool_output_voltage|unknown|Tool output voltage [V]|
|R4_tool_output_current|unknown|Tool current [A]|
|R4_tool_voltage_48V|unknown||
|R4_tool_current|unknown||
|R4_tool_temperature|unknown|Tool temperature in degrees Celsius|
|R4_actual_tool_accelerometer|unknown|Tool x, y and z accelerometer values|
|R4_motion_progress|unknown|Trajectory running progress|
|R4_actual_qdd|unknown|Actual joint accelerations|
|R4_controlbox_humidity|double|Controbox humidity|
|R4_actual_tool_pose|std::vector<double>|Actual Cartesian coordinates of the tool(without TCP)|
|R4_rtde_output_max|int||
|R4_actual_TCP_force_sensor|std::vector<double>|TCP force sensor|
|R4_fc_cond_fullfiled|bool||
|R4_actual_payload|Payload|Actual payload|
|R4_tool_button_status|bool|Tool button status|
