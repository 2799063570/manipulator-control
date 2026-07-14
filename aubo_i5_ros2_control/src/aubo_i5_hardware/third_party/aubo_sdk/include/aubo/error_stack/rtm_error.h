/** @file  rtm_error.h
 *  @brief 运行时错误码
 */
#ifndef AUBO_SDK_RTM_ERROR_H
#define AUBO_SDK_RTM_ERROR_H

// clang-format off

#define RTM_ERRORS \
    _D(ROBOT_BE_PULLING, 30001, "Something is pulling the robot.","Please check TCP configuration,payload and mounting settings") \
    _D(PSTOP_ELBOW_POS, 30002, "Protective Stop: Elbow position close to safety plane limits.","Please move robot Elbow joint away from the safety plane") \
    _D(PSTOP_STOP_TIME, 30003, "Protective Stop: Exceeding user safety settings for stopping time.","(a) Check speeds and accelerations in the program (b) Check usage of TCP,payload and CoG correctly (c) Check external equipmentactivation if correctly set") \
    _D(PSTOP_STOP_DISTANCE, 30004, "Protective Stop: Exceeding user safety settings for stopping distance.","(a) Check speeds and accelerations in the program (b) Check usage of TCP,payload and CoG correctly (c) Check external equipmentactivation if correctly set") \
    _D(PSTOP_CLAMP, 30005, "Protective Stop: Danger of clamping between the Robot’s lower arm and tool.","(a) Check speeds and accelerations in the program (b) Check usage of TCP,payload and CoG correctly (c) Check external equipmentactivation if correctly set") \
    _D(PSTOP_POS_LIMIT, 30006, "Protective Stop: Position close to joint limits", "suggest...") \
    _D(PSTOP_ORI_LIMIT, 30007, "Protective Stop: Tool orientation close to limits", "suggest...") \
    _D(PSTOP_PLANE_LIMIT, 30008, "Protective Stop: Position close to safety plane limits", "suggest...") \
    _D(PSTOP_POS_DEVIATE, 30009, "Protective Stop: Position deviates from path", "Check payload, center of gravity and acceleration settings.") \
    _D(JOINT_CHK_PAYLOAD, 30010, "Joint " _PH1_ ": Check payload, center of gravity and acceleration settings. Log screen may contain additional information.", "suggest...") \
    _D(PSTOP_SINGULARITY, 30011, "Protective Stop: Position in singularity.","Please use MoveJ or change the motion") \
    _D(PSTOP_CANNOT_MAINTAIN, 30012, "Protective Stop: Robot cannot maintain its position, check if payload is correct", "suggest...") \
    _D(PSTOP_WRONG_PAYLOAD, 30013, "Protective Stop: Wrong payload or mounting detected, or something is pushing the robot when entering Freedrive mode","Verify that the TCP configuration and mounting in the used installation is correct") \
    _D(PSTOP_JOINT_COLLISION, 30014, "Protective Stop: Collision detected by joint " _PH1_, "Make sure no objects are in the path of the robot and resume the program") \
    _D(PSTOP_POS_DISAGREE, 30015, "Protective stop: The robot was powered off last time due to a joint position disagreement."," (a) Verify that the robot position in the 3D graphics matches the real robot, to ensure that the encoders function before releasing the brakes. Stand back and monitor the robot performing its first program cycle as expected. (b) If the position is not correct, the robot must be repaired. In this case, click Power Off Robot. (c) If the position is correct, please tick the check box below the 3D graphics and click Robot Position Verified") \
    _D(TARGET_JOINT_SPEED_EXCEED, 30016, "Target joint speed exceed limits", "suggest...") \
    _D(TARGET_POS_SUDDEN_CHG, 30017, "Sudden change in target position", "suggest...") \
    _D(SUDDEN_STOP, 30018, "Sudden stop."," To abort a motion, use \"stopj\" or \"stopl\" script commands to generate a smooth deceleration before using \"wait\". Avoid aborting motions between waypoints with blend”") \
    _D(ROBOT_STOP_ABNORMAL, 30019, "Robot has not stopped in the allowed reaction and braking time", "suggest...") \
    _D(PROG_INVALID_SETP, 30020, "Robot program resulted in invalid setpoint.", "Please review waypoints in the program") \
    _D(BLEND_INVALID_SETP, 30021, "Blending failed and resulted in an invalid setpoint.", "Try changing the blend radius or contact technical support") \
    _D(APPROACH_SINGULARITY, 30022, "Robot approaching singularity – Acceleration threshold failed.","Review waypoints in the program, try using MoveJ instead of MoveL in the position close to singularity") \
    _D(TSPEED_UNMATCH_POS, 30023, "Target speed does not match target position", "suggest...") \
    _D(INCONSIS_TPOS_SPD, 30024, "Inconsistency between target position and speed", "suggest...") \
    _D(JOINT_TSPD_UNMATCH_POS, 30025, "Target joint speed does not match target joint position change – Joint " _PH1_ "", "suggest...") \
    _D(FIELDBUS_INPUT_DISCONN, 30026, "Fieldbus input disconnected.","Please check fieldbus connections (RTDE, ModBus, EtherNet/IP and Profinet) or disable the fieldbus in the installation. Check RTDE watchdog feature. Check if a URCap is using this feature.") \
    _D(OPMODE_CHANGED, 30027, "Operational mode changed: " _PH1_ "", "suggest...") \
    _D(NO_KIN_CALIB, 30028, "No Kinematic Calibration found (calibration.conf file is either corrupt or missing).","A new kinematics calibration may be needed if the robot needs to improve its kinematics, otherwise, ignore this message)") \
    _D(KIN_CALIB_UNMATCH_JOINT, 30029, "Kinematic Calibration for the robot does not match the joint(s).", "If moving a program from a different robot to this one, rekinematic calibrate the second robot to improve kinematics, otherwise ignore this message.") \
    _D(KIN_CALIB_UNMATCH_ROBOT, 30030, "Kinematic Calibration does not match the robot.","Please check if the serial number of the robot arm matches the Control Box") \
    _D(JOINT_OFFSET_CHANGED, 30031, "Large movement of the robot detected while it was powered off. The joints were moved while it was powered off, or the encoders do not function", "suggest...") \
    _D(OFFSET_CHANGE_HIGH, 30032, "Change in offset is too high", "suggest...") \
    _D(JOINT_SPEED_LIMIT, 30033, "Close to joint speed safety limit.", "Review program speed and acceleration") \
    _D(TOOL_SPEED_LIMIT, 30034, "Close to tool speed safety limit.", "Review program speed and acceleration") \
    _D(MOMENTUM_LIMIT, 30035, "Close to momentum safety limit.", "Review program speed and acceleration") \
    _D(ROBOT_MV_STOP, 30036, "Robot is moving when in Stop Mode", "suggest...") \
    _D(HAND_PROTECTION, 30037, "Hand protection: Tool is too close to the lower arm: " _PH1_ " meter.","(a) Check wrist position. (b) Verify mounting (c) Do a Complete rebooting sequence (d) Update software (e) Contact your local AUBO Robots service provider for assistance") \
    _D(WRONG_SAFETYMODE, 30038, "Wrong safety mode: " _PH1_, "suggest...") \
    _D(SAFETYMODE_CHANGED, 30039, "Safety mode changed: " _PH1_, "suggest...") \
    _D(JOINT_ACC_LIMIT, 30040, "Close to joint acceleration safety limit", "suggest...") \
    _D(TOOL_ACC_LIMIT, 30041, "Close to tool acceleration safety limit", "suggest...") \
    _D(JOINT_TEMPERATURE_LIMIT, 30042, "Joint " _PH1_ " temperature too high(>" _PH2_ "℃)", "suggest...") \
    _D(CONTROL_BOX_TEMPERATURE_LIMIT, 30043, "Control box temperature too high(>" _PH1_ "℃)", "suggest...") \
    _D(ROBOT_EMERGENCY_STOP, 30044, "Robot emergency stop", "suggest...") \
    _D(ROBOTMODE_CHANGED, 30045, "Robot mode changed: " _PH1_, "suggest...") \
    _D(ROBOTMODE_ERROR, 30046, "Wrong robot mode: " _PH1_, "suggest...") \
    _D(POSE_OUT_OF_REACH, 30047, "Target pose [" _PH1_ "] out of reach", "suggest...") \
    _D(TP_PLAN_FAILED, 30048, "Trajectory plan FAILED." , "suggest...") \
    _D(START_FORCE_FAILED, 30049, "Start force control failed, because force sensor does not exist." , "suggest...") \
    _D(OVER_SAFE_PLANE_LIMIT,30050, _PH1_ " axis exceeds the safety plane limit (Move_type:" _PH2_ " id:" _PH3_ ").","Please move the robot to the safety plane range.") \
    _D(POWERON_FAIL_VIOLATION,30051, "Failed to power on because the robot safety mode is in violation", "suggest...") \
    _D(POWERON_FAIL_SYSTEMEMERGENCYSTOP, 30052, "Failed to power on because the robot safety mode is in system emergency stop", "suggest...") \
    _D(POWERON_FAIL_ROBOTEMERGENCYSTOP, 30053, "Failed to power on because the robot safety mode is in robot emergency stop", "Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion") \
    _D(POWERON_FAIL_FAULT, 30054, "Failed to power on because the robot safety mode is in fault", "suggest...") \
    _D(STARTUP_FAIL_VIOLATION, 30055, "Failed to startup because the robot safety mode is in violation", "suggest...") \
    _D(STARTUP_FAIL_SYSTEMEMERGENCYSTOP, 30056, "Failed to startup because the robot safety mode is in system emergency stop", "suggest...") \
    _D(STARTUP_FAIL_ROBOTEMERGENCYSTOP, 30057, "Failed to startup because the robot safety mode is in robot emergency stop", "Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion") \
    _D(STARTUP_FAIL_FAULT, 30058, "Failed to startup because the robot safety mode is in fault", "suggest...") \
    _D(BACKDRIVE_FAIL_VIOLATION, 30059, "Failed to backdrive because the robot safety mode is in violation", "suggest...") \
    _D(BACKDRIVE_FAIL_SYSTEMEMERGENCYSTOP, 30060, "Failed to backdrive because the robot safety mode is in system emergency stop", "suggest...") \
    _D(BACKDRIVE_FAIL_ROBOTEMERGENCYSTOP, 30061, "Failed to backdrive because the robot safety mode is in robot emergency stop", "Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion") \
    _D(BACKDRIVE_FAIL_FAULT, 30062, "Failed to backdrive because the robot safety mode is in fault", "suggest...") \
    _D(SETSIM_FAIL_VIOLATION, 30063, "Switch sim mode failed because the robot safety mode is in violation", "suggest...") \
    _D(SETSIM_FAIL_SYSTEMEMERGENCYSTOP, 30064, "Switch sim mode failed because the robot safety mode is in system emergency stop", "suggest...") \
    _D(SETSIM_FAIL_ROBOTEMERGENCYSTOP, 30065, "Switch sim mode failed because the robot safety mode is in robot emergency stop", "Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion") \
    _D(SETSIM_FAIL_FAULT, 30066, "Switch sim mode failed because the robot safety mode is in fault", "suggest...") \
    _D(FREEDRIVE_FAIL_VIOLATION, 30067, "Enable handguide mode failed because the robot safety mode is in violation", "suggest...") \
    _D(FREEDRIVE_FAIL_SYSTEMEMERGENCYSTOP, 30068, "Enable handguide mode failed because the robot safety mode is in system emergency stop", "suggest...") \
    _D(FREEDRIVE_FAIL_ROBOTEMERGENCYSTOP, 30069, "Enable handguide mode failed because the robot safety mode is in robot emergency stop", "Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion") \
    _D(FREEDRIVE_FAIL_FAULT, 30070, "Enable handguide mode failed because the robot safety mode is in fault", "suggest...") \
    _D(UPFIRMWARE_FAIL_VIOLATION, 30071, "Firmware update failed because the robot safety mode is in violation", "suggest...") \
    _D(UPFIRMWARE_FAIL_SYSTEMEMERGENCYSTOP, 30072, "Firmware update failed because the robot safety mode is in system emergency stop", "suggest...") \
    _D(UPFIRMWARE_FAIL_ROBOTEMERGENCYSTOP, 30073, "Firmware update failed because the robot safety mode is in robot emergency stop", "Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion") \
    _D(UPFIRMWARE_FAIL_FAULT, 30074, "Firmware update failed because the robot safety mode is in fault", "suggest...") \
    _D(SETPERSOSTENT_FAIL_VIOLATION, 30075, "Set persistent parameter failed because the robot safety mode is in violation", "suggest...") \
    _D(SETPERSOSTENT_FAIL_SYSTEMEMERGENCYSTOP, 30076, "Set persistent parameter failed because the robot safety mode is in system emergency stop", "suggest...") \
    _D(SETPERSOSTENT_FAIL_ROBOTEMERGENCYSTOP, 30077, "Set persistent parameter failed because the robot safety mode is in robot emergency stop", "Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion") \
    _D(SETPERSOSTENT_FAIL_FAULT, 30078, "Set persistent parameter failed because the robot safety mode is in fault", "suggest...") \
    _D(SETPERSOSTENT_FAIL_PARAM_ERR, 30079, "Set persistent parameter failed", "(a) Check the parameter format, whether all are floating point numbers") \
    _D(ROBOT_CABLE_DISCONN, 30080, "Robot cable not connected", "(a) Make sure the cable between Control Box and Robot Arm is correctly connected and it has no damage. (b) Check for loose connections (c) Do a Complete rebooting sequence (d) Update software (e) Contact your local AUBO Robots service provider for assistance Contact your local AUBO Robots service provider for assistance.") \
    _D(TP_TOO_SHORT, 30081, "The generated trajectory is ignored because it is too short", "(a) Please check if the added waypoints are coincident (b) If it is an arc movement, please check whether the three points are collinear") \
    _D(INV_KIN_FAIL, 30082, "Inverse kinematics solution failed. The target pose may be in a singular position or exceed the joint limits", "(a) Change the target pose and try moving again") \
    _D(FREEDRIVE_ENABLED, 30083, "Freedrive status changed to " _PH1_ "", "suggest...") \
    _D(TP_INV_FAIL_REFERENCE_JOINT_OUT_OF_LIMIT, 30084, "Inverse kinematics solution failed. Reference angle [" _PH1_ "] exceeds joint limit [" _PH2_ "].", "suggest...") \
    _D(TP_INV_FAIL_NO_SOLUTION, 30085, "Inverse kinematics solution failed. The reference angle [" _PH1_ "] and the target angle [" _PH2_ "] are used as parameters. there is no solution in the calculation of the inverse solution process.", "suggest...")\
    _D(SERVO_FAIL_VIOLATION, 30086, "Switch servo mode failed because the robot safety mode is in violation", "suggest...") \
    _D(SERVO_FAIL_SYSTEMEMERGENCYSTOP, 30087, "Switch servo mode failed because the robot safety mode is in system emergency stop", "suggest...") \
    _D(SERVO_FAIL_ROBOTEMERGENCYSTOP, 30088, "Switch servo mode failed because the robot safety mode is in robot emergency stop", "Pop up the red emergency stop button on the teach pendant when the robot is in a safe range of motion") \
    _D(SERVO_FAIL_FAULT, 30089, "Switch servo mode failed because the robot safety mode is in fault", "suggest...") \
    _D(FREEDRIVE_FAIL_NO_RUNNING, 30090, "Enable handguide mode failed because the robot mode type is " _PH1_ "(not running)", "suggest...") \
    _D(RUNTIME_MACHINE_ERROR, 30091, "The state of the running machine is " _PH1_ ", not " _PH2_ ". " _PH3_ " function execution failed because the state is wrong." , "suggest...") \
    _D(RESUME_FAR_PAUSE_PT, 30092, "Cannot resume from joint position [" _PH1_ "].\\nToo far away from paused point [" _PH2_ "]." , "suggest...") \
    _D(PAYLOAD_LIGHTER_ERROR, 30093, "The payload setting is too small!" , "suggest...") \
    _D(PAYLOAD_OVERLOAD_ERROR, 30094, "The payload setting is too large!" , "suggest...") \
    _D(PAUSE_FAIL_NOT_POSITION_PLAN_MODE, 30095, "This motion does not support the pause function. The motion is stopping." , "suggest...") \
    _D(TP_PLAN_FAILED_CIRCULAR_WAYPOINTS_COINCIDE, 30096, "The planning failed because the three waypoints of the arc were determined to coincide." , "Check the circular waypoints to make sure they are different.") \
    _D(SERVO_WRONG_SAFETYMODE, 30097, "Switch servo mode failed because the robot safety mode is in " _PH1_ "." , "Check the circular waypoints to make sure they are different.") \
    _D(SET_PERSTPARAM_WRONG_SAFETYMODE, 30098, "Set persistent parameter failed because the robot safety mode is in " _PH1_ , "suggest...") \
    _D(SET_KINPARAM_WRONG_SAFETYMODE, 30099, "Set Kinematics Compensate parameters failed because the robot safety mode is in " _PH1_ , "suggest...") \
    _D(SET_ROBOT_ZERO_WRONG_SAFETYMODE, 30100, "Set current joint angles to zero failed because the robot safety mode is in " _PH1_ , "suggest...") \
    _D(UPFIRMWARE_WRONG_SAFETYMODE, 30101, "Firmware update failed because the robot safety mode is in " _PH1_, "suggest...") \
    _D(POWERON_WRONG_SAFETYMODE, 30102, "Failed to power on because the robot safety mode is in " _PH1_, "suggest...") \
    _D(STARTUP_WRONG_SAFETYMODE, 30103, "Failed to startup because the robot safety mode is in " _PH1_, "suggest...") \
    _D(BACKDRIVE_WRONG_SAFETYMODE, 30104, "Failed to backdrive because the robot safety mode is in system emergency stop", "suggest...") \
    _D(SETSIM_WRONG_SAFETYMODE, 30105, "Switch sim mode failed because the robot safety mode is in violation", "suggest...") \
    _D(FREEDRIVE_WRONG_SAFETYMODE, 30106, "Enable handguide mode failed because the robot safety mode is in wrong safety mode: " _PH1_, "suggest...") \
    _D(TP_PLAN_FAILED_JOINT_JUMP_BIGGER, 30107, "Inverse kinematics solution failed. The target point and the current point are in different robot configuration spaces.", "Add a few more points between the target point and the current point.") \
    _D(RUN_PROGRAM_FAILED, 30108, "Run program " _PH1_ " failed.", "suggset...") \
    _D(FREEDRIVE_FAIL_WRONG_RTMSTATE, 30109, "Unable to enter the HandGuide mode as the robot is not currently in a stopped or paused state.", "suggset...") \
    _D(SAFEGUARDSTOP_CONFIGURABLE_INPUT, 30110, "Configurable safety input is triggered.", "suggset...") \
    _D(SAFEGUARDSTOP_3PE, 30111, "3PE is triggered.", "suggset...") \
    _D(SAFEGUARDSTOP_SI, 30112, "SI0/SI1 is triggered.", "suggset...") \
    _D(ROBOT_TYPE_CHANGED, 30200, "Robot type changed to '" _PH1_ "', and robot subtype changed to '" _PH2_ "'", "suggest...") \
    _D(LINKMODE_CHANGED, 30201, "Link mode changed to " _PH1_ "", "suggest...") \
    _D(ROBOT_SELF_COLLISION, 30301, "Detect risk of robot self collision", "suggest...") \
    _D(CONSTANT_INVALID, 30302, "Joint torque constants are invalid. HandGuide will be disabled, and the collision protection may be triggered by mistake.", "suggest...") \
    _D(GRAVITY_INVALID, 30303, "Abnormal value of gravity acceleration sensor. HandGuide will be disabled, and the collision protection may be triggered by mistake.", "suggest...") \
    _D(DYNAMICS_INVALID, 30304, "Robot dynamics parameters are invalid. HandGuide will be disabled, and the collision protection may be triggered by mistake.", "suggest...") \
    _D(FRICTION_INVALID, 30305, "Joint friction parameters are invalid. HandGuide will be disabled, and the collision protection may be triggered by mistake.", "suggest...") \
    _D(HANDGUIDE_UNDER_DEVELOP, 30306, "Robot type of " _PH1_ " function under development. HandGuide will be disabled, and the collision protection may be triggered by mistake.", "suggest...") \
    _D(SLOW_DOWN_INFO, 30307, "Slow down level changed to " _PH1_ "(" _PH2_ "%)", "suggest...") \
    _D(WRONG_JOINT_SAFETY_LIMIT, 30308, "Joint designed ranges exceeds ranges read from hardware interface.", "suggest...") \
    _D(FREEDRIVE_IN_SIMULATION, 30309, "Enable handguide mode failed because the robot is in simulation mode.", "suggest...") \
    _D(ROBOT_STOPPING_TIMEOUT, 30310, "Robot stopping timeout.", "suggest...") \
    _D(PSTOP_TCP_PLANE_VIOLATION, 30311, "Protective Stop: TCP position close to safety plane limits.", "suggest...") \
    _D(PSTOP_ELBOW_PLANE_VIOLATION, 30312, "Protective Stop: elbow position close to safety plane limits.", "suggest...") \
    _D(PSTOP_JOINT_TORQUE_VIOLATION, 30313, "Protective Stop: joint" _PH1_ " exceeds torque limit.", "suggest...") \
    _D(PSTOP_JOINT_POSITION_VIOLATION, 30314, "Protective Stop: joint" _PH1_ " exceeds position limit.", "suggest...") \
    _D(PSTOP_JOINT_SPEED_VIOLATION, 30315, "Protective Stop: joint" _PH1_ " exceeds speed limit.", "suggest...") \
    _D(PSTOP_TCP_SPEED_VIOLATION, 30316, "Protective Stop: TCP speed close to safety limits.", "suggest...") \
    _D(PSTOP_ELBOW_SPEED_VIOLATION, 30317, "Protective Stop: elbow speed close to safety limits.", "suggest...") \
    _D(PSTOP_TCP_FORCE_VIOLATION, 30318, "Protective Stop: TCP foece close to safety limits.", "suggest...") \
    _D(PSTOP_ELBOW_TORQUE_VIOLATION, 30319, "Protective Stop: elbow torque close to safety limits.", "suggest...") \
    _D(PSTOP_POWER_VIOLATION, 30320, "Protective Stop: robot power close to safety limits.", "suggest...") \
    _D(PSTOP_MOMENTUM_VIOLATION, 30321, "Protective Stop: robot momentum close to safety limits.", "suggest...") \
    _D(PSTOP_TCP_CUBE_VIOLATION, 30322, "Protective Stop: TCP position close to safety cube.", "suggest...") \
    _D(PSTOP_ELBOW_CUBE_VIOLATION, 30323, "Protective Stop: TCP position close to safety cube.", "suggest...") \
    _D(REDUCE_ELBOW_PLANE_TRIGGER, 30324, "Reduce mode: elbow close to safety plane triggers reduction mode.", "suggest...") \
    _D(REDUCE_TCP_PLANE_TRIGGER, 30325, "Reduce mode: TCP close to safety plane triggers reduction mode.", "suggest...") \
    _D(PSTOP_INCORRECT_FORCE_OFFSET, 30326, "Protective Stop: Sudden change in force control target position. Force sensor offset may be incorrect or force sensor fault.", "suggest...")



// clang-format on

#endif // AUBO_SDK_RTM_ERROR_H
