/** @file  hal_error.h
 *  @brief 定义硬件抽象层的错误码
 */
#ifndef AUBO_SDK_HAL_ERROR_H
#define AUBO_SDK_HAL_ERROR_H

// 缩写说明
// JNT: joint
// PDL: pedstral
// TP: teach pendant
// COMM: communication
// ENC: encoder
// CURR: current
// POS: position
// PKG: package
// PROG: program

// clang-format off
#define JOINT_ERRORS \
    _D(JOINT_ERR_OVER_CURRENET,  10001, "joint" _PH1_ " error: over current", "(a) Check for short circuit. (b) Do a Complete rebooting sequence. (c) If this happens more than two times in a row, replace joint") \
    _D(JOINT_ERR_OVER_VOLTAGE,  10002, "joint" _PH1_ " error: over voltage", "(a) Do a Complete rebooting sequence. (b) Check 48 V Power supply, current distributer, energy eater and Control Board for issues") \
    _D(JOINT_ERR_LOW_VOLTAGE,  10003, "joint" _PH1_ " error: low voltage", "(a) Do a Complete rebooting sequence. (b) Check for short circuit in robot arm. (c) Check 48 V Power supply, current distributer, energy eater and Control Board for issues") \
    _D(JOINT_ERR_OVER_TEMP,  10004, "joint" _PH1_ " error: over temperature", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_HALL,  10005, "joint" _PH1_ " error: hall", "suggest...") \
    _D(JOINT_ERR_ENCODER,  10006, "joint" _PH1_ " error: encoder", "Check encoder connections") \
    _D(JOINT_ERR_ABS_ENCODER,  10007, "joint" _PH1_ " error: abs encoder", "suggest...") \
    _D(JOINT_ERR_Q_CURRENT,  10008, "joint" _PH1_ " error: detect current", "suggest...") \
    _D(JOINT_ERR_ENC_POLL,  10009, "joint" _PH1_ " error: encoder pollustion", "suggest...") \
    _D(JOINT_ERR_ENC_Z_SIGNAL,  10010, "joint" _PH1_ " error: enocder z signal", "suggest...") \
    _D(JOINT_ERR_ENC_CAL,  10011, "joint" _PH1_ " error: encoder calibrate", "suggest...") \
    _D(JOINT_ERR_IMU_SENS, 10012, "joint" _PH1_ " error: IMU sensor", "suggest...") \
    _D(JOINT_ERR_TEMP_SENS, 10013, "joint" _PH1_ " error: TEMP sensor", "suggest...") \
    _D(JOINT_ERR_CAN_BUS, 10014, "joint" _PH1_ " error: CAN bus error", "suggest...") \
    _D(JOINT_ERR_SYS_CUR, 10015, "joint" _PH1_ " error: system current error", "suggest...") \
    _D(JOINT_ERR_SYS_POS, 10016, "joint" _PH1_ " error: system position error","suggest...") \
    _D(JOINT_ERR_OVER_SP, 10017, "joint" _PH1_ " error: over speed","suggest...") \
    _D(JOINT_ERR_OVER_ACC, 10018, "joint" _PH1_ " error: over accelerate", "suggest...") \
    _D(JOINT_ERR_TRACE, 10019, "joint" _PH1_ " error: trace accuracy", "suggest...") \
    _D(JOINT_ERR_TAG_POS_OVER, 10020, "joint" _PH1_ " error: target position out of range", "suggest...") \
    _D(JOINT_ERR_TAG_SP_OVER, 10021, "joint" _PH1_ " error: target speed out of range", "suggest...") \
    _D(JOINT_ERR_COLLISION, 10022, "joint" _PH1_ " error: collision", "suggest...") \
    _D(JOINT_ERR_COMMON, 10023, "joint" _PH1_ " error: unkown error. Check communication with joint.", "suggest...") \
    _D(JOINT_ERR_SWITCH_SERVO_MODE, 10024, "joint" _PH1_ " error: switch servo mode timeout.", "suggest...") \
    _D(JOINT_ERR_MOTOR_STUCK, 10025, "joint" _PH1_ " error: motor stucked.", "suggest...") \
    _D(JOINT_ERR_REDUCER_OVER_TEMP,  10026, "joint" _PH1_ " error: reducer over temperature", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_REDUCER_NTC,  10027, "joint" _PH1_ " error: reducer TEMP sensor failure", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_ABS_MULTITURN,  10028, "joint" _PH1_ " error: absolute encoder multiturn error", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_ADC_ZERO_OFFSET,  10029, "joint" _PH1_ " error: ADC zero offset failure", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_SHORT_CIRCUIT,  10030, "joint" _PH1_ " error: short circuit", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_PHASE_LOST,  10031, "joint" _PH1_ " error: motor phase lost", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_BRAKE,  10032, "joint" _PH1_ " error: brake failure", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_FIRMWARE_UPDATE,  10033, "joint" _PH1_ " error: firmware update failure", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_BATTERY_LOW,  10034, "joint" _PH1_ " error: battery low", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_PHASE_ALIGN,  10035, "joint" _PH1_ " error: phase align", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_CAN_HW_FAULT,  10036, "joint" _PH1_ " error: CAN bus hw fault", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_POS_DISCONTINUOUS,  10037, "joint" _PH1_ " error: target position discontinuous", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_POS_INIT,  10038, "joint" _PH1_ " error: position initiallization failure", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_TORQUE_SENSOR,  10039, "joint" _PH1_ " error: torqure sensor failure", "(a) Check robot’s environment and make sure the robot is operating within recommended limits. (b) Do a Complete rebooting sequence") \
    _D(JOINT_ERR_OFFLINE,  10040, "joint" _PH1_ " error: joint may be offline", "(a) Check joint's hardware. (b) Check joint's id.") \
    _D(JOINT_ERR_BOOTLOADER,  10041, "joint" _PH1_ " error: joint at bootloader mode", "Retry firmware update. ") \
    _D(JOINT_ERR_SLAVE_OFFLINE,  10042, "slave joint" _PH1_ " error: slave joint may be offline", "(a) Check slave joint's hardware. (b) Check slave joint's id.") \
    _D(JOINT_ERR_SLAVE_BOOTLOADER,  10043, "slave joint" _PH1_ " error: slave joint at bootloader mode", "Retry firmware update. ")

