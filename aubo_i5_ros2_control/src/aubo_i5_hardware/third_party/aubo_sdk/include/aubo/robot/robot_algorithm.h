/** @file  robot_algorithm.h
 *  @brief 机器人算法相关的对外接口
 */
#ifndef AUBO_SDK_ROBOT_ALGORITHM_INTERFACE_H
#define AUBO_SDK_ROBOT_ALGORITHM_INTERFACE_H

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <aubo/global_config.h>
#include <aubo/type_def.h>

namespace arcs {
namespace common_interface {

/**
 * 机器人算法相关的对外接口
 */
class ARCS_ABI_EXPORT RobotAlgorithm
{
public:
    RobotAlgorithm();
    virtual ~RobotAlgorithm();

    /**
     * 力传感器标定算法(三点标定法)
     *
     * @param force
     * @param q
     * @return
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * calibrateTcpForceSensor(self: pyaubo_sdk.RobotAlgorithm, arg0:
     * List[List[float]], arg1: List[List[float]]) -> Tuple[List[float],
     * List[float], float, List[float]]
     *
     * @par Lua函数原型
     * calibrateTcpForceSensor(force: table, q: table) -> table
     *
     */
    ForceSensorCalibResult calibrateTcpForceSensor(
        const std::vector<std::vector<double>> &forces,
        const std::vector<std::vector<double>> &poses);

    /**
     * 力传感器标定算法(三点标定法)
     * @param forces
     * @param poses
     * @return force_offset, com, mass, angle error
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    ForceSensorCalibResultWithError calibrateTcpForceSensor2(
        const std::vector<std::vector<double>> &forces,
        const std::vector<std::vector<double>> &poses);

    /**
     * 基于电流的负载辨识算法接口
     *
     * 需要采集空载时运行激励轨迹的位置、速度、电流以及带负载时运行激励轨迹的位置、速度、电流
     *
     * @param data_file_no_payload
     * 空载时运行激励轨迹各关节数据的文件路径（.csv格式），共18列，依次为6个关节位置、6个关节速度、6个关节电流
     * @param data_file_with_payload
     * 带负载运行激励轨迹各关节数据的文件路径（.csv格式），共18列，依次为6个关节位置、6个关节速度、6个关节电流
     * @return 辨识的结果
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * payloadIdentify(self: pyaubo_sdk.RobotAlgorithm, arg0: List[List[float]],
     * arg1: List[List[float]]) -> Tuple[List[float], List[float], float,
     * List[float]]
     *
     * @par Lua函数原型
     * payloadIdentify(data_with_payload: table, data_with_payload: table) ->
     * table
     *
     */
    int payloadIdentify(const std::string &data_file_no_payload,
                        const std::string &data_file_with_payload);

    /**
     * 新版基于电流的负载辨识算法接口
     *
     * 需要采集带载时运行最少三个点的位置、速度、加速度、电流、温度、末端传感器数据、底座数据
     *
     * @param data
     * 带负载的各关节数据的文件路径（.csv格式），共42列，末端传感器数据、底座数据默认为0
     * @return 辨识的结果
     *
     */
    int payloadIdentify1(const std::string &file_name);

    /**
     * 负载辨识是否计算完成
     * @return 完成返回0; 正在进行中返回1; 计算失败返回<0;
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int payloadCalculateFinished();

    /**
     * 获取负载辨识结果
     * @return
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    Payload getPayloadIdentifyResult();

    /**
     * 关节摩擦力模型辨识算法接口
     *
     * @param q
     * @param qd
     * @param qdd
     * @param temp
     * @return
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * frictionModelIdentify(self: pyaubo_sdk.RobotAlgorithm, arg0:
     * List[List[float]], arg1: List[List[float]], arg2: List[List[float]],
     * arg3: List[List[float]]) -> bool
     *
     * @par Lua函数原型
     * frictionModelIdentify(q: table, qd: table, qdd: table, temp: table) ->
     * boolean
     *
     */
    bool frictionModelIdentify(const std::vector<std::vector<double>> &q,
                               const std::vector<std::vector<double>> &qd,
                               const std::vector<std::vector<double>> &qdd,
                               const std::vector<std::vector<double>> &temp);

    /**
     * 工件坐标系标定算法接口(需要在调用之前正确的设置机器人的TCP偏移)
     * 输入多组关节角度和标定类型，输出工件坐标系位姿(相对于机器人基坐标系)
     *
     * @param q 关节角度
     * @param type 标定类型
     * @return 计算结果(工件坐标系位姿)以及错误代码
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * calibWorkpieceCoordinatePara(self: pyaubo_sdk.RobotAlgorithm, arg0:
     * List[List[float]], arg1: int) -> Tuple[List[float], int]
     *
     * @par Lua函数原型
     * calibWorkpieceCoordinatePara(q: table, type: number) -> table, number
     *
     */
    ResultWithErrno calibWorkpieceCoordinatePara(
        const std::vector<std::vector<double>> &q, int type);

