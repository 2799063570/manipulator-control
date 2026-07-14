/** @file  robot_config.h
 *  @brief 获取机器人配置接口，如获取DH参数、碰撞等级、安装位姿等等
 */
#ifndef AUBO_SDK_ROBOT_CONFIG_H
#define AUBO_SDK_ROBOT_CONFIG_H

#include <vector>
#include <unordered_map>

#include <aubo/global_config.h>
#include <aubo/type_def.h>

namespace arcs {
namespace common_interface {

class ARCS_ABI_EXPORT RobotConfig
{
public:
    RobotConfig();
    virtual ~RobotConfig();

    /**
     * 获取机器人的名字
     *
     * @return 返回机器人的名字
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getName(self: pyaubo_sdk.RobotConfig) -> str
     *
     * @par Lua函数原型
     * getName() -> string
     *
     */
    std::string getName();

    /**
     * 获取机器人的自由度(从硬件抽象层读取)
     *
     * @return 返回机器人的自由度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getDof(self: pyaubo_sdk.RobotConfig) -> int
     *
     * @par Lua函数原型
     * getDof() -> number
     *
     */
    int getDof();

    /**
     * 获取机器人的伺服控制周期(从硬件抽象层读取)
     *
     * @return 机器人的伺服控制周期
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getCycletime(self: pyaubo_sdk.RobotConfig) -> float
     *
     * @par Lua函数原型
     * getCycletime() -> number
     *
     */
    double getCycletime();

    /**
     * 预设缓速模式下的速度缩减比例
     *
     * @param level 缓速等级 1, 2
     * @param fraction
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setSlowDownFraction(int level, double fraction);

    /**
     * 获取预设的缓速模式下的速度缩减比例
     *
     * @param level 缓速等级 1, 2
     * @return 返回预设的缓速模式下的速度缩减比例
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    double getSlowDownFraction(int level);

    /**
     * 获取默认的工具端加速度，单位m/s^2
     *
     * @return 默认的工具端加速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getDefaultToolAcc(self: pyaubo_sdk.RobotConfig) -> float
     *
     * @par Lua函数原型
     * getDefaultToolAcc() -> number
     *
     */
    double getDefaultToolAcc();

    /**
     * 获取默认的工具端速度，单位m/s
     *
     * @return 默认的工具端速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getDefaultToolSpeed(self: pyaubo_sdk.RobotConfig) -> float
     *
     * @par Lua函数原型
     * getDefaultToolSpeed() -> number
     *
     */
    double getDefaultToolSpeed();

    /**
     * 获取默认的关节加速度，单位rad/s^2
     *
     * @return 默认的关节加速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getDefaultJointAcc(self: pyaubo_sdk.RobotConfig) -> float
     *
     * @par Lua函数原型
     * getDefaultJointAcc() -> number
     *
     */
    double getDefaultJointAcc();

    /**
     * 获取默认的关节速度，单位rad/s
     *
     * @return 默认的关节速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getDefaultJointSpeed(self: pyaubo_sdk.RobotConfig) -> float
     *
     * @par Lua函数原型
     * getDefaultJointSpeed() -> number
     *
     */
    double getDefaultJointSpeed();

    /**
     * 获取机器人类型代码
     *
     * @return 机器人类型代码
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getRobotType(self: pyaubo_sdk.RobotConfig) -> str
     *
     * @par Lua函数原型
     * getRobotType() -> string
     *
     */
    std::string getRobotType();

    /**
     * 获取机器人子类型代码
     *
     * @return 机器人子类型代码
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getRobotSubType(self: pyaubo_sdk.RobotConfig) -> str
     *
     * @par Lua函数原型
     * getRobotSubType() -> string
     *
     */
    std::string getRobotSubType();

    /**
     * 获取控制柜类型代码
     *
     * @return 控制柜类型代码
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getControlBoxType(self: pyaubo_sdk.RobotConfig) -> str
     *
     * @par Lua函数原型
     * getControlBoxType() -> string
     *
     */
    std::string getControlBoxType();

    /**
     * 设置安装位姿(机器人的基坐标系相对于世界坐标系)  world->base
     *
     * 一般在多机器人系统中使用，默认为 [0,0,0,0,0,0]
     *
     * @param pose 安装位姿
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setMountingPose(self: pyaubo_sdk.RobotConfig, arg0: List[float]) -> int
     *
     * @par Lua函数原型
     * setMountingPose(pose: table) -> nil
     *
     */
    int setMountingPose(const std::vector<double> &pose);