#define TOOL_ERRORS \
    _D(TOOL_FLASH_VERIFY_FAILED, 40001, "Flash write verify failed", "suggest...") \
    _D(TOOL_PROGRAM_CRC_FAILED, 40002, "Program flash checksum failed during bootloading", "suggest...") \
    _D(TOOL_PROGRAM_CRC_FAILED2, 40003, "Program flash checksum failed at runtime", "suggest...") \
    _D(TOOL_ID_UNDIFINED, 40004, "Tool ID is undefined", "suggest...") \
    _D(TOOL_ILLEGAL_BL_CMD, 40005, "Illegal bootloader command", "suggest...") \
    _D(TOOL_FW_WRONG, 40006, "Wrong firmware at the joint", "suggest...") \
    _D(TOOL_HW_INVALID, 40007, "Invalid hardware revision", "suggest...") \
    _D(TOOL_SHORT_CURCUIT_H, 40011, "Short circuit detected on Digital Output: " _PH1_ " high side", "suggest...") \
    _D(TOOL_SHORT_CURCUIT_L, 40012, "Short circuit detected on Digital Output: " _PH1_ " low side", "suggest...") \
    _D(TOOL_AVERAGE_CURR_HIGH, 40013, "10 second Average tool IO Current of " _PH1_ " A is outside of the allowed range.", "suggest...") \
    _D(TOOL_POWER_PIN_OVER_CURR, 40014, "Current of " _PH1_ " A on the POWER pin is outside of the allowed range.", "suggest...") \
    _D(TOOL_DOUT_PIN_OVER_CURR, 40015, "Current of " _PH1_ " A on the Digital Output pins is outside of the allowed range.", "suggest...") \
    _D(TOOL_GROUND_PIN_OVER_CURR, 40016, "Current of " _PH1_ " A on the ground pin is outside of the allowed range.", "suggest...") \
    _D(TOOL_RX_FRAMING, 40021, "RX framing error", "suggest...") \
    _D(TOOL_RX_PARITY, 40022, "RX Parity error", "suggest...") \
    _D(TOOL_48V_LOW, 40031, "48V input is too low", "suggest...") \
    _D(TOOL_48V_HIGH, 40032, "48V input is too high", "suggest...") \
    _D(TOOL_ERR_OFFLINE,  40033, "tool error: tool may be offline", "(a) Check tool's hardware. (b) Check joint's id.") \
    _D(TOOL_ERR_BOOTLOADER,  40034, "tool error: tool at bootloader mode", "Retry firmware update. ")


