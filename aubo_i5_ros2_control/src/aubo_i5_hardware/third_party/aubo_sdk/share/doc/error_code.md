<div align='center'>
<h1 align='center'>Aubo Sdk Error Code </h1>
</div>

## 缩写说明
* JNT: joint
* PDL: pedstral
* TP: teach pendant
* COMM: communication
* ENC: encoder
* CURR: current
* POS: position
* PKG: package
* PROG: program
## 错误码
| 错误名称 | 错误码 | 说明 | 建议 |
| :----:| :----: | :----: | :----: |
|DEBUG|0|Debug message {}|suggest...|
|POPUP|1|Popup title: {}, msg: {}, mode: {}|suggest...|
|POPUP_DISMISS|2|{}|suggest...|
|SYSTEM_HALT|3|{}|suggest...|
|INV_ARGUMENTS|4|Invalid arguments.|suggest...|
|USER_NOTIFY|5|{}|suggest...|
|MODBUS_SIGNAL_CREATED|10|Modbus signal {} created.|suggest...|
|MODBUS_SIGNAL_REMOVED|11|Modbus signal {} removed.|suggest...|
|MODBUS_SIGNAL_VALUE_CHANGED|12|Modbus signal {} value changed to {}|suggest...|
|RUNTIME_CONTEXT|13|tid: {} lineno: {} index: {} comment: {}|suggest...|
|JOINT_ERR_OVER_CURRENET|10001|joint{} error: over current|(a) Check for short circuit. (b) Do a Complete rebooting sequence. (c) If this happens more than two times in a row, replace joint|
|JOINT_ERR_OVER_VOLTAGE|10002|joint{} error: over voltage|(a) Do a Complete rebooting sequence. (b) Check 48 V Power supply, current distributer, energy eater and Control Board for issues|
|JOINT_ERR_LOW_VOLTAGE|10003|joint{} error: low voltage|(a) Do a Complete rebooting sequence. (b) Check for short circuit in robot arm. (c) Check 48 V Power supply, current distributer, energy eater and Control Board for issues|
|JOINT_ERR_OVER_TEMP|10004|joint{} error: over temperature|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_HALL|10005|joint{} error: hall|suggest...|
|JOINT_ERR_ENCODER|10006|joint{} error: encoder|Check encoder connections|
|JOINT_ERR_ABS_ENCODER|10007|joint{} error: abs encoder|suggest...|
|JOINT_ERR_Q_CURRENT|10008|joint{} error: detect current|suggest...|
|JOINT_ERR_ENC_POLL|10009|joint{} error: encoder pollustion|suggest...|
|JOINT_ERR_ENC_Z_SIGNAL|10010|joint{} error: enocder z signal|suggest...|
|JOINT_ERR_ENC_CAL|10011|joint{} error: encoder calibrate|suggest...|
|JOINT_ERR_IMU_SENS|10012|joint{} error: IMU sensor|suggest...|
|JOINT_ERR_TEMP_SENS|10013|joint{} error: TEMP sensor|suggest...|
|JOINT_ERR_CAN_BUS|10014|joint{} error: CAN bus error|suggest...|
|JOINT_ERR_SYS_CUR|10015|joint{} error: system current error|suggest...|
|JOINT_ERR_SYS_POS|10016|joint{} error: system position error|suggest...|
|JOINT_ERR_OVER_SP|10017|joint{} error: over speed|suggest...|
|JOINT_ERR_OVER_ACC|10018|joint{} error: over accelerate|suggest...|
|JOINT_ERR_TRACE|10019|joint{} error: trace accuracy|suggest...|
|JOINT_ERR_TAG_POS_OVER|10020|joint{} error: target position out of range|suggest...|
|JOINT_ERR_TAG_SP_OVER|10021|joint{} error: target speed out of range|suggest...|
|JOINT_ERR_COLLISION|10022|joint{} error: collision|suggest...|
|JOINT_ERR_COMMON|10023|joint{} error: unkown error. Check communication with joint.|suggest...|
|JOINT_ERR_SWITCH_SERVO_MODE|10024|joint{} error: switch servo mode timeout.|suggest...|
|JOINT_ERR_MOTOR_STUCK|10025|joint{} error: motor stucked.|suggest...|
|JOINT_ERR_REDUCER_OVER_TEMP|10026|joint{} error: reducer over temperature|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_REDUCER_NTC|10027|joint{} error: reducer TEMP sensor failure|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_ABS_MULTITURN|10028|joint{} error: absolute encoder multiturn error|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_ADC_ZERO_OFFSET|10029|joint{} error: ADC zero offset failure|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_SHORT_CIRCUIT|10030|joint{} error: short circuit|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_PHASE_LOST|10031|joint{} error: motor phase lost|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_BRAKE|10032|joint{} error: brake failure|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_FIRMWARE_UPDATE|10033|joint{} error: firmware update failure|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_BATTERY_LOW|10034|joint{} error: battery low|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_PHASE_ALIGN|10035|joint{} error: phase align|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_CAN_HW_FAULT|10036|joint{} error: CAN bus hw fault|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_POS_DISCONTINUOUS|10037|joint{} error: target position discontinuous|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_POS_INIT|10038|joint{} error: position initiallization failure|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_TORQUE_SENSOR|10039|joint{} error: torqure sensor failure|(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence|
|JOINT_ERR_OFFLINE|10040|joint{} error: joint may be offline|(a) Check joint's hardware. (b) Check joint's id.|
|JOINT_ERR_BOOTLOADER|10041|joint{} error: joint at bootloader mode|Retry firmware update. |
|JOINT_ERR_SLAVE_OFFLINE|10042|slave joint{} error: slave joint may be offline|(a) Check slave joint's hardware. (b) Check slave joint's id.|
|JOINT_ERR_SLAVE_BOOTLOADER|10043|slave joint{} error: slave joint at bootloader mode|Retry firmware update. |
|TOOL_FLASH_VERIFY_FAILED|40001|Flash write verify failed|suggest...|
|TOOL_PROGRAM_CRC_FAILED|40002|Program flash checksum failed during bootloading|suggest...|
|TOOL_PROGRAM_CRC_FAILED2|40003|Program flash checksum failed at runtime|suggest...|
|TOOL_ID_UNDIFINED|40004|Tool ID is undefined|suggest...|
|TOOL_ILLEGAL_BL_CMD|40005|Illegal bootloader command|suggest...|
|TOOL_FW_WRONG|40006|Wrong firmware at the joint|suggest...|
|TOOL_HW_INVALID|40007|Invalid hardware revision|suggest...|
|TOOL_SHORT_CURCUIT_H|40011|Short circuit detected on Digital Output: {} high side|suggest...|
|TOOL_SHORT_CURCUIT_L|40012|Short circuit detected on Digital Output: {} low side|suggest...|
|TOOL_AVERAGE_CURR_HIGH|40013|10 second Average tool IO Current of {} A is outside of the allowed range.|suggest...|
|TOOL_POWER_PIN_OVER_CURR|40014|Current of {} A on the POWER pin is outside of the allowed range.|suggest...|
|TOOL_DOUT_PIN_OVER_CURR|40015|Current of {} A on the Digital Output pins is outside of the allowed range.|suggest...|
|TOOL_GROUND_PIN_OVER_CURR|40016|Current of {} A on the ground pin is outside of the allowed range.|suggest...|
|TOOL_RX_FRAMING|40021|RX framing error|suggest...|
|TOOL_RX_PARITY|40022|RX Parity error|suggest...|
|TOOL_48V_LOW|40031|48V input is too low|suggest...|
|TOOL_48V_HIGH|40032|48V input is too high|suggest...|
|TOOL_ERR_OFFLINE|40033|tool error: tool may be offline|(a) Check tool's hardware. (b) Check joint's id.|
|TOOL_ERR_BOOTLOADER|40034|tool error: tool at bootloader mode|Retry firmware update. |
|PKG_LOST|50001|Lost package from pedestal|suggest...|
|PEDSTRAL_OFFLINE|50002|pedestal error: pedestal may be offline|(a) Check pedestal's hardware. (b) Check pedestal's id.|
|PEDESTAL_ERR_BOOTLOADER|50003|pedestal error: pedestal at bootloader mode|Retry firmware update. |
|IFB_ERR_ROBOTTYPE|20001|Robot error type!|suggest...|
|IFB_ERR_ADXL_SENS|20002|Acceleration sensor error!|suggest...|
|IFB_ERR_EN_LINE|20003|Encoder line error!|suggest...|
|IFB_ERR_ENTER_HDG_MODE|20004|Robot enter handguide mode!|suggest...|
|IFB_ERR_EXIT_HDG_MODE|20005|Robot exit handguide mode!|suggest...|
|IFB_ERR_MAC_DATA_BREAK|20006|MAC data break!|suggest...|
|IFB_ERR_DRV_FIRMWARE_VERSION|20007|Motor driver firmware version error!|suggest...|
|INIT_ERR_EN_DRV|20008|Motor driver enable failed!|suggest...|
|INIT_ERR_EN_AUTO_BACK|20009|Motor driver enable auto back failed!|suggest...|
|INIT_ERR_EN_CUR_LOOP|20010|Motor driver enable current loop failed!|suggest...|
|INIT_ERR_SET_TAG_CUR|20011|Motor driver set target current failed!|suggest...|
|INIT_ERR_RELEASE_BRAKE|20012|Motor driver release brake failed!|suggest...|
|INIT_ERR_EN_POS_LOOP|20013|Motor driver enable postion loop failed!|suggest...|
|INIT_ERR_SET_MAX_ACC|20014|Motor set max accelerate failed!|suggest...|
|SAFETY_ERR_PROTECTION_STOP_TIMEOUT|20015|Protective stop timeout!|suggest...|
|SAFETY_ERR_REDUCED_MODE_TIMEOUT|20016|Reduced mode timeout!|suggest...|
|SYS_ERR_MCU_COM|20017|Robot system error: mcu communication error!|suggest...|
|SYS_ERR_RS485_COM|20018|Robot system error: RS485 communication error!|suggest...|
|IFB_ERR_DISCONNECTED|20019|Interface board may be disconnected. Please check connection between IPC and Interface board.|suggest...|
|IFB_ERR_PAYLOAD_ERROR|20020|Payload error.|suggest...|
|IFB_OFFLINE|20021|ifaceboard error: ifaceboard may be offline|(a) Check ifaceboard's hardware. (b) Check ifaceboard's id.|
|IFB_ERR_BOOTLOADER|20022|ifaceboard error: ifaceboard at bootloader mode|Retry firmware update. |
|IFB_SLAVE_OFFLINE|20023|interface slave board error: interface slave board may be offline|(a) Check interface slave board's hardware. (b) Check interface slave board's id.|
|IFB_SLAVE_ERR_BOOTLOADER|20024|interface slave board error: interface slave board at bootloader mode|Retry firmware update. |
|HW_SCB_SETUP_FAILED|60001|Setup of Interface Board failed|suggest...|
|HW_PKG_CNT_DISAGEE|60002|Packet counter disagreements|suggest...|
|HW_SCB_DISCONNECT|60003|Connection to Interface Board lost|suggest...|
|HW_SCB_PKG_LOST|60004|Package lost from Interface Board|suggest...|
|HW_SCB_CONN_INIT_FAILED|60005|Ethernet connection initialization with Interface Board failed|suggest...|
|HW_LOST_JOINT_PKG|60006|Lost package from joint  {}|suggest...|
|HW_LOST_TOOL_PKG|60007|Lost package from tool|suggest...|
|HW_JOINT_PKG_CNT_DISAGREE|60008|Packet counter disagreement in packet from joint {}|suggest...|
|HW_TOOL_PKG_CNT_DISAGREE|60009|Packet counter disagreement in packet from tool|suggest...|
|HW_JOINTS_FAULT|60011|{} joint entered the Fault State|suggest...|
|HW_JOINTS_VIOLATION|60012|{} joint entered the Violation State|suggest...|
|HW_TP_FAULT|60013|Teach Pendant entered the Fault State|suggest...|
|HW_TP_VIOLATION|60014|Teach Pendant entered the Violation State|suggest...|
|HW_JOINT_MV_TOO_FAR|60021|{} joint moved too far before robot entered RUNNING State|suggest...|
|HW_JOINT_STOP_NOT_FAST|60022|Joint Not stopping fast enough|suggest...|
|HW_JOINT_MV_LIMIT|60023|Joint moved more than allowable limit|suggest...|
|HW_FT_SENSOR_DATA_INVALID|60024|Force-Torque Sensor data invalid|suggest...|
|HW_NO_FT_SENSOR|60025|Force-Torque sensor is expected, but it cannot be detected|suggest...|
|HW_FT_SENSOR_NOT_CALIB|60026|Force-Torque sensor is detected but not calibrated|suggest...|
|HW_RELEASE_BRAKE_FAILED|60030|Robot was not able to brake release, see log for details|suggest...|
|HW_OVERCURR_SHUTDOWN|60040|Overcurrent shutdown|suggest...|
|HW_ENERGEY_SURPLUS|60050|Energy surplus shutdown|suggest...|
|HW_IDLE_POWER_HIGH|60060|Idle power consumption to high|suggest...|
|HW_ENTER_COLLISION_TIMEOUT|60071|Enter collision stop procedure timeout|suggest...|
|HW_POWERON_TIMEOUT|60072|Poweron robot timeout|suggest...|
|HW_NO_NIC_FOUND|60073|No network cards found.|suggest...|
|HW_IFB_NOT_FOUND|60074|No Interface Board found.|suggest...|
|HW_IFB_BOOTLOAD|60075|Interface Board in bootloader mode. Update firmware firstly.|suggest...|
|HW_TOOL_NOT_FOUND|60076|No Tool Board found.|suggest...|
|HW_BASE_NOT_FOUND|60077|No Base Board found.|suggest...|
|HW_BRINGUP_TIMEOUT|60078|Poweron robot timeout|suggest...|
|ROBOT_BE_PULLING|30001|Something is pulling the robot.|Please check TCP configuration,payload and mounting settings|
|PSTOP_ELBOW_POS|30002|Protective Stop: Elbow position close to safety plane limits.|Please move robot Elbow joint away from the safety plane|
|PSTOP_STOP_TIME|30003|Protective Stop: Exceeding user safety settings for stopping time.|(a) Check speeds and accelerations in the program (b) Check usage of TCP,payload and CoG correctly (c) Check external equipmentactivation if correctly set|
|PSTOP_STOP_DISTANCE|30004|Protective Stop: Exceeding user safety settings for stopping distance.|(a) Check speeds and accelerations in the program (b) Check usage of TCP,payload and CoG correctly (c) Check external equipmentactivation if correctly set|
|PSTOP_CLAMP|30005|Protective Stop: Danger of clamping between the Robot’s lower arm and tool.|(a) Check speeds and accelerations in the program (b) Check usage of TCP,payload and CoG correctly (c) Check external equipmentactivation if correctly set|
|PSTOP_POS_LIMIT|30006|Protective Stop: Position close to joint limits|suggest...|
|PSTOP_ORI_LIMIT|30007|Protective Stop: Tool orientation close to limits|suggest...|
|PSTOP_PLANE_LIMIT|30008|Protective Stop: Position close to safety plane limits|suggest...|
|PSTOP_POS_DEVIATE|30009|Protective Stop: Position deviates from path|Check payload, center of gravity and acceleration settings.|
|JOINT_CHK_PAYLOAD|30010|Joint {}: Check payload, center of gravity and acceleration settings. Log screen may contain additional information.|suggest...|
|PSTOP_SINGULARITY|30011|Protective Stop: Position in singularity.|Please use MoveJ or change the motion|
|PSTOP_CANNOT_MAINTAIN|30012|Protective Stop: Robot cannot maintain its position, check if payload is correct|suggest...|
|PSTOP_WRONG_PAYLOAD|30013|Protective Stop: Wrong payload or mounting detected, or something is pushing the robot when entering Freedrive mode|Verify that the TCP configuration and mounting in the used installation is correct|
|PSTOP_JOINT_COLLISION|30014|Protective Stop: Collision detected by joint {}|Make sure no objects are in the path of the robot and resume the program|
|PSTOP_POS_DISAGREE|30015|Protective stop: The robot was powered off last time due to a joint position disagreement.| (a) Verify that the robot position in the 3D graphics matches the real robot, to ensure that the encoders function before releasing the brakes. Stand back and monitor the robot performing its first program cycle as expected. (b) If the position is not correct, the robot must be repaired. In this case, click Power Off Robot. (c) If the position is correct, please tick the check box below the 3D graphics and click Robot Position Verified|
|TARGET_JOINT_SPEED_EXCEED|30016|Target joint speed exceed limits|suggest...|
|TARGET_POS_SUDDEN_CHG|30017|Sudden change in target position|suggest...|
|SUDDEN_STOP|30018|Sudden stop.| To abort a motion, use "stopj" or "stopl" script commands to generate a smooth deceleration before using "wait". Avoid aborting motions between waypoints with blend”|
|ROBOT_STOP_ABNORMAL|30019|Robot has not stopped in the allowed reaction and braking time|suggest...|
|PROG_INVALID_SETP|30020|Robot program resulted in invalid setpoint.|Please review waypoints in the program|
|BLEND_INVALID_SETP|30021|Blending failed and resulted in an invalid setpoint.|Try changing the blend radius or contact technical support|
|APPROACH_SINGULARITY|30022|Robot approaching singularity – Acceleration threshold failed.|Review waypoints in the program, try using MoveJ instead of MoveL in the position close to singularity|
|TSPEED_UNMATCH_POS|30023|Target speed does not match target position|suggest...|
|INCONSIS_TPOS_SPD|30024|Inconsistency between target position and speed|suggest...|
|JOINT_TSPD_UNMATCH_POS|30025|Target joint speed does not match target joint position change – Joint {}|suggest...|
|FIELDBUS_INPUT_DISCONN|30026|Fieldbus input disconnected.|Please check fieldbus connections (RTDE, ModBus, EtherNet/IP and Profinet) or disable the fieldbus in the installation. Check RTDE watchdog feature. Check if a URCap is using this feature.|
|OPMODE_CHANGED|30027|Operational mode changed: {}|suggest...|
|NO_KIN_CALIB|30028|No Kinematic Calibration found (calibration.conf file is either corrupt or missing).|A new kinematics calibration may be needed if the robot needs to improve its kinematics, otherwise, ignore this message)|
|KIN_CALIB_UNMATCH_JOINT|30029|Kinematic Calibration for the robot does not match the joint(s).|If moving a program from a different robot to this one, rekinematic calibrate the second robot to improve kinematics, otherwise ignore this message.|
|KIN_CALIB_UNMATCH_ROBOT|30030|Kinematic Calibration does not match the robot.|Please check if the serial number of the robot arm matches the Control Box|
|JOINT_OFFSET_CHANGED|30031|Large movement of the robot detected while it was powered off. The joints were moved while it was powered off, or the encoders do not function|suggest...|
|OFFSET_CHANGE_HIGH|30032|Change in offset is too high|suggest...|
|JOINT_SPEED_LIMIT|30033|Close to joint speed safety limit.|Review program speed and acceleration|
|TOOL_SPEED_LIMIT|30034|Close to tool speed safety limit.|Review program speed and acceleration|
|MOMENTUM_LIMIT|30035|Close to momentum safety limit.|Review program speed and acceleration|
|ROBOT_MV_STOP|30036|Robot is moving when in Stop Mode|suggest...|
|HAND_PROTECTION|30037|Hand protection: Tool is too close to the lower arm: {} meter.|(a) Check wrist position. (b) Verify mounting (c) Do a Complete rebooting sequence (d) Update software (e) Contact your local AUBO Robots service provider for assistance|
|WRONG_SAFETYMODE|30038|Wrong safety mode: {}|suggest...|
|SAFETYMODE_CHANGED|30039|Safety mode changed: {}|suggest...|
|JOINT_ACC_LIMIT|30040|Close to joint acceleration safety limit|suggest...|
|TOOL_ACC_LIMIT|30041|Close to tool acceleration safety limit|suggest...|
|JOINT_TEMPERATURE_LIMIT|30042|Joint {} temperature too high(>{}℃)|suggest...|
|CONTROL_BOX_TEMPERATURE_LIMIT|30043|Control box temperature too high(>{}℃)|suggest...|
|ROBOT_EMERGENCY_STOP|30044|Robot emergency stop|suggest...|
|ROBOTMODE_CHANGED|30045|Robot mode changed: {}|suggest...|
|ROBOTMODE_ERROR|30046|Wrong robot mode: {}|suggest...|
|POSE_OUT_OF_REACH|30047|Target pose [{}] out of reach|suggest...|
|TP_PLAN_FAILED|30048|Trajectory plan FAILED.|suggest...|
|START_FORCE_FAILED|30049|Start force control failed, because force sensor does not exist.|suggest...|
|OVER_SAFE_PLANE_LIMIT|30050|{} axis exceeds the safety plane limit (Move_type:{} id:{}).|Please move the robot to the safety plane range.|
|POWERON_FAIL_VIOLATION|30051|Failed to power on because the robot safety mode is in violation|suggest...|
|POWERON_FAIL_SYSTEMEMERGENCYSTOP|30052|Failed to power on because the robot safety mode is in system emergency stop|suggest...|
|POWERON_FAIL_ROBOTEMERGENCYSTOP|30053|Failed to power on because the robot safety mode is in robot emergency stop|Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion|
|POWERON_FAIL_FAULT|30054|Failed to power on because the robot safety mode is in fault|suggest...|
|STARTUP_FAIL_VIOLATION|30055|Failed to startup because the robot safety mode is in violation|suggest...|
|STARTUP_FAIL_SYSTEMEMERGENCYSTOP|30056|Failed to startup because the robot safety mode is in system emergency stop|suggest...|
|STARTUP_FAIL_ROBOTEMERGENCYSTOP|30057|Failed to startup because the robot safety mode is in robot emergency stop|Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion|
|STARTUP_FAIL_FAULT|30058|Failed to startup because the robot safety mode is in fault|suggest...|
|BACKDRIVE_FAIL_VIOLATION|30059|Failed to backdrive because the robot safety mode is in violation|suggest...|
|BACKDRIVE_FAIL_SYSTEMEMERGENCYSTOP|30060|Failed to backdrive because the robot safety mode is in system emergency stop|suggest...|
|BACKDRIVE_FAIL_ROBOTEMERGENCYSTOP|30061|Failed to backdrive because the robot safety mode is in robot emergency stop|Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion|
|BACKDRIVE_FAIL_FAULT|30062|Failed to backdrive because the robot safety mode is in fault|suggest...|
|SETSIM_FAIL_VIOLATION|30063|Switch sim mode failed because the robot safety mode is in violation|suggest...|
|SETSIM_FAIL_SYSTEMEMERGENCYSTOP|30064|Switch sim mode failed because the robot safety mode is in system emergency stop|suggest...|
|SETSIM_FAIL_ROBOTEMERGENCYSTOP|30065|Switch sim mode failed because the robot safety mode is in robot emergency stop|Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion|
|SETSIM_FAIL_FAULT|30066|Switch sim mode failed because the robot safety mode is in fault|suggest...|
|FREEDRIVE_FAIL_VIOLATION|30067|Enable handguide mode failed because the robot safety mode is in violation|suggest...|
|FREEDRIVE_FAIL_SYSTEMEMERGENCYSTOP|30068|Enable handguide mode failed because the robot safety mode is in system emergency stop|suggest...|
|FREEDRIVE_FAIL_ROBOTEMERGENCYSTOP|30069|Enable handguide mode failed because the robot safety mode is in robot emergency stop|Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion|
|FREEDRIVE_FAIL_FAULT|30070|Enable handguide mode failed because the robot safety mode is in fault|suggest...|
|UPFIRMWARE_FAIL_VIOLATION|30071|Firmware update failed because the robot safety mode is in violation|suggest...|
|UPFIRMWARE_FAIL_SYSTEMEMERGENCYSTOP|30072|Firmware update failed because the robot safety mode is in system emergency stop|suggest...|
|UPFIRMWARE_FAIL_ROBOTEMERGENCYSTOP|30073|Firmware update failed because the robot safety mode is in robot emergency stop|Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion|
|UPFIRMWARE_FAIL_FAULT|30074|Firmware update failed because the robot safety mode is in fault|suggest...|
|SETPERSOSTENT_FAIL_VIOLATION|30075|Set persistent parameter failed because the robot safety mode is in violation|suggest...|
|SETPERSOSTENT_FAIL_SYSTEMEMERGENCYSTOP|30076|Set persistent parameter failed because the robot safety mode is in system emergency stop|suggest...|
|SETPERSOSTENT_FAIL_ROBOTEMERGENCYSTOP|30077|Set persistent parameter failed because the robot safety mode is in robot emergency stop|Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion|
|SETPERSOSTENT_FAIL_FAULT|30078|Set persistent parameter failed because the robot safety mode is in fault|suggest...|
|SETPERSOSTENT_FAIL_PARAM_ERR|30079|Set persistent parameter failed|(a) Check the parameter format, whether all are floating point numbers|
|ROBOT_CABLE_DISCONN|30080|Robot cable not connected|(a) Make sure the cable between Control Box and Robot Arm is correctly connected and it has no damage. (b) Check for loose connections (c) Do a Complete rebooting sequence (d) Update software (e) Contact your local AUBO Robots service provider for assistance Contact your local AUBO Robots service provider for assistance.|
|TP_TOO_SHORT|30081|The generated trajectory is ignored because it is too short|(a) Please check if the added waypoints are coincident (b) If it is an arc movement, please check whether the three points are collinear|
|INV_KIN_FAIL|30082|Inverse kinematics solution failed. The target pose may be in a singular position or exceed the joint limits|(a) Change the target pose and try moving again|
|FREEDRIVE_ENABLED|30083|Freedrive status changed to {}|suggest...|
|TP_INV_FAIL_REFERENCE_JOINT_OUT_OF_LIMIT|30084|Inverse kinematics solution failed. Reference angle [{}] exceeds joint limit [{}].|suggest...|
|TP_INV_FAIL_NO_SOLUTION|30085|Inverse kinematics solution failed. The reference angle [{}] and the target angle [{}] are used as parameters. there is no solution in the calculation of the inverse solution process.|suggest...|
|SERVO_FAIL_VIOLATION|30086|Switch servo mode failed because the robot safety mode is in violation|suggest...|
|SERVO_FAIL_SYSTEMEMERGENCYSTOP|30087|Switch servo mode failed because the robot safety mode is in system emergency stop|suggest...|
|SERVO_FAIL_ROBOTEMERGENCYSTOP|30088|Switch servo mode failed because the robot safety mode is in robot emergency stop|Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion|
|SERVO_FAIL_FAULT|30089|Switch servo mode failed because the robot safety mode is in fault|suggest...|
|FREEDRIVE_FAIL_NO_RUNNING|30090|Enable handguide mode failed because the robot mode type is {}(not running)|suggest...|
|RUNTIME_MACHINE_ERROR|30091|The state of the running machine is {}, not {}. {} function execution failed because the state is wrong.|suggest...|
|RESUME_FAR_PAUSE_PT|30092|Cannot resume from joint position [{}].\nToo far away from paused point [{}].|suggest...|
|PAYLOAD_LIGHTER_ERROR|30093|The payload setting is too small!|suggest...|
|PAYLOAD_OVERLOAD_ERROR|30094|The payload setting is too large!|suggest...|
|PAUSE_FAIL_NOT_POSITION_PLAN_MODE|30095|This motion does not support the pause function. The motion is stopping.|suggest...|
|TP_PLAN_FAILED_CIRCULAR_WAYPOINTS_COINCIDE|30096|The planning failed because the three waypoints of the arc were determined to coincide.|Check the circular waypoints to make sure they are different.|
|SERVO_WRONG_SAFETYMODE|30097|Switch servo mode failed because the robot safety mode is in {}.|Check the circular waypoints to make sure they are different.|
|SET_PERSTPARAM_WRONG_SAFETYMODE|30098|Set persistent parameter failed because the robot safety mode is in {}|suggest...|
|SET_KINPARAM_WRONG_SAFETYMODE|30099|Set Kinematics Compensate parameters failed because the robot safety mode is in {}|suggest...|
|SET_ROBOT_ZERO_WRONG_SAFETYMODE|30100|Set current joint angles to zero failed because the robot safety mode is in {}|suggest...|
|UPFIRMWARE_WRONG_SAFETYMODE|30101|Firmware update failed because the robot safety mode is in {}|suggest...|
|POWERON_WRONG_SAFETYMODE|30102|Failed to power on because the robot safety mode is in {}|suggest...|
|STARTUP_WRONG_SAFETYMODE|30103|Failed to startup because the robot safety mode is in {}|suggest...|
|BACKDRIVE_WRONG_SAFETYMODE|30104|Failed to backdrive because the robot safety mode is in system emergency stop|suggest...|
|SETSIM_WRONG_SAFETYMODE|30105|Switch sim mode failed because the robot safety mode is in violation|suggest...|
|FREEDRIVE_WRONG_SAFETYMODE|30106|Enable handguide mode failed because the robot safety mode is in wrong safety mode: {}|suggest...|
|TP_PLAN_FAILED_JOINT_JUMP_BIGGER|30107|Inverse kinematics solution failed. The target point and the current point are in different robot configuration spaces.|Add a few more points between the target point and the current point.|
|RUN_PROGRAM_FAILED|30108|Run program {} failed.|suggset...|
|FREEDRIVE_FAIL_WRONG_RTMSTATE|30109|Unable to enter the HandGuide mode as the robot is not currently in a stopped or paused state.|suggset...|
|SAFEGUARDSTOP_CONFIGURABLE_INPUT|30110|Configurable safety input is triggered.|suggset...|
|SAFEGUARDSTOP_3PE|30111|3PE is triggered.|suggset...|
|SAFEGUARDSTOP_SI|30112|SI0/SI1 is triggered.|suggset...|
|ROBOT_TYPE_CHANGED|30200|Robot type changed to '{}', and robot subtype changed to '{}'|suggest...|
|LINKMODE_CHANGED|30201|Link mode changed to {}|suggest...|
|ROBOT_SELF_COLLISION|30301|Detect risk of robot self collision|suggest...|
|CONSTANT_INVALID|30302|Joint torque constants are invalid. HandGuide will be disabled, and the collision protection may be triggered by mistake.|suggest...|
|GRAVITY_INVALID|30303|Abnormal value of gravity acceleration sensor. HandGuide will be disabled, and the collision protection may be triggered by mistake.|suggest...|
|DYNAMICS_INVALID|30304|Robot dynamics parameters are invalid. HandGuide will be disabled, and the collision protection may be triggered by mistake.|suggest...|
|FRICTION_INVALID|30305|Joint friction parameters are invalid. HandGuide will be disabled, and the collision protection may be triggered by mistake.|suggest...|
|HANDGUIDE_UNDER_DEVELOP|30306|Robot type of {} function under development. HandGuide will be disabled, and the collision protection may be triggered by mistake.|suggest...|
|SLOW_DOWN_INFO|30307|Slow down level changed to {}({}%)|suggest...|
|WRONG_JOINT_SAFETY_LIMIT|30308|Joint designed ranges exceeds ranges read from hardware interface.|suggest...|
|FREEDRIVE_IN_SIMULATION|30309|Enable handguide mode failed because the robot is in simulation mode.|suggest...|
|ROBOT_STOPPING_TIMEOUT|30310|Robot stopping timeout.|suggest...|
|PSTOP_TCP_PLANE_VIOLATION|30311|Protective Stop: TCP position close to safety plane limits.|suggest...|
|PSTOP_ELBOW_PLANE_VIOLATION|30312|Protective Stop: elbow position close to safety plane limits.|suggest...|
|PSTOP_JOINT_TORQUE_VIOLATION|30313|Protective Stop: joint{} exceeds torque limit.|suggest...|
|PSTOP_JOINT_POSITION_VIOLATION|30314|Protective Stop: joint{} exceeds position limit.|suggest...|
|PSTOP_JOINT_SPEED_VIOLATION|30315|Protective Stop: joint{} exceeds speed limit.|suggest...|
|PSTOP_TCP_SPEED_VIOLATION|30316|Protective Stop: TCP speed close to safety limits.|suggest...|
|PSTOP_ELBOW_SPEED_VIOLATION|30317|Protective Stop: elbow speed close to safety limits.|suggest...|
|PSTOP_TCP_FORCE_VIOLATION|30318|Protective Stop: TCP foece close to safety limits.|suggest...|
|PSTOP_ELBOW_TORQUE_VIOLATION|30319|Protective Stop: elbow torque close to safety limits.|suggest...|
|PSTOP_POWER_VIOLATION|30320|Protective Stop: robot power close to safety limits.|suggest...|
|PSTOP_MOMENTUM_VIOLATION|30321|Protective Stop: robot momentum close to safety limits.|suggest...|
|PSTOP_TCP_CUBE_VIOLATION|30322|Protective Stop: TCP position close to safety cube.|suggest...|
|PSTOP_ELBOW_CUBE_VIOLATION|30323|Protective Stop: TCP position close to safety cube.|suggest...|
|REDUCE_ELBOW_PLANE_TRIGGER|30324|Reduce mode: elbow close to safety plane triggers reduction mode.|suggest...|
|REDUCE_TCP_PLANE_TRIGGER|30325|Reduce mode: TCP close to safety plane triggers reduction mode.|suggest...|
|PSTOP_INCORRECT_FORCE_OFFSET|30326|Protective Stop: Sudden change in force control target position. Force sensor offset may be incorrect or force sensor fault.|suggest...|
|ARCS_MAX_ERROR_CODE|-1|Max error code|suggest...|