    /**
     * 获取安装位姿(机器人的基坐标系相对于世界坐标系)
     *
     * @return 安装位姿
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getMountingPose(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getMountingPose() -> table
     *
     */
    std::vector<double> getMountingPose();

    /**
     * 设置碰撞灵敏度等级
     * 数值越大越灵敏
     *
     * @param level 碰撞灵敏度等级
     * 0: 关闭碰撞检测功能
     * 1~9: 碰撞灵敏等级
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCollisionLevel(self: pyaubo_sdk.RobotConfig, arg0: int) -> int
     *
     * @par Lua函数原型
     * setCollisionLevel(level: number) -> nil
     *
     */
    int setCollisionLevel(int level);

    /**
     * 获取碰撞灵敏度等级
     *
     * @return 碰撞灵敏度等级
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getCollisionLevel(self: pyaubo_sdk.RobotConfig) -> int
     *
     * @par Lua函数原型
     * getCollisionLevel() -> number
     *
     */
    int getCollisionLevel();

    /**
     * 设置碰撞停止类型
     *
     * @param type 类型 \n
     * 0: 碰撞之后进入拖动模式 \n
     * 1: 碰撞停机
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCollisionStopType(self: pyaubo_sdk.RobotConfig, arg0: int) -> int
     *
     * @par Lua函数原型
     * setCollisionStopType(type: number) -> nil
     *
     */
    int setCollisionStopType(int type);

    /**
     * 获取碰撞停止类型
     *
     * @return 返回碰撞停止类型
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getCollisionStopType(self: pyaubo_sdk.RobotConfig) -> int
     *
     * @par Lua函数原型
     * getCollisionStopType() -> number
     *
     */
    int getCollisionStopType();

    /**
     * 设置机器人的 Home 位置
     *
     * @param positions 关节角度
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setHomePosition(const std::vector<double> &positions);

    /**
     * 获取机器人 Home 位置
     *
     * @return
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    std::vector<double> getHomePosition();

    /**
     * 设置拖动阻尼
     *
     * @param damp 阻尼
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setFreedriveDamp(self: pyaubo_sdk.RobotConfig, arg0: List[float]) -> int
     *
     * @par Lua函数原型
     * setFreedriveDamp(damp: table) -> number
     *
     */
    int setFreedriveDamp(const std::vector<double> &damp);

    /**
     * 获取拖动阻尼
     *
     * @return 拖动阻尼
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getFreedriveDamp(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getFreedriveDamp() -> table
     *
     */
    std::vector<double> getFreedriveDamp();

    /**
     * 获取机器人DH参数
     * alpha a d theta beta
     *
     * @param real 读取真实参数(理论值+补偿值)或者理论参数
     * @return 返回机器人DH参数
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getKinematicsParam(self: pyaubo_sdk.RobotConfig, arg0: bool) -> Dict[str,
     * List[float]]
     *
     * @par Lua函数原型
     * getKinematicsParam(real: boolean) -> table
     *
     */
    std::unordered_map<std::string, std::vector<double>> getKinematicsParam(
        bool real);

    /**
     * 获取指定温度下的DH参数补偿值:alpha a d theta beta
     *
     * @param ref_temperature 参考温度 ℃，默认20℃
     * @return 返回DH参数补偿值
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getKinematicsCompensate(self: pyaubo_sdk.RobotConfig, arg0: float) ->
     * Dict[str, List[float]]
     *
     * @par Lua函数原型
     * getKinematicsCompensate(ref_temperature: number) -> table
     *
     */
    std::unordered_map<std::string, std::vector<double>>
    getKinematicsCompensate(double ref_temperature);

    /**
     * 设置标准 DH 补偿到机器人
     *
     * @param param
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setKinematicsCompensate(
        const std::unordered_map<std::string, std::vector<double>> &param);

    /**
     * 设置需要保存到接口板底座的参数
     *
     * @param param 补偿数据
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setPersistentParameters(self: pyaubo_sdk.RobotConfig, arg0: str) -> int
     *
     * @par Lua函数原型
     * setPersistentParameters(param: string) -> nil
     *
     */
    int setPersistentParameters(const std::string &param);