#define PEDSTRAL_ERRORS \
    _D(PKG_LOST, 50001, "Lost package from pedestal", "suggest...") \
    _D(PEDSTRAL_OFFLINE,  50002, "pedestal error: pedestal may be offline", "(a) Check pedestal's hardware. (b) Check pedestal's id.") \
    _D(PEDESTAL_ERR_BOOTLOADER,  50003, "pedestal error: pedestal at bootloader mode", "Retry firmware update. ")


#define SAFETY_INTERFACE_BOARD_ERRORS \
    _D(IFB_ERR_ROBOTTYPE, 20001, "Robot error type!", "suggest...") \
    _D(IFB_ERR_ADXL_SENS, 20002, "Acceleration sensor error!", "suggest...") \
    _D(IFB_ERR_EN_LINE, 20003, "Encoder line error!", "suggest...") \
    _D(IFB_ERR_ENTER_HDG_MODE, 20004, "Robot enter handguide mode!", "suggest...") \
    _D(IFB_ERR_EXIT_HDG_MODE, 20005, "Robot exit handguide mode!", "suggest...") \
    _D(IFB_ERR_MAC_DATA_BREAK, 20006, "MAC data break!", "suggest...") \
    _D(IFB_ERR_DRV_FIRMWARE_VERSION, 20007, "Motor driver firmware version error!", "suggest...") \
    _D(INIT_ERR_EN_DRV, 20008, "Motor driver enable failed!", "suggest...") \
    _D(INIT_ERR_EN_AUTO_BACK, 20009, "Motor driver enable auto back failed!", "suggest...") \
    _D(INIT_ERR_EN_CUR_LOOP, 20010, "Motor driver enable current loop failed!", "suggest...") \
    _D(INIT_ERR_SET_TAG_CUR, 20011, "Motor driver set target current failed!", "suggest...") \
    _D(INIT_ERR_RELEASE_BRAKE, 20012, "Motor driver release brake failed!", "suggest...") \
    _D(INIT_ERR_EN_POS_LOOP, 20013, "Motor driver enable postion loop failed!", "suggest...") \
    _D(INIT_ERR_SET_MAX_ACC, 20014, "Motor set max accelerate failed!", "suggest...") \
    _D(SAFETY_ERR_PROTECTION_STOP_TIMEOUT, 20015, "Protective stop timeout!", "suggest...") \
    _D(SAFETY_ERR_REDUCED_MODE_TIMEOUT, 20016, "Reduced mode timeout!", "suggest...") \
    _D(SYS_ERR_MCU_COM, 20017, "Robot system error: mcu communication error!", "suggest...") \
    _D(SYS_ERR_RS485_COM, 20018, "Robot system error: RS485 communication error!", "suggest...") \
    _D(IFB_ERR_DISCONNECTED, 20019, "Interface board may be disconnected. Please check connection between IPC and Interface board.", "suggest...")\
    _D(IFB_ERR_PAYLOAD_ERROR, 20020, "Payload error.", "suggest...") \
    _D(IFB_OFFLINE,  20021, "ifaceboard error: ifaceboard may be offline", "(a) Check ifaceboard's hardware. (b) Check ifaceboard's id.") \
    _D(IFB_ERR_BOOTLOADER,  20022, "ifaceboard error: ifaceboard at bootloader mode", "Retry firmware update. ") \
    _D(IFB_SLAVE_OFFLINE,  20023, "interface slave board error: interface slave board may be offline", "(a) Check interface slave board's hardware. (b) Check interface slave board's id.") \
    _D(IFB_SLAVE_ERR_BOOTLOADER,  20024, "interface slave board error: interface slave board at bootloader mode", "Retry firmware update. ")


