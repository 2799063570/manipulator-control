/** @file  robot_manage.h
 *  @brief 机器人管理接口，如上电、启动、拖动示教模式等
 */
#ifndef AUBO_SDK_ROBOT_CONTROL_INTERFACE_H
#define AUBO_SDK_ROBOT_CONTROL_INTERFACE_H

#include <vector>
#include <thread>

#include <aubo/global_config.h>
#include <aubo/type_def.h>

namespace arcs {
namespace common_interface {

class ARCS_ABI_EXPORT RobotManage
{
public:
    RobotManage();
    virtual ~RobotManage();

    /**
     * 发起机器人上电请求
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * poweron(self: pyaubo_sdk.RobotManage) -> int
     *
     * @par Lua函数原型
     * poweron() -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->poweron();
     * @endcode
     *
     */
    int poweron();

    /**
     * 发起机器人启动请求
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * startup(self: pyaubo_sdk.RobotManage) -> int
     *
     * @par Lua函数原型
     * startup() -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->startup();
     * @endcode
     */
    int startup();

    /**
     * 发起机器人断电请求
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * poweroff(self: pyaubo_sdk.RobotManage) -> int
     *
     * @par Lua函数原型
     * poweroff() -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->poweroff();
     * @endcode
     */
    int poweroff();

    /**
     * 发起机器人反向驱动请求
     *
     * @param enable
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * backdrive(self: pyaubo_sdk.RobotManage, arg0: bool) -> int
     *
     * @par Lua函数原型
     * backdrive(enable: boolean) -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->backdrive(true);
     * @endcode
     *
     */
    int backdrive(bool enable);

    /**
     * 发起机器人自由驱动请求
     *
     * @param enable
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * freedrive(self: pyaubo_sdk.RobotManage, arg0: bool) -> int
     *
     * @par Lua函数原型
     * freedrive(enable: boolean) -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->freedrive(true);
     * @endcode
     *
     */
    int freedrive(bool enable);

    /**
     * 高阶拖动示教
     *
     * @note 暂未实现
     *
     * @param freeAxes
     * @param feature 如果维度为0，代表基于 TCP 坐标系拖动
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int handguideMode(const std::vector<int> &freeAxes,
                      const std::vector<double> &feature);

    /**
     * 退出拖动示教
     *
     * @note 暂未实现
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int exitHandguideMode();

    /**
     * 获取拖动示教器的状态（是否处于奇异空间）
     *
     * @note 暂未实现
     *
     * @return
     * • 0 - 正常操作.
     * • 1 - 接近奇异空间.
     * • 2 - 极其接近奇异点，将产生较大的拖动阻尼.
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int getHandguideStatus();

    /**
     * 获取拖动示教器触发源
     *
     * @note 暂未实现
     *
     * @return
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int getHandguideTrigger();

    /**
     * 发起机器人进入/退出仿真模式请求
     *
     * @param enable
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
     * setSim(self: pyaubo_sdk.RobotManage, arg0: bool) -> int
     *
     * @par Lua函数原型
     * setSim(enable: boolean) -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->setSim(true);
     * @endcode
     *
     */
    int setSim(bool enable);

    /**
     * 设置机器人操作模式
     *
     * @param mode 操作模式
     *
     * @return 成功返回0; 失败返回错误码
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setOperationalMode(self: pyaubo_sdk.RobotManage, arg0:
     * arcs::common_interface::OperationalModeType) -> int
     *
     * @par Lua函数原型
     * setOperationalMode(mode: number) -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->setOperationalMode(OperationalModeType::Automatic);
     * @endcode
     *
     */
    int setOperationalMode(OperationalModeType mode);

    /**
     * 获取机器人操作模式
     *
     * @return 机器人操作模式
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getOperationalMode(self: pyaubo_sdk.RobotManage) ->
     * arcs::common_interface::OperationalModeType
     *
     * @par Lua函数原型
     * getOperationalMode() -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * OperationalModeType mode =
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->getOperationalMode();
     * @endcode
     *
     */
    OperationalModeType getOperationalMode();

    /**
     * 获取控制模式
     *
     * @return 控制模式
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getRobotControlMode(self: pyaubo_sdk.RobotManage) ->
     * arcs::common_interface::RobotControlModeType
     *
     * @par Lua函数原型
     * getRobotControlMode() -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * RobotControlModeType mode =
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->getRobotControlMode();
     * @endcode
     */
    RobotControlModeType getRobotControlMode();

    /**
     * 是否使能了拖动示教模式
     *
     * @return 使能返回true; 反之返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isFreedriveEnabled(self: pyaubo_sdk.RobotManage) -> bool
     *
     * @par Lua函数原型
     * isFreedriveEnabled() -> boolean
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * bool isEnabled =
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->isFreedriveEnabled();
     * @endcode
     *
     */
    bool isFreedriveEnabled();