    /**
     * 设置硬件抽象层自定义参数
     * 目的是为了做不同硬件之间的兼容
     *
     * @param param 自定义参数
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setHardwareCustomParameters(const std::string &param);

    /**
     * 获取硬件抽象层自定义参数
     *
     * @param
     * @return 返回硬件抽象层自定义的参数
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    std::string getHardwareCustomParameters(const std::string &param);

    /**
     * 设置机器人关节零位
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setRobotZero(self: pyaubo_sdk.RobotConfig) -> int
     *
     * @par Lua函数原型
     * setRobotZero() -> nil
     *
     */
    int setRobotZero();

    /**
     * 获取可用的末端力矩传感器的名字
     *
     * @return 返回可用的末端力矩传感器的名字
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpForceSensorNames(self: pyaubo_sdk.RobotConfig) -> List[str]
     *
     * @par Lua函数原型
     * getTcpForceSensorNames() -> table
     *
     */
    std::vector<std::string> getTcpForceSensorNames();

    /**
     * 设置末端力矩传感器
     * 如果存在内置的末端力矩传感器，默认将使用内置的力矩传感器
     *
     * @param name 末端力矩传感器的名字
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * selectTcpForceSensor(self: pyaubo_sdk.RobotConfig, arg0: str) -> int
     *
     * @par Lua函数原型
     * selectTcpForceSensor(name: string) -> nil
     *
     */
    int selectTcpForceSensor(const std::string &name);

    /**
     * 设置传感器安装位姿
     *
     * @param sensor_pose 传感器安装位姿
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setTcpForceSensorPose(const std::vector<double> &sensor_pose);

    /**
     * 获取传感器安装位姿
     *
     * @return 传感器安装位姿
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    std::vector<double> getTcpForceSensorPose();
    /**
     * 是否安装了末端力矩传感器
     *
     * @return 安装返回true; 没有安装返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * hasTcpForceSensor(self: pyaubo_sdk.RobotConfig) -> bool
     *
     * @par Lua函数原型
     * hasTcpForceSensor() -> boolean
     *
     */
    bool hasTcpForceSensor();

    /**
     * 设置末端力矩偏移
     *
     * @param force_offset 末端力矩偏移
     * @return 成功返回0; 失败返回错误码
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setTcpForceOffset(self: pyaubo_sdk.RobotConfig, arg0: List[float]) -> int
     *
     * @par Lua函数原型
     * setTcpForceOffset(force_offset: table) -> nil
     *
     */
    int setTcpForceOffset(const std::vector<double> &force_offset);

    /**
     * 获取末端力矩偏移
     *
     * @return 返回末端力矩偏移
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpForceOffset(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getTcpForceOffset() -> table
     *
     */
    std::vector<double> getTcpForceOffset();

    /**
     * 获取可用的底座力矩传感器的名字
     *
     * @return 返回可用的底座力矩传感器的名字
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getBaseForceSensorNames(self: pyaubo_sdk.RobotConfig) -> List[str]
     *
     * @par Lua函数原型
     * getBaseForceSensorNames() -> table
     *
     */
    std::vector<std::string> getBaseForceSensorNames();

    /**
     * 设置底座力矩传感器
     * 如果存在内置的底座力矩传感器，默认将使用内置的力矩传感器
     *
     * @param name 底座力矩传感器的名字
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * selectBaseForceSensor(self: pyaubo_sdk.RobotConfig, arg0: str) -> int
     *
     * @par Lua函数原型
     * selectBaseForceSensor(name: string) -> nil
     *
     */
    int selectBaseForceSensor(const std::string &name);

    /**
     * 是否安装了底座力矩传感器
     *
     * @return 安装返回true;没有安装返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * hasBaseForceSensor(self: pyaubo_sdk.RobotConfig) -> bool
     *
     * @par Lua函数原型
     * hasBaseForceSensor() -> boolean
     *
     */
    bool hasBaseForceSensor();

    /**
     * 设置底座力矩偏移
     *
     * @param force_offset 底座力矩偏移
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setBaseForceOffset(self: pyaubo_sdk.RobotConfig, arg0: List[float]) ->
     * int
     *
     * @par Lua函数原型
     * setBaseForceOffset(force_offset: table) -> nil
     *
     */
    int setBaseForceOffset(const std::vector<double> &force_offset);

