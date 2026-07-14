/** @file  robot_interface.h
 *  @brief 机器人API 接口
 */
#ifndef AUBO_SDK_ROBOT_INTERFACE_H
#define AUBO_SDK_ROBOT_INTERFACE_H

#include <aubo/sync_move.h>
#include <aubo/trace.h>
#include <aubo/robot/motion_control.h>
#include <aubo/robot/force_control.h>
#include <aubo/robot/io_control.h>
#include <aubo/robot/robot_algorithm.h>
#include <aubo/robot/robot_state.h>
#include <aubo/robot/robot_manage.h>
#include <aubo/robot/robot_config.h>
#include <aubo/global_config.h>

namespace arcs {
namespace common_interface {

class ARCS_ABI_EXPORT RobotInterface
{
public:
    RobotInterface();
    virtual ~RobotInterface();

    /**
     * 获取RobotConfig接口
     *
     * @return RobotConfigPtr对象的指针
     *
     * @par Python函数原型
     * getRobotConfig(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::RobotConfig
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * RobotConfigPtr ptr =
     * rpc_cli->getRobotInterface(robot_name)->getRobotConfig();
     * @endcode
     */
    RobotConfigPtr getRobotConfig();

    /**
     * 获取运动规划接口
     *
     * @return MotionControlPtr对象的指针
     *
     * @par Python函数原型
     * getMotionControl(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::MotionControl
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * MotionControlPtr ptr =
     * rpc_cli->getRobotInterface(robot_name)->getMotionControl();
     * @endcode
     *
     */
    MotionControlPtr getMotionControl();

    /**
     * 获取力控接口
     *
     * @return ForceControlPtr对象的指针
     *
     * @par Python函数原型
     * getForceControl(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::ForceControl
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * ForceControlPtr ptr =
     * rpc_cli->getRobotInterface(robot_name)->getForceControl();
     * @endcode
     *
     */
    ForceControlPtr getForceControl();

    /**
     * 获取IO控制的接口
     *
     * @return IoControlPtr对象的指针
     *
     * @par Python函数原型
     * getIoControl(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::IoControl
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * IoControlPtr ptr =
     * rpc_cli->getRobotInterface(robot_name)->getIoControl();
     * @endcode
     *
     */
    IoControlPtr getIoControl();

    /**
     * 获取同步运动接口
     *
     * @return SyncMovePtr对象的指针
     *
     * @par Python函数原型
     * getSyncMove(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::SyncMove
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * SyncMovePtr ptr = rpc_cli->getRobotInterface(robot_name)->getSyncMove();
     * @endcode
     *
     */
    SyncMovePtr getSyncMove();

    /**
     * 获取机器人实用算法接口
     *
     * @return RobotAlgorithmPtr对象的指针
     *
     * @par Python函数原型
     * getRobotAlgorithm(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::RobotAlgorithm
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * RobotAlgorithmPtr ptr =
     * rpc_cli->getRobotInterface(robot_name)->getRobotAlgorithm();
     * @endcode
     *
     */
    RobotAlgorithmPtr getRobotAlgorithm();

    /**
     * 获取机器人管理接口(上电、启动、停止等)
     *
     * @return RobotManagePtr对象的指针
     *
     * @par Python函数原型
     * getRobotManage(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::RobotManage
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * RobotManagePtr ptr =
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage();
     * @endcode
     *
     */
    RobotManagePtr getRobotManage();

    /**
     * 获取机器人状态接口
     *
     * @return RobotStatePtr对象的指针
     *
     * @par Python函数原型
     * getRobotState(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::RobotState
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * RobotStatePtr ptr =
     * rpc_cli->getRobotInterface(robot_name)->getRobotState();
     * @endcode
     *
     */
    RobotStatePtr getRobotState();

    /**
     * 获取告警信息接口
     *
     * @return TracePtr对象的指针
     *
     * @par Python函数原型
     * getTrace(self: pyaubo_sdk.RobotInterface) ->
     * arcs::common_interface::Trace
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * TracePtr ptr = rpc_cli->getRobotInterface(robot_name)->getTrace();
     * @endcode
     *
     */
    TracePtr getTrace();

protected:
    void *d_;
};
using RobotInterfacePtr = std::shared_ptr<RobotInterface>;

// clang-format off
#define RobotInterface_DECLARES                   \
    _FUNC(RobotInterface, 0, getRobotConfig)      \
    _FUNC(RobotInterface, 0, getMotionControl)    \
    _FUNC(RobotInterface, 0, getForceControl)     \
    _FUNC(RobotInterface, 0, getIoControl)        \
    _FUNC(RobotInterface, 0, getSyncMove)         \
    _FUNC(RobotInterface, 0, getRobotAlgorithm)   \
    _FUNC(RobotInterface, 0, getRobotManage)      \
    _FUNC(RobotInterface, 0, getRobotState)       \
    _FUNC(RobotInterface, 0, getTrace)
// clang-format on

} // namespace common_interface
} // namespace arcs

#endif // AUBO_SDK_ROBOT_INTERFACE_H
