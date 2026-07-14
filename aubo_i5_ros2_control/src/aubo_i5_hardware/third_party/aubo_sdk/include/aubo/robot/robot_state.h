/** @file  robot_state.h
 *  @brief 获取机器人状态接口，如关节速度、关节角度、固件/硬件版本
 */
#ifndef AUBO_SDK_ROBOT_STATE_INTERFACE_H
#define AUBO_SDK_ROBOT_STATE_INTERFACE_H

#include <vector>

#include <aubo/global_config.h>
#include <aubo/type_def.h>

namespace arcs {
namespace common_interface {

class ARCS_ABI_EXPORT RobotState
{
public:
    RobotState();
    virtual ~RobotState();

    /**
     * 获取机器人的模式状态
     *
     * @return 机器人的模式状态
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getRobotModeType(self: pyaubo_sdk.RobotState) ->
     * arcs::common_interface::RobotModeType
     *
     * @par Lua函数原型
     * getRobotModeType() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getRobotModeType","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"Running"}
     *
     */
    RobotModeType getRobotModeType();

    /**
     * 获取安全模式
     *
     * @return 安全模式
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getSafetyModeType(self: pyaubo_sdk.RobotState) ->
     * arcs::common_interface::SafetyModeType
     *
     * @par Lua函数原型
     * getSafetyModeType() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getSafetyModeType","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"Normal"}
     *
     */
    SafetyModeType getSafetyModeType();

    /**
     * 获取机器人通电状态
     *
     * @return 机器人通电返回 true; 反之返回 false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.isPowerOn","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":true}
     *
     */
    bool isPowerOn();

    /**
     * 机器人是否已经停止下来
     *
     * @return 停止返回true; 反之返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isSteady(self: pyaubo_sdk.RobotState) -> bool
     *
     * @par Lua函数原型
     * isSteady() -> boolean
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.isSteady","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":true}
     *
     */
    bool isSteady();

    /**
     * 机器人是否发生了碰撞
     *
     * @return 发生碰撞返回 true; 反之返回 false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.isCollisionOccurred","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":false}
     *
     */
    bool isCollisionOccurred();

    /**
     * 机器人是否已经在安全限制之内
     *
     * @return 在安全限制之内返回true; 反之返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isWithinSafetyLimits(self: pyaubo_sdk.RobotState) -> bool
     *
     * @par Lua函数原型
     * isWithinSafetyLimits() -> boolean
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.isWithinSafetyLimits","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":true}
     *
     */
    bool isWithinSafetyLimits();

    /**
     * 获取当前的TCP位姿，其 TCP 偏移可以通过 getActualTcpOffset 获取
     *
     * 位姿表示形式为(x,y,z,rx,ry,rz)。
     * 其中x、y、z是工具中心点（TCP）在基坐标系下的位置，单位是m。
     * rx、ry、rz是工具中心点（TCP）在基坐标系下的姿态，是ZYX欧拉角，单位是rad。
     *
     * @return TCP的位姿,形式为(x,y,z,rx,ry,rz)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpPose(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getTcpPose() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getTcpPose","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.41777839846910425,-0.13250000000000012,0.20928451364415995,3.1415792312578987,0.0,1.5707963267948963]}
     *
     */
    std::vector<double> getTcpPose();

    /**
     * 获取当前的 TCP 偏移，也就是 getTcpPose 返回的 pose 用到的 TCP 偏移
     *
     * @return 当前的 TCP 偏移
     */
    std::vector<double> getActualTcpOffset();

    /**
     * 获取下一个目标路点
     * 注意与 getTcpTargetPose 的区别，此处定义存在歧义，命名需要优化
     *
     * 位姿表示形式为(x,y,z,rx,ry,rz)。
     * 其中x、y、z是工具中心点（TCP）在基坐标系下的目标位置，单位是m。
     * rx、ry、rz是工具中心点（TCP）在基坐标系下的目标姿态，是ZYX欧拉角，单位是rad。
     *
     * @return 当前目标位姿,形式为(x,y,z,rx,ry,rz)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTargetTcpPose(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getTargetTcpPose() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getTargetTcpPose","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.4173932217619493,-0.13250000000000012,0.43296496133045825,3.141577313781914,0.0,1.5707963267948963]}
     *
     */
    std::vector<double> getTargetTcpPose();