    /**
     * 获取底座力矩偏移
     *
     * @return 返回底座力矩偏移
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getBaseForceOffset(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getBaseForceOffset() -> table
     *
     */
    std::vector<double> getBaseForceOffset();

    /**
     * 获取安全参数校验码 CRC32
     *
     * @return 返回安全参数校验码
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getSafetyParametersCheckSum(self: pyaubo_sdk.RobotConfig) -> int
     *
     * @par Lua函数原型
     * getSafetyParametersCheckSum() -> number
     *
     */
    uint32_t getSafetyParametersCheckSum();

    /**
     * 发起确认安全配置参数请求:
     * 将安全配置参数写入到安全接口板flash或文件
     *
     * @param parameters 安全配置参数
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * confirmSafetyParameters(self: pyaubo_sdk.RobotConfig, arg0:
     * arcs::common_interface::RobotSafetyParameterRange) -> int
     *
     * @par Lua函数原型
     *
     *
     */
    int confirmSafetyParameters(const RobotSafetyParameterRange &parameters);

    /**
     * 计算安全参数的 CRC32 校验值
     *
     * @return crc32
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    uint32_t calcSafetyParametersCheckSum(
        const RobotSafetyParameterRange &parameters);

    /**
     * 获取关节最大位置（物理极限）
     *
     * @return 返回关节最大位置
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointMaxPositions(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getJointMaxPositions() -> table
     *
     */
    std::vector<double> getJointMaxPositions();

    /**
     * 获取关节最小位置（物理极限）
     *
     * @return 返回关节最小位置
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointMinPositions(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getJointMinPositions() -> table
     *
     */
    std::vector<double> getJointMinPositions();

    /**
     * 获取关节最大速度（物理极限）
     *
     * @return 返回关节最大速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointMaxSpeeds(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getJointMaxSpeeds() -> table
     *
     */
    std::vector<double> getJointMaxSpeeds();

    /**
     * 获取关节最大加速度（物理极限）
     *
     * @return 返回关节最大加速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getJointMaxAccelerations(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getJointMaxAccelerations() -> table
     *
     */
    std::vector<double> getJointMaxAccelerations();

    /**
     * 获取TCP最大速度（物理极限）
     *
     * @return 返回TCP最大速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpMaxSpeeds(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getTcpMaxSpeeds() -> table
     *
     */
    std::vector<double> getTcpMaxSpeeds();

    /**
     * 获取TCP最大加速度（物理极限）
     *
     * @return 返回TCP最大加速度
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpMaxAccelerations(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getTcpMaxAccelerations() -> table
     *
     */
    std::vector<double> getTcpMaxAccelerations();

    /**
     * 设置机器人安装姿态
     *
     * @param gravity 安装姿态
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setGravity(self: pyaubo_sdk.RobotConfig, arg0: List[float]) -> int
     *
     * @par Lua函数原型
     * setGravity(gravity: table) -> nil
     *
     */
    int setGravity(const std::vector<double> &gravity);

    /**
     * 获取机器人的安装姿态
     *
     * 如果机器人底座安装了姿态传感器，则从传感器读取数据，否则按照用户设置
     *
     * @return 返回安装姿态
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getGravity(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getGravity() -> table
     *
     */
    std::vector<double> getGravity();

    /**
     * 设置当前的TCP偏移
     *
     * TCP偏移表示形式为(x,y,z,rx,ry,rz)。
     * 其中x、y、z是工具中心点（TCP）在基坐标系下相对于法兰盘中心的位置偏移，单位是m。
     * rx、ry、rz是工具中心点（TCP）在基坐标系下相对于法兰盘中心的的姿态偏移，是ZYX欧拉角，单位是rad。
     *
     * @param offset 当前的TCP偏移,形式为(x,y,z,rx,ry,rz)
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setTcpOffset(self: pyaubo_sdk.RobotConfig, arg0: List[float]) -> int
     *
     * @par Lua函数原型
     * setTcpOffset(offset: table) -> nil
     *
     */
    int setTcpOffset(const std::vector<double> &offset);