    /**
     * 动力学正解
     *
     * @param q 关节角
     * @param torqs
     * @return 计算结果以及错误代码
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * forwardDynamics(self: pyaubo_sdk.RobotAlgorithm, arg0: List[float], arg1:
     * List[float]) -> Tuple[List[float], int]
     *
     * @par Lua函数原型
     * forwardDynamics(q: table, torqs: table) -> table, number
     *
     */
    ResultWithErrno forwardDynamics(const std::vector<double> &q,
                                    const std::vector<double> &torqs);

    /**
     * 动力学正解，基于给定的TCP偏移
     *
     * @param q 关节角
     * @param torqs
     * @return 计算结果以及错误代码，同forwardDynamics
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * forwardDynamics(self: pyaubo_sdk.RobotAlgorithm, arg0: List[float], arg1:
     * List[float]) -> Tuple[List[float], int]
     *
     * @par Lua函数原型
     * forwardDynamics(q: table, torqs: table) -> table, number
     *
     */
    ResultWithErrno forwardDynamics1(const std::vector<double> &q,
                                     const std::vector<double> &torqs,
                                     const std::vector<double> &tcp_offset);

    /**
     * 运动学正解, 基于激活的TCP偏移(最近的通过 setTcpOffset 设置的参数)
     * 输入关节角度，输出TCP位姿
     *
     * @param q 关节角
     * @return TCP位姿和正解结果是否有效
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * forwardKinematics(self: pyaubo_sdk.RobotAlgorithm, arg0: List[float]) ->
     * Tuple[List[float], int]
     *
     * @par Lua函数原型
     * forwardKinematics(q: table) -> table, number
     *
     */
    ResultWithErrno forwardKinematics(const std::vector<double> &q);

    /**
     * 运动学正解
     * 输入关节角度，输出TCP位姿
     *
     * @param q 关节角
     * @param tcp_offset tcp偏移
     * @return TCP位姿和正解结果是否有效
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * forwardKinematics(self: pyaubo_sdk.RobotAlgorithm, arg0: List[float],
     * arg1: List[float]) -> Tuple[List[float], int]
     *
     * @par Lua函数原型
     * forwardKinematics1(q: table, tcp_offset: table) -> table, number
     *
     * @since 0.24.1
     *
     */
    ResultWithErrno forwardKinematics1(const std::vector<double> &q,
                                       const std::vector<double> &tcp_offset);

    /**
     * 运动学正解(忽略 TCP 偏移值)
     *
     * @param q 关节角
     * @return 法兰盘中心位姿和正解结果是否有效
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    ResultWithErrno forwardToolKinematics(const std::vector<double> &q);

    /**
     * 运动学逆解
     * 输入TCP位姿和参考关节角度，输出关节角度
     *
     * @param qnear 参考关节角
     * @param pose TCP位姿
     * @return 关节角和逆解结果是否有效
     * 返回值的第一个参数为逆解结果，第二个为逆解错误码，错误码返回列表如下
     * 0 - 成功
     * -23 - 逆解计算不收敛，计算出错
     * -24 - 逆解计算超出机器人最大限制
     * -25 - 逆解输入配置存在错误
     * -26 - 逆解雅可比矩阵计算失败
     * -27 - 目标点存在解析解，但均不满足选解条件
     * -28 - 逆解返回未知类型错误
     * 若错误码非0,则返回值的第一个参数为输入参考关节角qnear
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * inverseKinematics(self: pyaubo_sdk.RobotAlgorithm, arg0: List[float],
     * arg1: List[float]) -> Tuple[List[float], int]
     *
     * @par Lua函数原型
     * inverseKinematics(qnear: table, pose: table) -> table, int
     *
     */
    ResultWithErrno inverseKinematics(const std::vector<double> &qnear,
                                      const std::vector<double> &pose);

    /**
     * 运动学逆解
     * 输入TCP位姿和参考关节角度，输出关节角度
     *
     * @param qnear 参考关节角
     * @param pose TCP位姿
     * @param tcp_offset TCP偏移
     * @return 关节角和逆解结果是否有效，同 inverseKinematics
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * inverseKinematics(self: pyaubo_sdk.RobotAlgorithm, arg0: List[float],
     * arg1: List[float]) -> Tuple[List[float], int]
     *
     * @par Lua函数原型
     * inverseKinematics(qnear: table, pose: table) -> table, int
     *
     */
    ResultWithErrno inverseKinematics1(const std::vector<double> &qnear,
                                       const std::vector<double> &pose,
                                       const std::vector<double> &tcp_offset);