    /**
     * 获取工具端的位姿（不带TCP偏移）
     *
     * 位姿表示形式为(x,y,z,rx,ry,rz)。
     * 其中x、y、z是法兰盘中心在基坐标系下的目标位置，单位是m。
     * rx、ry、rz是法兰盘中心在基坐标系下的目标姿态，是ZYX欧拉角，单位是rad。
     *
     * @return 工具端的位姿,形式为(x,y,z,rx,ry,rz)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolPose(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getToolPose() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getToolPose","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.41777820858878617,-0.13250000000000012,0.20928410288421018,3.141579231257899,0.0,1.5707963267948963]}
     *
     */
    std::vector<double> getToolPose();

    /**
     * 获取TCP速度
     *
     * @return TCP速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpSpeed(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getTcpSpeed() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getTcpSpeed","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getTcpSpeed();

    /**
     * 获取TCP的力/力矩
     *
     * @return TCP的力/力矩
     *
     * @par Python函数原型
     * getTcpForce(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getTcpForce() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getTcpForce","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getTcpForce();

    /**
     * 获取肘部的位置
     *
     * @return 肘部的位置
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getElbowPosistion(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getElbowPosistion() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getElbowPosistion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.07355755887512408,-0.1325,0.43200874126125227,-1.5707963267948968,0.433006344376404,0.0]}
     *
     */
    std::vector<double> getElbowPosistion();

    /**
     * 获取肘部速度
     *
     * @return 肘部速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getElbowVelocity(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getElbowVelocity() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getElbowVelocity","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getElbowVelocity();

    /**
     * 获取基座力/力矩
     *
     * @return 基座力/力矩
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getBaseForce(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getBaseForce() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getBaseForce","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getBaseForce();

    /**
     * 获取上一次发送的TCP目标位姿
     *
     * @return TCP目标位姿
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpTargetPose(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getTcpTargetPose() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getTcpTargetPose","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.41777829240862013,-0.13250000000000012,0.2092832117232601,3.1415812372223217,0.0,1.5707963267948963]}
     *
     */
    std::vector<double> getTcpTargetPose();

    /**
     * 获取TCP目标速度
     *
     * @return TCP目标速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpTargetSpeed(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getTcpTargetSpeed() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getTcpTargetSpeed","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getTcpTargetSpeed();

    /**
     * 获取TCP目标力/力矩
     *
     * @return TCP目标力/力矩
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpTargetForce(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getTcpTargetForce() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getTcpTargetForce","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getTcpTargetForce();

    /**
     * 获取机械臂关节标志
     *
     * @return 机械臂关节标志
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointState(self: pyaubo_sdk.RobotState) ->
     * List[arcs::common_interface::JointStateType]
     *
     * @par Lua函数原型
     * getJointState() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointState","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":["Running","Running","Running","Running","Running","Running"]}
     *
     */
    std::vector<JointStateType> getJointState();

    /**
     * 获取关节的伺服状态
     *
     * @return 关节的伺服状态
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointServoMode(self: pyaubo_sdk.RobotState) ->
     * List[arcs::common_interface::JointServoModeType]
     *
     * @par Lua函数原型
     * getJointServoMode() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointServoMode","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":["Position","Position","Position","Position","Position","Position"]}
     *
     */
    std::vector<JointServoModeType> getJointServoMode();

    /**
     * 获取机械臂关节角度
     *
     * @return 机械臂关节角度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointPositions(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointPositions() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointPositions","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,-0.26199241371495835,1.7418102574563423,0.4330197667082982,1.5707963267948966,0.0]}
     *
     */
    std::vector<double> getJointPositions();