    /**
     * 获取当前的TCP偏移
     *
     * TCP偏移表示形式为(x,y,z,rx,ry,rz)。
     * 其中x、y、z是工具中心点（TCP）在基坐标系下相对于法兰盘中心的位置偏移，单位是m。
     * rx、ry、rz是工具中心点（TCP）在基坐标系下相对于法兰盘中心的的姿态偏移，是ZYX欧拉角，单位是rad。
     *
     * @return 当前的TCP偏移,形式为(x,y,z,rx,ry,rz)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getTcpOffset(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getTcpOffset() -> table
     *
     */
    std::vector<double> getTcpOffset();

    /**
     * 设置工具端质量、质心及惯量
     *
     * @param m 工具端质量
     * @param com 质心
     * @param inertial 惯量
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolInertial(self: pyaubo_sdk.RobotConfig, arg0: float, arg1:
     * List[float], arg2: List[float]) -> int
     *
     * @par Lua函数原型
     * setToolInertial(m: number, com: table, inertial: table) -> nil
     *
     */
    int setToolInertial(double m, const std::vector<double> &com,
                        const std::vector<double> &inertial);

    /**
     * 设置有效负载
     *
     * @param m 质量, 单位: kg
     * @param cog 重心, 单位: m, 形式为(CoGx, CoGy, CoGz)
     * @param aom 力矩轴的方向, 单位: rad, 形式为(rx, ry, rz)
     * @param inertia 惯量, 单位: kg*m^2, 形式为(Ixx, Ixy, Ixz, Iyx, Iyy, Iyz,
     * Izx, Izy, Izz)
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setPayload(self: pyaubo_sdk.RobotConfig, arg0: float, arg1: List[float],
     * arg2: List[float], arg3: List[float]) -> int
     *
     * @par Lua函数原型
     * setPayload(m: number, cog: table, aom: table, inertia: table) -> nil
     *
     * @par Lua示例
     * setPayload(3, {0,0,0}, {0,0,0}, {0,0,0,0,0,0,0,0,0})
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotConfig.setPayload","params":[3,[0,0,0],[0,0,0],[0,0,0,0,0,0,0,0,0]],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int setPayload(double m, const std::vector<double> &cog,
                   const std::vector<double> &aom,
                   const std::vector<double> &inertia);

    /**
     * 获取有效负载
     *
     * @return 有效负载.
     * 第一个元素表示质量, 单位: kg;
     * 第二个元素表示重心, 单位: m, 形式为(CoGx, CoGy, CoGz);
     * 第三个元素表示力矩轴的方向, 单位: rad, 形式为(rx, ry, rz);
     * 第四个元素表示惯量, 单位: kg*m^2, 形式为(Ixx, Ixy, Ixz, Iyx, Iyy, Iyz,
     * Izx, Izy, Izz)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getPayload(self: pyaubo_sdk.RobotConfig) -> Tuple[float, List[float],
     * List[float], List[float]]
     *
     * @par Lua函数原型
     * getPayload() -> number, table, table, table
     *
     * @par Lua示例
     * m, cog, aom, inertia = getPayload()
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.RobotConfig.getPayload","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":[3.0,[0.0,0.0,0.0],[0.0,0.0,0.0],[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]]}
     *
     */
    Payload getPayload();

    /**
     * 末端位姿是否在安全范围之内
     *
     * @param pose 末端位姿
     * @return 在安全范围内返回true; 反之返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * toolSpaceInRange(self: pyaubo_sdk.RobotConfig, arg0: List[float]) -> bool
     *
     * @par Lua函数原型
     * toolSpaceInRange(pose: table) -> boolean
     *
     */
    bool toolSpaceInRange(const std::vector<double> &pose);

    /**
     * 发起固件升级请求，控制器软件将进入固件升级模式
     *
     * @param fw 固件路径 \n
     *     pm://param/model/xx.bin \n
     *     /absolute/path/to/xx.bin \n
     *     relative/path/to/xx.bin
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE: 当前运行时状态不处于 Stopped, 固件升级请求被拒绝
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * firmwareUpdate(self: pyaubo_sdk.RobotConfig, arg0: str) -> int
     *
     * @par Lua函数原型
     * firmwareUpdate(fw: string) -> nil
     *
     */
    int firmwareUpdate(const std::string &fw);

