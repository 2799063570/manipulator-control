#ifndef AUBO_SCOPE_REALTIME_ROBOT_STATE_H
#define AUBO_SCOPE_REALTIME_ROBOT_STATE_H

#include <QObject>
#include <mutex>

#include <aubo_sdk/rpc.h>
#include <aubo_sdk/rtde.h>

namespace arcs {
namespace aubo_sdk {
using namespace common_interface;

struct RobotInfo
{
    std::string robot_type_;
    std::string robot_subtype_;
    std::string cb_type_;
    int dof_{ 6 };

    Payload actual_payload_{ 0., std::vector<double>(3, 0.),
                             std::vector<double>(3, 0.),
                             std::vector<double>(9, 0.) };

    std::vector<double> actual_q_{ std::vector<double>(6, 0.) };
    std::vector<double> actual_current_{ std::vector<double>(6, 0.) };
    std::vector<double> actual_TCP_pose_{ std::vector<double>(6, 0.) };
    std::vector<double> actual_TCP_speed_{ std::vector<double>(6, 0.) };
    std::vector<double> actual_tool_pose_{ std::vector<double>(6, 0.) };

    std::vector<double> joint_voltages_{ std::vector<double>(6, 0.) };
    std::vector<double> joint_temperatures_{ std::vector<double>(6, 0.) };
    std::vector<JointStateType> joint_mode_{ std::vector<JointStateType>(
        6, JointStateType::Idle) };
    RobotModeType robot_mode_{ RobotModeType::NoController };
    SafetyModeType safety_mode_{ SafetyModeType::Normal };
    OperationalModeType operational_mode_{ OperationalModeType::Disabled };
    bool link_mode_{ false };
    bool freedrive_enabled_{ false };

    double actual_main_voltage_{ 0 };
    double actual_robot_voltage_{ 0 };
    double actual_robot_current_{ 0 };
    double cb_temperature_{ 0 };
    double cb_humidity_{ 0 };
    int collision_level_{ 0 };
    int slow_down_level_{ 0 };

    std::string tool_uuid_;
    std::string mb_uuid_;
    std::string sb_uuid_;
    std::string pedestal_uuid_;
    std::vector<std::string> joint_uuids_;

    std::vector<int> joint_fw_;
    std::vector<int> joint_hw_;
    int tool_fw_;
    int tool_hw_;
    int m_ifb_fw_;
    int m_ifb_hw_;
    int s_ifb_fw_;
    int s_ifb_hw_;
    int pedestal_fw_;
    int pedestal_hw_;

    int standardDigitalInputNum_{ 0 };
    int toolDigitalInputNum_{ 0 };
    int configurableDigitalInputNum_{ 0 };

    int standardDigitalOutputNum_{ 0 };
    int toolDigitalOutputNum_{ 0 };
    int configurableDigitalOutputNum_{ 0 };

    int standardAnalogInputNum_{ 0 };
    int toolAnalogInputNum_{ 0 };

    int standardAnalogOutputNum_{ 0 };
    int toolAnalogOutputNum_{ 0 };
    int staticLinkInputNum_{ 0 };
    int staticLinkOutputNum_{ 0 };

    std::unordered_map<std::string, std::vector<double>> dh_;
    std::unordered_map<std::string, std::vector<double>> threory_dh_;

    std::vector<bool> standard_digital_input_data_{ std::vector<bool>(64,
                                                                      false) };
    std::vector<bool> tool_digital_input_data_{ std::vector<bool>(64, false) };
    std::vector<bool> configurable_digital_input_data_{ std::vector<bool>(
        64, false) };
    std::vector<bool> link_digital_input_data_{ std::vector<bool>(64, false) };
    std::vector<bool> standard_digital_output_data_{ std::vector<bool>(64,
                                                                       false) };
    std::vector<bool> tool_digital_output_data_{ std::vector<bool>(64, false) };
    std::vector<bool> configurable_digital_output_data_{ std::vector<bool>(
        64, false) };
    std::vector<bool> link_digital_output_data_{ std::vector<bool>(64, false) };

    std::vector<double> standard_analog_input_data_;
    std::vector<double> tool_analog_input_data_;
    std::vector<double> standard_analog_output_data_;
    std::vector<double> tool_analog_output_data_;

    std::vector<double> joint_max_positions_{ std::vector<double>(6, 0.) };
    std::vector<double> joint_min_positions_{ std::vector<double>(6, 0.) };
    std::vector<double> joint_max_speeds_{ std::vector<double>(6, 0.) };
    std::vector<double> joint_max_accelerations_{ std::vector<double>(6, 0.) };
    std::vector<double> tcp_max_speeds_{ std::vector<double>(6, 0.) };
    std::vector<double> tcp_max_accelerations_{ std::vector<double>(6, 0.) };
    std::vector<double> gravity_{ std::vector<double>{ 0, 0, -9.81 } };

    bool simulation_enabled_{ false };
    double speed_fraction_{ 0 };
};

class ARCS_ABI RealtimeRobotState : public QObject
{
    Q_OBJECT
    friend class RobotProxy;

public:
    RealtimeRobotState(RpcClientPtr client, RtdeClientPtr rtde);