    /**
     * 获取机械臂历史关节角度
     *
     * @param steps
     * @return
     */
    std::vector<double> getJointPositionsHistory(int steps);

    /**
     * 获取机械臂关节速度
     *
     * @return 机械臂关节速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointSpeeds(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointSpeeds() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointSpeeds","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getJointSpeeds();

    /**
     * 获取机械臂关节加速度
     *
     * @return 机械臂关节加速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointAccelerations(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointAccelerations() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointAccelerations","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getJointAccelerations();

    /**
     * 获取机械臂关节力矩
     *
     * @return 机械臂关节力矩
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointTorqueSensors(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointTorqueSensors() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointTorqueSensors","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,6275.367736816406,-7704.2816162109375,3586.9766235351563,503.0364990234375,1506.0882568359375]}
     *
     */
    std::vector<double> getJointTorqueSensors();

    /**
     * 获取机械臂关节接触力矩（外力距）
     *
     * @return 机械臂关节接触力矩
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointContactTorques(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointContactTorques() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointContactTorques","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0，0.0,0.0,0.0]}
     */
    std::vector<double> getJointContactTorques();

    /**
     * 获取底座力传感器读数
     *
     * @return 底座力传感器读数
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getBaseForceSensor(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getBaseForceSensor() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getBaseForceSensor","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getBaseForceSensor();

    /**
     * 获取TCP力传感器读数
     *
     * @return TCP力传感器读数
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpForceSensors(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getTcpForceSensors() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getTcpForceSensors","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getTcpForceSensors();

    /**
     * 获取机械臂关节电流
     *
     * @return 机械臂关节电流
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointCurrents(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointCurrents() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointCurrents","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,1.25885009765625,-1.5289306640625,0.71868896484375,0.1007080078125,0.3021240234375]}
     *
     */
    std::vector<double> getJointCurrents();

    /**
     * 获取机械臂关节电压
     *
     * @return 机械臂关节电压
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointVoltages(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointVoltages() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointVoltages","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[2.0,2.5,3.0,2.0,2.5,2.0]}
     *
     */
    std::vector<double> getJointVoltages();

    /**
     * 获取机械臂关节温度
     *
     * @return 机械臂关节温度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointTemperatures(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointTemperatures() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointTemperatures","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[38.0,38.0,38.0,39.0,38.0,39.0]}
     *
     */
    std::vector<double> getJointTemperatures();

    /**
     * 获取关节全球唯一ID
     *
     * @return 关节全球唯一ID
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointUniqueIds(self: pyaubo_sdk.RobotState) -> List[str]
     *
     * @par Lua函数原型
     * getJointUniqueIds() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointUniqueIds","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":["00800020ffffffff31405153","00800020ffffffff3e3f5153","00800020ffffffff414b5153","00800020ffffffff31065153","00800020ffffffff41535153","00800020ffffffff41545153"]}
     *
     */
    std::vector<std::string> getJointUniqueIds();

    /**
     * 获取关节固件版本
     *
     * @return 关节固件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointFirmwareVersions(self: pyaubo_sdk.RobotState) -> List[int]
     *
     * @par Lua函数原型
     * getJointFirmwareVersions() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointFirmwareVersions","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[1000010,1000010,1000010,1000010,1000010,1000010]}
     *
     */
    std::vector<int> getJointFirmwareVersions();

    /**
     * 获取关节硬件版本
     *
     * @return 关节硬件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointHardwareVersions(self: pyaubo_sdk.RobotState) -> List[int]
     *
     * @par Lua函数原型
     * getJointHardwareVersions() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointHardwareVersions","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[1000000,1000000,1004000,1004000,1004000,1004000]}
     *
     */
    std::vector<int> getJointHardwareVersions();

    /**
     * 获取MasterBoard全球唯一ID
     *
     * @return MasterBoard全球唯一ID
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getMasterBoardUniqueId(self: pyaubo_sdk.RobotState) -> str
     *
     * @par Lua函数原型
     * getMasterBoardUniqueId() -> string
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getMasterBoardUniqueId","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"001e0044510f343037323637"}
     *
     */
    std::string getMasterBoardUniqueId();