    /**
     * 获取固件升级的进程
     *
     * @return 返回升级进程 \n
     * std::string 代表步骤名称 \n
     * double 代表进度(0~1)，完成之后，返回("", 1)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getFirmwareUpdateProcess(self: pyaubo_sdk.RobotConfig) -> Tuple[str,
     * float]
     *
     * @par Lua函数原型
     * getFirmwareUpdateProcess() -> table
     *
     */
    std::tuple<std::string, double> getFirmwareUpdateProcess();

    /**
     * 获取关节最大位置（当前正在使用的限制值）
     *
     * @return 返回关节最大位置（当前正在使用的限制值）
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getLimitJointMaxPositions(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getLimitJointMaxPositions() -> table
     *
     */
    std::vector<double> getLimitJointMaxPositions();

    /**
     * 获取关节最小位置（当前正在使用的限制值）
     *
     * @return 返回关节最小位置（当前正在使用的限制值）
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getLimitJointMinPositions(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getLimitJointMinPositions() -> table
     *
     */
    std::vector<double> getLimitJointMinPositions();

    /**
     * 获取关节最大速度（当前正在使用的限制值）
     *
     * @return 返回关节最大速度（当前正在使用的限制值）
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getLimitJointMaxSpeeds(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getLimitJointMaxSpeeds() -> table
     *
     */
    std::vector<double> getLimitJointMaxSpeeds();

    /**
     * 获取关节最大加速度（当前正在使用的限制值）
     *
     * @return 返回关节最大加速度（当前正在使用的限制值）
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getLimitJointMaxAccelerations(self: pyaubo_sdk.RobotConfig) ->
     * List[float]
     *
     * @par Lua函数原型
     * getLimitJointMaxAccelerations() -> table
     *
     */
    std::vector<double> getLimitJointMaxAccelerations();

    /**
     * 获取TCP最大速度（当前正在使用的限制值）
     *
     * @return 返回TCP最大速度（当前正在使用的限制值）
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getLimitTcpMaxSpeed(self: pyaubo_sdk.RobotConfig) -> List[float]
     *
     * @par Lua函数原型
     * getLimitTcpMaxSpeed() -> table
     *
     */
    double getLimitTcpMaxSpeed();

    /**
     * 获取当前安全停止的类型
     *
     * @return 返回当前安全停止的类型
     *
     */
    SafeguedStopType getSafeguardStopType();