    ~RealtimeRobotState();

    int updateRobotInformationOnLogin();
    int updateRobotInformationOnPoweron(int robot_index = -1);
    int updateHDParamOnRobotTypeChanged(int robot_index = -1);

    std::vector<std::string> getRobotNames() const;

    int getLineNumber();
    /// 弃用，只能返回主线程号
    ARCS_DEPRECATED int getThreadID();

    RuntimeState getRuntimeState();

    int getDof();

    bool isSimulationEnabled();
    double getSpeedFraction();

    std::string getRobotType() const;
    std::string getRobotSubType() const;
    std::string getControlBoxType() const;

    OperationalModeType getOperationalMode() const;
    bool isLinkModeEnabled() const;
    bool isFreedriveEnabled() const;
    int getSlowDownLevel() const;

    int updateOperationMode(int robot_index);
    int updateIsLinkModeEnabled(int robot_index);

    // 获取机器人的模式状态
    RobotModeType getRobotModeType();

    // 获取安全模式
    SafetyModeType getSafetyModeType();

    // 获取TCP的位姿
    std::vector<double> getTcpPose();

    // 获取工具端的位姿（不带TCP偏移）
    std::vector<double> getToolPose();

    // 获取TCP速度
    std::vector<double> getTcpSpeed();

    // 获取TCP的力/力矩
    std::vector<double> getTcpForce();

    // 获取肘部的位置
    std::vector<double> getElbowPosistion();

    // 获取肘部速度
    std::vector<double> getElbowVelocity();

    // 获取基座力/力矩
    std::vector<double> getBaseForce();

    // 获取TCP目标位姿
    std::vector<double> getTcpTargetPose();

    // 获取TCP目标速度
    std::vector<double> getTcpTargetSpeed();

    // 获取TCP目标力/力矩
    std::vector<double> getTcpTargetForce();

    // 获取机械臂关节标志接口
    std::vector<JointStateType> getJointState();

    // 获取关节的伺服状态
    std::vector<JointServoModeType> getJointServoMode();

    // 获取实际负载
    Payload getPayload();

    // 获取机械臂关节角度接口
    std::vector<double> getJointPositions();

    // 获取机械臂关节速度接口
    std::vector<double> getJointSpeeds();

    // 获取机械臂关节加速度接口
    std::vector<double> getJointAccelerations();

    // 获取机械臂关节力矩接口
    std::vector<double> getJointTorqueSensors();

    // 获取底座力传感器读数
    std::vector<double> getBaseForceSensor();

    // 获取TCP力传感器读数
    std::vector<double> getTcpForceSensors();

    // 获取机械臂关节电流接口
    std::vector<double> getJointCurrents();

    // 获取机械臂关节电压接口
    std::vector<double> getJointVoltages();

    // 获取机械臂关节温度接口
    std::vector<double> getJointTemperatures();

    // 获取关节固件版本
    std::vector<int> getJointFirmwareVersions();

    // 获取关节硬件版本
    std::vector<int> getJointHardwareVersions();

    std::string getToolUniqueId();
    std::string getMasterBoardUniqueId();
    std::string getSlaveBoardUniqueId();
    std::string getPedestalUniqueId();
    std::vector<std::string> getJointUniqueIds();

    // 获取 MasterBoard 固件版本
    int getMasterBoardFirmwareVersion();

    // 获取 MasterBoard 硬件版本
    int getMasterBoardHardwareVersion();

    // 获取 SlaveBoard 固件版本
    int getSlaveBoardFirmwareVersion();

    // 获取 SlaveBoard 硬件版本
    int getSlaveBoardHardwareVersion();

    // 获取工具端固件版本
    int getToolFirmwareVersion();

    // 获取工具端硬件版本
    int getToolHardwareVersion();

    // 获取 Pedestal 固件版本
    int getPedestalFirmwareVersion();

    // 获取 Pedestal 硬件版本
    int getPedestalHardwareVersion();

    // 获取机械臂关节目标位置角度接口
    std::vector<double> getJointTargetPositions();

    // 获取机械臂关节目标速度
    std::vector<double> getJointTargetSpeeds();

    // 获取机械臂关节目标加速度
    std::vector<double> getJointTargetAccelerations();

    // 获取机械臂关节目标力矩
    std::vector<double> getJointTargetTorques();

    // 获取机械臂关节目标电流
    std::vector<double> getJointTargetCurrents();

    // 获取关节最大位置（物理极限）
    std::vector<double> getJointMaxPositions();

    // 获取关节最小位置（物理极限）
    std::vector<double> getJointMinPositions();

    // 获取关节最大速度（物理极限）
    std::vector<double> getJointMaxSpeeds();

    // 获取关节最大加速度（物理极限）
    std::vector<double> getJointMaxAccelerations();

    // 获取TCP最大速度（物理极限）
    std::vector<double> getTcpMaxSpeeds();

    // 获取TCP最大加速度（物理极限）
    std::vector<double> getTcpMaxAccelerations();

    // 获取控制柜温度
    double getControlBoxTemperature();