    /**
     * 获取MasterBoard固件版本
     *
     * @return MasterBoard固件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getMasterBoardFirmwareVersion(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getMasterBoardFirmwareVersion() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getMasterBoardFirmwareVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":1000004}
     *
     */
    int getMasterBoardFirmwareVersion();

    /**
     * 获取MasterBoard硬件版本
     *
     * @return MasterBoard硬件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getMasterBoardHardwareVersion(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getMasterBoardHardwareVersion() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getMasterBoardHardwareVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":1000000}
     *
     */
    int getMasterBoardHardwareVersion();

    /**
     * 获取SlaveBoard全球唯一ID
     *
     * @return SlaveBoard全球唯一ID
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getSlaveBoardUniqueId(self: pyaubo_sdk.RobotState) -> str
     *
     * @par Lua函数原型
     * getSlaveBoardUniqueId() -> string
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getSlaveBoardUniqueId","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"736572630080000000000000"}
     *
     */
    std::string getSlaveBoardUniqueId();

    /**
     * 获取SlaveBoard固件版本
     *
     * @return SlaveBoard固件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getSlaveBoardFirmwareVersion(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getSlaveBoardFirmwareVersion() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getSlaveBoardFirmwareVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int getSlaveBoardFirmwareVersion();

    /**
     * 获取SlaveBoard硬件版本
     *
     * @return SlaveBoard硬件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getSlaveBoardHardwareVersion(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getSlaveBoardHardwareVersion() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getSlaveBoardHardwareVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":6030098}
     *
     */
    int getSlaveBoardHardwareVersion();

    /**
     * 获取工具端全球唯一ID
     *
     * @return 工具端全球唯一ID
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolUniqueId(self: pyaubo_sdk.RobotState) -> str
     *
     * @par Lua函数原型
     * getToolUniqueId() -> string
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getToolUniqueId","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"397d4e5331541252314d3042"}
     *
     */
    std::string getToolUniqueId();

    /**
     * 获取工具端固件版本
     *
     * @return 工具端固件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolFirmwareVersion(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getToolFirmwareVersion() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getToolFirmwareVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":1001003}
     *
     */
    int getToolFirmwareVersion();

    /**
     * 获取工具端硬件版本
     *
     * @return 工具端硬件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolHardwareVersion(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getToolHardwareVersion() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getToolHardwareVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":1000000}
     *
     */
    int getToolHardwareVersion();

    /**
     * 获取末端通信模式
     *
     * @return 末端通信模式
     * 0: 表示无串口
     * 1: 表示只有串口
     * 2: 表示带力传感器和串口
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolCommMode(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getToolCommMode() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getToolCommMode","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":1}
     *
     */
    int getToolCommMode();

    /**
     * 获取底座全球唯一ID
     *
     * @return 底座全球唯一ID
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getPedestalUniqueId(self: pyaubo_sdk.RobotState) -> str
     *
     * @par Lua函数原型
     * getPedestalUniqueId() -> string
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getPedestalUniqueId","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"205257533543065248544339"}
     *
     */
    std::string getPedestalUniqueId();

    /**
     * 获取底座固件版本
     *
     * @return 底座固件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getPedestalFirmwareVersion(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getPedestalFirmwareVersion() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getPedestalFirmwareVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":1000004}
     *
     */
    int getPedestalFirmwareVersion();

    /**
     * 获取底座硬件版本
     *
     * @return 底座硬件版本
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getPedestalHardwareVersion(self: pyaubo_sdk.RobotState) -> int
     *
     * @par Lua函数原型
     * getPedestalHardwareVersion() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getPedestalHardwareVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":1007000}
     *
     */
    int getPedestalHardwareVersion();