    /**
     * 求出所有的逆解, 基于激活的 TCP 偏移
     *
     * @param pose TCP位姿
     * @return 关节角和逆解结果是否有效
     * 返回的错误码同inverseKinematics
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    ResultWithErrno1 inverseKinematicsAll(const std::vector<double> &pose);

    /**
     * 求出所有的逆解, 基于提供的 TCP 偏移
     *
     * @param pose TCP位姿
     * @param tcp_offset TCP偏移
     * @return 关节角和逆解结果是否有效，同 inverseKinematicsAll
     * 返回的错误码同inverseKinematics
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    ResultWithErrno1 inverseKinematicsAll1(
        const std::vector<double> &pose, const std::vector<double> &tcp_offset);

    /**
     * 运动学逆解(忽略 TCP 偏移值)
     *
     * @param qnear 参考关节角
     * @param pose 法兰盘中心的位姿
     * @return 关节角和逆解结果是否有效
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    ResultWithErrno inverseToolKinematics(const std::vector<double> &qnear,
                                          const std::vector<double> &pose);

    /**
     * 运动学逆解(忽略 TCP 偏移值)
     *
     * @param qnear 参考关节角
     * @param pose 法兰盘中心的位姿
     * @return 关节角和逆解结果是否有效
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    ResultWithErrno1 inverseToolKinematicsAll(const std::vector<double> &pose);

    /**
     * 求解movej之间的轨迹点
     *
     * @param q1 movej的起点
     * @param r1 在q1处的交融半径
     * @param q2 movej的终点
     * @param r2 在q2处的交融半径
     * @param d 采样距离
     * @return q1~q2 之间笛卡尔空间离散轨迹点(x,y,z,rx,ry,rz)集合
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * pathMovej(self: pyaubo_sdk.RobotAlgorithm, arg0: List[float], arg1:
     * float, arg2: List[float], arg3: float, arg4: float) -> List[List[float]]
     *
     * @par Lua函数原型
     * pathMovej(q1: table, r1: number, q2: table, r2: number, d: number) ->
     * table, number
     */
    std::vector<std::vector<double>> pathMovej(const std::vector<double> &q1,
                                               double r1,
                                               const std::vector<double> &q2,
                                               double r2, double d);
    /**
     * 计算机械臂末端的雅克比矩阵
     *
     * @param q 关节角
     * @param base_or_end 参考坐标系为基坐标系（或者末端坐标系）
     *        true: 在 base 下描述
     *        false: 在 末端坐标系 下描述
     * @return 雅克比矩阵是否有效
     * 返回值的第一个参数为该构型下对应的雅克比矩阵，第二个为逆解错误码
     * 此接口的错误码返回值在0.28.1-rc.21 0.29.0-alpha.25版本之后做了修改。
     * 此前逆解错误时返回 30082 ，修改后错误码返回列表如下
     * 0 - 成功
     * -23 - 逆解计算不收敛，计算出错
     * -24 - 逆解计算超出机器人最大限制
     * -25 - 逆解输入配置存在错误
     * -26 - 逆解雅可比矩阵计算失败
     * -27 - 目标点存在解析解，但均不满足选解条件
     * -28 - 逆解返回未知类型错误
     * 若错误码非0,则返回值的第一个参数为输入参考关节角qnear
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * calJacobian(self: pyaubo_sdk.RobotAlgorithm, arg0: List[float],
     * arg1: bool) -> Tuple[List[float], int]
     *
     * @par Lua函数原型
     * calJacobian(q: table, base_or_end: boolean) -> table
     *
     */
    ResultWithErrno calcJacobian(const std::vector<double> &q,
                                 bool base_or_end);

    /**
     * 求解交融的轨迹点
     *
     * @param type
     * 0-movej and movej
     * 1-movej and movel
     * 2-movel and movej
     * 3-movel and movel
     * @param q_start 交融前路径的起点
     * @param q_via 在q1处的交融半径
     * @param q_to 交融后路径的终点
     * @param r 在q_via处的交融半径
     * @param d 采样距离
     * @return q_via处的交融段笛卡尔空间离散轨迹点(x,y,z)集合
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * pathBlend3Points(self: pyaubo_sdk.RobotAlgorithm, arg0: int, arg1:
     * List[float], arg2: List[float], arg3: List[float], arg4: float, arg5:
     * float) -> List[List[float]]
     *
     *
     * @par Lua函数原型
     * pathBlend3Points(type: number, q_start: table, q_via: table, q_to: table,
     * r: number, d: number) -> table, number
     */
    std::vector<std::vector<double>> pathBlend3Points(
        int type, const std::vector<double> &q_start,
        const std::vector<double> &q_via, const std::vector<double> &q_to,
        double r, double d);