    // 获取控制柜湿度
    double getControlBoxHumidity();

    // 获取母线电压
    double getMainVoltage();

    // 获取母线电流
    double getMainCurrent();

    int getCollisionLevel();

    // 获取机器人电压
    double getRobotVoltage();

    // 获取机器人电流
    double getRobotCurrent();

    int getStandardDigitalInputNum();
    int getToolDigitalInputNum();
    int getConfigurableDigitalInputNum();

    int getStandardDigitalOutputNum();
    int getToolDigitalOutputNum();
    int getConfigurableDigitalOutputNum();

    int getStandardAnalogInputNum();
    int getToolAnalogInputNum();

    int getStandardAnalogOutputNum();
    int getToolAnalogOutputNum();

    SafetyInputAction getConfigurableInputAction(int index);
    SafetyOutputRunState getConfigurableOutputRunstate(int index);

    int setStandardDigitalInputAction(int index, StandardInputAction action);
    int setToolDigitalInputAction(int index, StandardInputAction action);
    int setConfigurableDigitalInputAction(int index,
                                          StandardInputAction action);

    StandardInputAction getStandardDigitalInputAction(int index);
    StandardInputAction getToolDigitalInputAction(int index);
    StandardInputAction getConfigurableDigitalInputAction(int index);

    int setStandardDigitalOutputRunstate(int index,
                                         StandardOutputRunState runstate);
    int setToolDigitalOutputRunstate(int index,
                                     StandardOutputRunState runstate);
    int setConfigurableDigitalOutputRunstate(int index,
                                             StandardOutputRunState runstate);
    StandardOutputRunState getStandardDigitalOutputRunstate(int index);
    StandardOutputRunState getToolDigitalOutputRunstate(int index);
    StandardOutputRunState getConfigurableDigitalOutputRunstate(int index);

    int setStandardAnalogOutputRunstate(int index,
                                        StandardOutputRunState runstate);
    int setToolAnalogOutputRunstate(int index, StandardOutputRunState runstate);

    StandardOutputRunState getStandardAnalogOutputRunstate(int index);
    StandardOutputRunState getToolAnalogOutputRunstate(int index);

    int setStandardAnalogInputDomain(int index, int domain);
    int setToolAnalogInputDomain(int index, int domain);

    int getStandardAnalogInputDomain(int index);
    int getToolAnalogInputDomain(int index);

    int setStandardAnalogOutputDomain(int index, int domain);
    int setToolAnalogOutputDomain(int index, int domain);

    int getStandardAnalogOutputDomain(int index);
    int getToolAnalogOutputDomain(int index);

    int setStandardDigitalOutput(int index, bool value);
    int setToolDigitalOutput(int index, bool value);
    int setConfigurableDigitalOutput(int index, bool value);

    int setStandardAnalogOutput(int index, double value);
    int setToolAnalogOutput(int index, double value);

    bool getStandardDigitalInput(int index);
    bool getToolDigitalInput(int index);
    bool getConfigurableDigitalInput(int index);

    bool getStandardDigitalOutput(int index);
    bool getToolDigitalOutput(int index);
    bool getConfigurableDigitalOutput(int index);

    double getStandardAnalogInput(int index);
    double getToolAnalogInput(int index);

    double getStandardAnalogOutput(int index);
    double getToolAnalogOutput(int index);

    int getStaticLinkInputNum();
    int getStaticLinkOutputNum();
    bool getStaticLinkInput(int index);
    bool getStaticLinkOutput(int index);

    int configSubscribe();
    int selectRobot(int index);

    void reset();

    void updateModbusSignalNames();
    std::vector<std::string> getModbusSignalNames() const;
    std::vector<int> getModbusSignalValues() const;
    std::vector<int> getModbusSignalErrors() const;
    std::vector<double> getGravity();
    std::unordered_map<std::string, std::vector<double>> getRealDHParam();
    std::unordered_map<std::string, std::vector<double>> getTheoryDHParam();

    int updateGravity(int robot_index);

signals:
    void interfaceBoardVersionInfoUpdated(int robot_index);
    void jointVersionInfoUpdated(int robot_index);
    void runtimeStateChanged(arcs::common_interface::RuntimeState state);
    void safetyModeChanged(int robot_index,
                           arcs::common_interface::SafetyModeType mode);
    void robotModeChanged(int robot_index,
                          arcs::common_interface::RobotModeType mode);

private:
    RpcClientPtr rpc_client_{ nullptr };
    RtdeClientPtr rtde_client_{ nullptr };
    mutable std::mutex rtde_mtx_;

    std::vector<std::string> names_;
    std::string name_;
    int robot_index_{ -1 };

    int tid_{ -1 };
    int line_{ -1 };
    RuntimeState runtime_state_{ RuntimeState::Stopped };

    std::vector<int> modbus_signals_;
    std::vector<int> modbus_signals_errors_;
    std::vector<std::string> modbus_names_;

    RobotInfo info_[4];
};

using RealtimeRobotStatePtr = std::shared_ptr<RealtimeRobotState>;

} // namespace aubo_sdk
} // namespace arcs
#endif