    /**
     * 获取机械臂关节目标位置角度
     *
     * @return 机械臂关节目标位置角度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointTargetPositions(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointTargetPositions() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointTargetPositions","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,-0.2619944355631239,1.7418124015308052,0.4330219266665035,1.5707963267948966,0.0]}
     *
     */
    std::vector<double> getJointTargetPositions();

    /**
     * 获取机械臂关节目标速度
     *
     * @return 机械臂关节目标速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointTargetSpeeds(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointTargetSpeeds() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointTargetSpeeds","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.00024227101509399773,0.0016521760307419697,0.0026521060731088397,0.0,0.0]}
     *
     */
    std::vector<double> getJointTargetSpeeds();

    /**
     * 获取机械臂关节目标加速度
     *
     * @return 机械臂关节目标加速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointTargetAccelerations(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointTargetAccelerations() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointTargetAccelerations","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,-0.6737932929246071,-12.610253240108449,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getJointTargetAccelerations();

    /**
     * 获取机械臂关节目标力矩
     *
     * @return 机械臂关节目标力矩
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointTargetTorques(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointTargetTorques() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointTargetTorques","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getJointTargetTorques();

    /**
     * 获取机械臂关节目标电流
     *
     * @return 机械臂关节目标电流
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointTargetCurrents(self: pyaubo_sdk.RobotState) -> List[float]
     *
     * @par Lua函数原型
     * getJointTargetCurrents() -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getJointTargetCurrents","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[0.0,0.0,0.0,0.0,0.0,0.0]}
     *
     */
    std::vector<double> getJointTargetCurrents();

    /**
     * 获取控制柜温度
     *
     * @return 控制柜温度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getControlBoxTemperature(self: pyaubo_sdk.RobotState) -> float
     *
     * @par Lua函数原型
     * getControlBoxTemperature() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getControlBoxTemperature","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":25.0}
     *
     */
    double getControlBoxTemperature();

    /**
     * 获取控制柜湿度
     *
     * @return 控制柜湿度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getControlBoxHumidity(self: pyaubo_sdk.RobotState) -> float
     *
     * @par Lua函数原型
     * getControlBoxHumidity() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getControlBoxHumidity","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":20.0}
     *
     */
    double getControlBoxHumidity();

    /**
     * 获取母线电压
     *
     * @return 母线电压
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getMainVoltage(self: pyaubo_sdk.RobotState) -> float
     *
     * @par Lua函数原型
     * getMainVoltage() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getMainVoltage","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":52.75}
     *
     */
    double getMainVoltage();

    /**
     * 获取母线电流
     *
     * @return 母线电流
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getMainCurrent(self: pyaubo_sdk.RobotState) -> float
     *
     * @par Lua函数原型
     * getMainCurrent() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getMainCurrent","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0.3204345703125}
     *
     */
    double getMainCurrent();

    /**
     * 获取机器人电压
     *
     * @return 机器人电压
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getRobotVoltage(self: pyaubo_sdk.RobotState) -> float
     *
     * @par Lua函数原型
     * getRobotVoltage() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getRobotVoltage","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":52.75}
     *
     */
    double getRobotVoltage();

    /**
     * 获取机器人电流
     *
     * @return 机器人电流
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getRobotCurrent(self: pyaubo_sdk.RobotState) -> float
     *
     * @par Lua函数原型
     * getRobotCurrent() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotState.getRobotCurrent","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0.3204345703125}
     */
    double getRobotCurrent();