#define HARDWARE_INTERFACE_ERRORS \
    _D(HW_SCB_SETUP_FAILED, 60001, "Setup of Interface Board failed", "suggest...") \
    _D(HW_PKG_CNT_DISAGEE, 60002, "Packet counter disagreements", "suggest...") \
    _D(HW_SCB_DISCONNECT, 60003, "Connection to Interface Board lost", "suggest...") \
    _D(HW_SCB_PKG_LOST, 60004, "Package lost from Interface Board", "suggest...") \
    _D(HW_SCB_CONN_INIT_FAILED, 60005, "Ethernet connection initialization with Interface Board failed", "suggest...") \
    _D(HW_LOST_JOINT_PKG, 60006, "Lost package from joint  " _PH1_ "", "suggest...") \
    _D(HW_LOST_TOOL_PKG, 60007, "Lost package from tool", "suggest...") \
    _D(HW_JOINT_PKG_CNT_DISAGREE, 60008, "Packet counter disagreement in packet from joint " _PH1_ "", "suggest...") \
    _D(HW_TOOL_PKG_CNT_DISAGREE, 60009, "Packet counter disagreement in packet from tool", "suggest...") \
    _D(HW_JOINTS_FAULT, 60011, "" _PH1_ " joint entered the Fault State", "suggest...") \
    _D(HW_JOINTS_VIOLATION, 60012, "" _PH1_ " joint entered the Violation State", "suggest...") \
    _D(HW_TP_FAULT, 60013, "Teach Pendant entered the Fault State", "suggest...") \
    _D(HW_TP_VIOLATION, 60014, "Teach Pendant entered the Violation State", "suggest...") \
    _D(HW_JOINT_MV_TOO_FAR, 60021, "" _PH1_ " joint moved too far before robot entered RUNNING State", "suggest...") \
    _D(HW_JOINT_STOP_NOT_FAST, 60022, "Joint Not stopping fast enough", "suggest...") \
    _D(HW_JOINT_MV_LIMIT, 60023, "Joint moved more than allowable limit", "suggest...") \
    _D(HW_FT_SENSOR_DATA_INVALID, 60024, "Force-Torque Sensor data invalid", "suggest...") \
    _D(HW_NO_FT_SENSOR, 60025, "Force-Torque sensor is expected, but it cannot be detected", "suggest...") \
    _D(HW_FT_SENSOR_NOT_CALIB, 60026, "Force-Torque sensor is detected but not calibrated", "suggest...") \
    _D(HW_RELEASE_BRAKE_FAILED, 60030, "Robot was not able to brake release, see log for details", "suggest...") \
    _D(HW_OVERCURR_SHUTDOWN, 60040, "Overcurrent shutdown", "suggest...") \
    _D(HW_ENERGEY_SURPLUS, 60050, "Energy surplus shutdown", "suggest...") \
    _D(HW_IDLE_POWER_HIGH, 60060, "Idle power consumption to high", "suggest...") \
    _D(HW_ENTER_COLLISION_TIMEOUT, 60071, "Enter collision stop procedure timeout", "suggest...") \
    _D(HW_POWERON_TIMEOUT, 60072, "Poweron robot timeout", "suggest...") \
    _D(HW_NO_NIC_FOUND, 60073, "No network cards found.", "suggest...") \
    _D(HW_IFB_NOT_FOUND, 60074, "No Interface Board found.", "suggest...") \
    _D(HW_IFB_BOOTLOAD, 60075, "Interface Board in bootloader mode. Update firmware firstly.", "suggest...") \
    _D(HW_TOOL_NOT_FOUND, 60076, "No Tool Board found.", "suggest...") \
    _D(HW_BASE_NOT_FOUND, 60077, "No Base Board found.", "suggest...") \
    _D(HW_BRINGUP_TIMEOUT, 60078, "Poweron robot timeout", "suggest...")

// clang-format on

// 定义硬件抽象层的错误代码
#define HAL_ERRORS                \
    JOINT_ERRORS                  \
    TOOL_ERRORS                   \
    PEDSTRAL_ERRORS               \
    SAFETY_INTERFACE_BOARD_ERRORS \
    HARDWARE_INTERFACE_ERRORS

#endif // AUBO_SDK_JOINT_ERROR_H