    /**
     * 生成用于负载辨识的激励轨迹
     * 此接口内部调用pathBufferAppend
     * 将离线轨迹存入buffer中，后续可通过movePathBuffer运行离线轨迹
     * @param name 轨迹名字
     * @param traj_conf 各关节轨迹的限制条件
     * traj_conf.move_axis: 运动的轴
     * 由于实际用户现场可能不希望在负载辨识时控制机械臂多关节大幅度运动，故最好选用traj_conf.move_axis=LoadIdentifyMoveAxis::Joint_4_6;
     * traj_conf.init_joint:
     * 运动初始关节角,为了避免关节5接近零位时的奇异问题，应设置traj_conf.init_joint[4]的绝对值不小于0.3(rad)，接近1.57(rad)为宜。其余关节的关节角可任意设置
     * traj_conf.lower_joint_bound, traj_conf.upper_joint_bound:
     * 关节角上下限,维度应与config.move_axis维度保持一致,推荐设置upper_joint_bound为2，lower_joint_bound为-2
     * config.max_velocity, config.max_acceleration:
     * 关节角速度角加速度限制,维度应与config.move_axis维度保持一致,出于安全和驱动器跟随性能的考虑,推荐设置max_velocity=3,max_acceleration=5
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
    int generatePayloadIdentifyTraj(const std::string &name,
                                    const TrajConfig &traj_conf);
    /**
     * 负载辨识轨迹是否生成完成
     *
     * @return 完成返回0; 正在进行中返回1; 计算失败返回<0;
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int payloadIdentifyTrajGenFinished();

protected:
    void *d_;
};
using RobotAlgorithmPtr = std::shared_ptr<RobotAlgorithm>;

// clang-format off
#define RobotAlgorithm_DECLARES                                                   \
    _FUNC(RobotAlgorithm, 2, calibrateTcpForceSensor, forces, poses)              \
    _FUNC(RobotAlgorithm, 2, calibrateTcpForceSensor2, forces, poses)             \
    _FUNC(RobotAlgorithm, 2, payloadIdentify, data_no_payload, data_with_payload) \
    _FUNC(RobotAlgorithm, 1, payloadIdentify1, file_name)                         \
    _FUNC(RobotAlgorithm, 0, payloadCalculateFinished)                            \
    _FUNC(RobotAlgorithm, 0, getPayloadIdentifyResult)                            \
    _FUNC(RobotAlgorithm, 2, generatePayloadIdentifyTraj, name, TrajConfig)       \
    _FUNC(RobotAlgorithm, 0, payloadIdentifyTrajGenFinished)                      \
    _FUNC(RobotAlgorithm, 4, frictionModelIdentify, q, qd, qdd, temp)             \
    _FUNC(RobotAlgorithm, 2, calibWorkpieceCoordinatePara, q, type)               \
    _FUNC(RobotAlgorithm, 2, forwardDynamics, q, torqs)                           \
    _FUNC(RobotAlgorithm, 1, forwardKinematics, q)                                \
    _FUNC(RobotAlgorithm, 1, forwardToolKinematics, q)                            \
    _FUNC(RobotAlgorithm, 3, forwardDynamics1, q, torqs, tcp_offset)              \
    _FUNC(RobotAlgorithm, 2, forwardKinematics1, q, tcp_offset)                   \
    _FUNC(RobotAlgorithm, 2, inverseKinematics, qnear, pose)                      \
    _FUNC(RobotAlgorithm, 1, inverseKinematicsAll, pose)                          \
    _FUNC(RobotAlgorithm, 3, inverseKinematics1, qnear, pose, tcp_offset)         \
    _FUNC(RobotAlgorithm, 2, inverseKinematicsAll1, pose, tcp_offset)             \
    _FUNC(RobotAlgorithm, 2, inverseToolKinematics, qnear, pose)                  \
    _FUNC(RobotAlgorithm, 1, inverseToolKinematicsAll, pose)                      \
    _FUNC(RobotAlgorithm, 5, pathMovej, q1, r1, q2, r2, d)                        \
    _FUNC(RobotAlgorithm, 6, pathBlend3Points, type, q_start, q_via, q_to, r, d)  \
    _FUNC(RobotAlgorithm, 2, calcJacobian, q, base_or_end)

// clang-format on

} // namespace common_interface
} // namespace arcs

#endif // AUBO_SDK_ROBOT_ALGORITHM_INTERFACE_H