    /**
     * 获取机器人缓速等级
     *
     * @return
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int getSlowDownLevel();

protected:
    void *d_;
};
using RobotStatePtr = std::shared_ptr<RobotState>;

// clang-format off
#define RobotState_DECLARES                              \
    _FUNC(RobotState, 0, getRobotModeType)               \
    _FUNC(RobotState, 0, getSafetyModeType)              \
    _FUNC(RobotState, 0, isPowerOn)                      \
    _FUNC(RobotState, 0, isSteady)                       \
    _FUNC(RobotState, 0, isCollisionOccurred)            \
    _FUNC(RobotState, 0, isWithinSafetyLimits)           \
    _FUNC(RobotState, 0, getTcpPose)                     \
    _FUNC(RobotState, 0, getActualTcpOffset)             \
    _FUNC(RobotState, 0, getTargetTcpPose)               \
    _FUNC(RobotState, 0, getToolPose)                    \
    _FUNC(RobotState, 0, getTcpSpeed)                    \
    _FUNC(RobotState, 0, getTcpForce)                    \
    _FUNC(RobotState, 0, getElbowPosistion)              \
    _FUNC(RobotState, 0, getElbowVelocity)               \
    _FUNC(RobotState, 0, getBaseForce)                   \
    _FUNC(RobotState, 0, getTcpTargetPose)               \
    _FUNC(RobotState, 0, getTcpTargetSpeed)              \
    _FUNC(RobotState, 0, getTcpTargetForce)              \
    _FUNC(RobotState, 0, getJointState)                  \
    _FUNC(RobotState, 0, getJointServoMode)              \
    _FUNC(RobotState, 0, getJointPositions)              \
    _FUNC(RobotState, 1, getJointPositionsHistory, steps) \
    _FUNC(RobotState, 0, getJointSpeeds)                 \
    _FUNC(RobotState, 0, getJointAccelerations)          \
    _FUNC(RobotState, 0, getJointTorqueSensors)          \
    _FUNC(RobotState, 0, getJointContactTorques)         \
    _FUNC(RobotState, 0, getBaseForceSensor)             \
    _FUNC(RobotState, 0, getTcpForceSensors)             \
    _FUNC(RobotState, 0, getJointCurrents)               \
    _FUNC(RobotState, 0, getJointVoltages)               \
    _FUNC(RobotState, 0, getJointTemperatures)           \
    _FUNC(RobotState, 0, getJointUniqueIds)              \
    _FUNC(RobotState, 0, getJointFirmwareVersions)       \
    _FUNC(RobotState, 0, getJointHardwareVersions)       \
    _FUNC(RobotState, 0, getMasterBoardUniqueId)         \
    _FUNC(RobotState, 0, getMasterBoardFirmwareVersion)  \
    _FUNC(RobotState, 0, getMasterBoardHardwareVersion)  \
    _FUNC(RobotState, 0, getSlaveBoardUniqueId)          \
    _FUNC(RobotState, 0, getSlaveBoardFirmwareVersion)   \
    _FUNC(RobotState, 0, getSlaveBoardHardwareVersion)   \
    _FUNC(RobotState, 0, getToolUniqueId)                \
    _FUNC(RobotState, 0, getToolFirmwareVersion)         \
    _FUNC(RobotState, 0, getToolHardwareVersion)         \
    _FUNC(RobotState, 0, getToolCommMode)                \
    _FUNC(RobotState, 0, getPedestalUniqueId)            \
    _FUNC(RobotState, 0, getPedestalFirmwareVersion)     \
    _FUNC(RobotState, 0, getPedestalHardwareVersion)     \
    _FUNC(RobotState, 0, getJointTargetPositions)        \
    _FUNC(RobotState, 0, getJointTargetSpeeds)           \
    _FUNC(RobotState, 0, getJointTargetAccelerations)    \
    _FUNC(RobotState, 0, getJointTargetTorques)          \
    _FUNC(RobotState, 0, getJointTargetCurrents)         \
    _FUNC(RobotState, 0, getControlBoxTemperature)       \
    _FUNC(RobotState, 0, getControlBoxHumidity)          \
    _FUNC(RobotState, 0, getMainVoltage)                 \
    _FUNC(RobotState, 0, getMainCurrent)                 \
    _FUNC(RobotState, 0, getRobotVoltage)                \
    _FUNC(RobotState, 0, getRobotCurrent)                \
    _FUNC(RobotState, 0, getSlowDownLevel)
// clang-format on
} // namespace common_interface
} // namespace arcs
#endif // AUBO_SDK_ROBOT_STATE_INTERFACE_H