    /**
     * 按位获取完整的安全停止触发源
     *
     * @return 返回所有安全停止触发源
     *
     * 安全停止的原因:
     * 手动模式下可配置安全IO触发的安全停止 - 1<<0
     * 自动模式下可配置安全IO触发的安全停止 - 1<<1
     * 控制柜SI输入触发的安全停止 - 1<<2
     * 示教器三态开关触发的安全停止 - 1<<3
     * 自动切手动触发的安全停止 - 1<<4
     *
     */
    int getSafeguardStopSource();

protected:
    void *d_;
};
using RobotConfigPtr = std::shared_ptr<RobotConfig>;

// clang-format off
#define RobotConfig_DECLARES                                                     \
    _FUNC(RobotConfig, 0, getDof)                                                \
    _FUNC(RobotConfig, 0, getName)                                               \
    _FUNC(RobotConfig, 0, getCycletime)                                          \
    _FUNC(RobotConfig, 2, setSlowDownFraction, level, fraction)                  \
    _FUNC(RobotConfig, 1, getSlowDownFraction, level)                            \
    _FUNC(RobotConfig, 0, getRobotType)                                          \
    _FUNC(RobotConfig, 0, getRobotSubType)                                       \
    _FUNC(RobotConfig, 0, getControlBoxType)                                     \
    _FUNC(RobotConfig, 0, getDefaultToolAcc)                                     \
    _FUNC(RobotConfig, 0, getDefaultToolSpeed)                                   \
    _FUNC(RobotConfig, 0, getDefaultJointAcc)                                    \
    _FUNC(RobotConfig, 0, getDefaultJointSpeed)                                  \
    _INST(RobotConfig, 1, setMountingPose, pose)                                 \
    _FUNC(RobotConfig, 0, getMountingPose)                                       \
    _INST(RobotConfig, 1, setCollisionLevel, level);                             \
    _FUNC(RobotConfig, 0, getCollisionLevel);                                    \
    _INST(RobotConfig, 1, setCollisionStopType, type);                           \
    _FUNC(RobotConfig, 0, getCollisionStopType);                                 \
    _FUNC(RobotConfig, 1, setHomePosition, positions);                           \
    _FUNC(RobotConfig, 0, getHomePosition);                                      \
    _INST(RobotConfig, 1, setFreedriveDamp, damp);                               \
    _FUNC(RobotConfig, 0, getFreedriveDamp);                                     \
    _FUNC(RobotConfig, 0, getTcpForceSensorNames)                                \
    _INST(RobotConfig, 1, selectTcpForceSensor, name)                            \
    _FUNC(RobotConfig, 0, hasTcpForceSensor)                                     \
    _INST(RobotConfig, 1, setTcpForceOffset, force_offset)                       \
    _FUNC(RobotConfig, 0, getTcpForceOffset)                                     \
    _FUNC(RobotConfig, 0, getBaseForceSensorNames)                               \
    _INST(RobotConfig, 1, selectBaseForceSensor, name)                           \
    _FUNC(RobotConfig, 0, hasBaseForceSensor)                                    \
    _INST(RobotConfig, 1, setBaseForceOffset, force_offset)                      \
    _FUNC(RobotConfig, 0, getBaseForceOffset)                                    \
    _INST(RobotConfig, 1, setPersistentParameters, param)                        \
    _INST(RobotConfig, 1, setKinematicsCompensate, param)                        \
    _INST(RobotConfig, 1, setHardwareCustomParameters, param)                    \
    _FUNC(RobotConfig, 1, getHardwareCustomParameters, param)                    \
    _INST(RobotConfig, 0, setRobotZero)                                          \
    _FUNC(RobotConfig, 1, getKinematicsParam, real)                              \
    _FUNC(RobotConfig, 1, getKinematicsCompensate, ref_temperature)              \
    _FUNC(RobotConfig, 0, getSafetyParametersCheckSum)                           \
    _FUNC(RobotConfig, 1, confirmSafetyParameters, parameters)                   \
    _FUNC(RobotConfig, 1, calcSafetyParametersCheckSum, parameters)              \
    _FUNC(RobotConfig, 0, getJointMaxPositions)                                  \
    _FUNC(RobotConfig, 0, getJointMinPositions)                                  \
    _FUNC(RobotConfig, 0, getJointMaxSpeeds)                                     \
    _FUNC(RobotConfig, 0, getJointMaxAccelerations)                              \
    _FUNC(RobotConfig, 0, getTcpMaxSpeeds)                                       \
    _FUNC(RobotConfig, 0, getTcpMaxAccelerations)                                \
    _FUNC(RobotConfig, 1, toolSpaceInRange, pose)                                \
    _INST(RobotConfig, 4, setPayload, m, cog, aom, inertia)                      \
    _FUNC(RobotConfig, 0, getPayload)                                            \
    _FUNC(RobotConfig, 0, getTcpOffset)                                          \
    _FUNC(RobotConfig, 0, getGravity)                                            \
    _INST(RobotConfig, 1, setGravity, gravity)                                   \
    _INST(RobotConfig, 1, setTcpOffset, offset)                                  \
    _INST(RobotConfig, 3, setToolInertial, m, com, inertial)                     \
    _FUNC(RobotConfig, 1, firmwareUpdate, fw)                                    \
    _FUNC(RobotConfig, 0, getFirmwareUpdateProcess)                              \
    _FUNC(RobotConfig, 1, setTcpForceSensorPose, sensor_pose)                    \
    _FUNC(RobotConfig, 0, getTcpForceSensorPose)                                 \
    _FUNC(RobotConfig, 0, getLimitJointMaxPositions)                             \
    _FUNC(RobotConfig, 0, getLimitJointMinPositions)                             \
    _FUNC(RobotConfig, 0, getLimitJointMaxSpeeds)                                \
    _FUNC(RobotConfig, 0, getLimitJointMaxAccelerations)                         \
    _FUNC(RobotConfig, 0, getLimitTcpMaxSpeed)                                   \
    _FUNC(RobotConfig, 0, getSafeguardStopType)                                  \
    _FUNC(RobotConfig, 0, getSafeguardStopSource)

// clang-format on
} // namespace common_interface
} // namespace arcs
#endif // AUBO_SDK_ROBOT_CONFIG_H