    /**
     * 是否使能了反向驱动模式
     *
     * @return 使能返回true; 反之返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isBackdriveEnabled(self: pyaubo_sdk.RobotManage) -> bool
     *
     * @par Lua函数原型
     * isBackdriveEnabled() -> boolean
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * bool isEnabled =
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->isBackdriveEnabled();
     * @endcode
     */
    bool isBackdriveEnabled();

    /**
     * 是否使能了仿真模式
     *
     * @return 使能返回true; 反之返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isSimulationEnabled(self: pyaubo_sdk.RobotManage) -> bool
     *
     * @par Lua函数原型
     * isSimulationEnabled() -> boolean
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * bool isEnabled =
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->isSimulationEnabled();
     * @endcode
     */
    bool isSimulationEnabled();

    /**
     * 清除防护停机，包括碰撞停机
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setUnlockProtectiveStop(self: pyaubo_sdk.RobotManage) -> int
     *
     * @par Lua函数原型
     * setUnlockProtectiveStop() -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->setUnlockProtectiveStop();
     * @endcode
     */
    int setUnlockProtectiveStop();

    /**
     * 重置安全接口板，一般在机器人断电之后需要重置时调用，比如机器人急停、故障等之后
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * restartInterfaceBoard(self: pyaubo_sdk.RobotManage) -> int
     *
     * @par Lua函数原型
     * restartInterfaceBoard() -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->restartInterfaceBoard();
     * @endcode
     */
    int restartInterfaceBoard();

    /**
     * 开始实时轨迹的记录
     *
     * @param file_name
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
    int startRecord(const std::string &file_name);

    /**
     * 停止实时记录
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int stopRecord();

    /**
     * 暂停实时记录
     *
     * @param pause
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int pauseRecord(bool pause);

    /**
     * 发起机器人进入/退出联动模式请求,
     * 只有操作模式为自动或者无时，才能使能联动模式
     *
     * @param enable
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_BUSY
     * AUBO_REQUEST_IGNORE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setLinkModeEnable(self: pyaubo_sdk.RobotManage, arg0: bool) -> int
     *
     * @par Lua函数原型
     * setLinkModeEnable(enable: boolean) -> number
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->setLinkModeEnable(true);
     * @endcode
     *
     */
    int setLinkModeEnable(bool enable);

    /**
     * 是否使能了联动模式，联动模式下用户可以通过外部IO控制机器人（用户可以对IO的功能进行配置）
     *
     * @return 使能返回true; 反之返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isLinkModeEnabled(self: pyaubo_sdk.RobotManage) -> bool
     *
     * @par Lua函数原型
     * isLinkModeEnabled() -> boolean
     *
     * @par C++示例
     * @code
     * auto rpc_cli = std::make_shared<RpcClient>();
     * auto robot_name = rpc_cli->getRobotNames().front();
     * bool isEnabled =
     * rpc_cli->getRobotInterface(robot_name)->getRobotManage()->isLinkModeEnabled();
     * @endcode
     */
    bool isLinkModeEnabled();

protected:
    void *d_;
};
using RobotManagePtr = std::shared_ptr<RobotManage>;

// clang-format off
#define RobotManage_DECLARES                         \
    _FUNC(RobotManage, 0, poweron)                   \
    _FUNC(RobotManage, 0, startup)                   \
    _FUNC(RobotManage, 0, poweroff)                  \
    _FUNC(RobotManage, 1, backdrive, enable)         \
    _FUNC(RobotManage, 1, freedrive, enable)         \
    _FUNC(RobotManage, 2, handguideMode, freeAxes,feature) \
    _FUNC(RobotManage, 0, exitHandguideMode)         \
    _FUNC(RobotManage, 0, getHandguideStatus)        \
    _FUNC(RobotManage, 0, getHandguideTrigger)       \
    _FUNC(RobotManage, 1, setSim, enable)            \
    _FUNC(RobotManage, 1, setOperationalMode, mode)  \
    _FUNC(RobotManage, 0, getOperationalMode)        \
    _FUNC(RobotManage, 0, getRobotControlMode)       \
    _FUNC(RobotManage, 0, isSimulationEnabled)       \
    _FUNC(RobotManage, 0, isFreedriveEnabled)        \
    _FUNC(RobotManage, 0, isBackdriveEnabled)        \
    _FUNC(RobotManage, 0, setUnlockProtectiveStop)   \
    _INST(RobotManage, 1, startRecord, file_name)    \
    _INST(RobotManage, 0, stopRecord)                \
    _INST(RobotManage, 1, pauseRecord, pause)        \
    _FUNC(RobotManage, 0, restartInterfaceBoard)     \
    _FUNC(RobotManage, 1, setLinkModeEnable, enable) \
    _FUNC(RobotManage, 0, isLinkModeEnabled)
// clang-format on
} // namespace common_interface
} // namespace arcs

#endif // AUBO_SDK_ROBOT_CONTROL_INTERFACE_H
