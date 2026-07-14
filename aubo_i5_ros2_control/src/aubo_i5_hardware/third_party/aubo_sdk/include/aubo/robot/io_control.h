/** @file  io_control.h
 *  @brief IO控制接口
 */
#ifndef AUBO_SDK_IO_CONTROL_INTERFACE_H
#define AUBO_SDK_IO_CONTROL_INTERFACE_H

#include <vector>
#include <thread>

#include <aubo/global_config.h>
#include <aubo/type_def.h>

namespace arcs {
namespace common_interface {

/**
 * IoControl类提供了一系列的接口对机器人标配的一些数字、模拟IO进行配置，输出状态设置、读取
 *
 * 1. 获取各种IO的数量 \n
 * 2. 配置IO的输入输出功能 \n
 * 3. 可配置IO的配置 \n
 * 4. 模拟IO的输入输出范围设置、读取
 *
 * 标准数字输入输出：控制柜IO面板上的标准IO \n
 * 工具端数字输入输出：通过工具末端航插暴露的数字IO \n
 * 可配置输入输出：可以配置为安全IO或者普通数字IO \n
 */
class ARCS_ABI_EXPORT IoControl
{
public:
    IoControl();
    virtual ~IoControl();

    /**
     * 获取标准数字输入数量
     *
     * @return 标准数字输入数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardDigitalInputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStandardDigitalInputNum() -> number
     *
     */
    int getStandardDigitalInputNum();

    /**
     * 获取工具端数字IO数量(包括数字输入和数字输出)
     *
     * @return 工具端数字IO数量(包括数字输入和数字输出)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolDigitalInputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getToolDigitalInputNum() -> number
     *
     */
    int getToolDigitalInputNum();

    /**
     * 获取可配置数字输入数量
     *
     * @return 可配置数字输入数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getConfigurableDigitalInputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getConfigurableDigitalInputNum() -> number
     *
     */
    int getConfigurableDigitalInputNum();

    /**
     * 获取标准数字输出数量
     *
     * @return 标准数字输出数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardDigitalOutputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStandardDigitalOutputNum() -> number
     *
     */
    int getStandardDigitalOutputNum();

    /**
     * 获取工具端数字IO数量(包括数字输入和数字输出)
     *
     * @return 工具端数字IO数量(包括数字输入和数字输出)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolDigitalOutputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getToolDigitalOutputNum() -> number
     *
     */
    int getToolDigitalOutputNum();

    /**
     * 设置指定的工具端数字IO为输入或输出
     *
     * 工具端数字IO比较特殊，IO可以配置为输入或者输出
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param input: 表示指定IO是否为输入。
     * input 为true时，设置指定IO为输入，否则为输出
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolIoInput(self: pyaubo_sdk.IoControl, arg0: int, arg1: bool) -> int
     *
     * @par Lua函数原型
     * setToolIoInput(index: number, input: boolean) -> nil
     *
     */
    int setToolIoInput(int index, bool input);

    /**
     * 判断指定的工具端数字IO类型是否为输入
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 当指定的IO为输入时返回 true, 否则为 false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isToolIoInput(self: pyaubo_sdk.IoControl, arg0: int) -> bool
     *
     * @par Lua函数原型
     * isToolIoInput(index: number) -> boolean
     *
     */
    bool isToolIoInput(int index);

    /**
     * 获取可配置数字输出数量
     *
     * @return 可配置数字输出数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getConfigurableDigitalOutputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getConfigurableDigitalOutputNum() -> number
     *
     */
    int getConfigurableDigitalOutputNum();

    /**
     * 获取标准模拟输入数量
     *
     * @return 标准模拟输入数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardAnalogInputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStandardAnalogInputNum() -> number
     *
     */
    int getStandardAnalogInputNum();

    /**
     * 获取工具端模拟输入数量
     *
     * @return 工具端模拟输入数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolAnalogInputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getToolAnalogInputNum() -> number
     *
     */
    int getToolAnalogInputNum();

    /**
     * 获取标准模拟输出数量
     *
     * @return 标准模拟输出数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardAnalogOutputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStandardAnalogOutputNum() -> number
     *
     */
    int getStandardAnalogOutputNum();

    /**
     * 获取工具端模拟输出数量
     *
     * @return 工具端模拟输出数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolAnalogOutputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getToolAnalogOutputNum() -> number
     *
     */
    int getToolAnalogOutputNum();

    /**
     * 设置所有数字输入动作为无触发
     *
     * @note
     * 当输入动作为无触发时，用户设置数字输入值为高电平，不会触发机器人发生动作
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setDigitalInputActionDefault(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * setDigitalInputActionDefault() -> nil
     *
     */
    int setDigitalInputActionDefault();

    /**
     * 设置标准数字输入触发动作
     *
     * @note
     * 当给输入设置为无触发动作(StandardInputAction::Default)时，
     * 用户设置数字输入值为高电平，不会触发机器人发生动作。\n
     * 当给输入设置触发动作时，用户设置数字输入值为高电平，会触发机器人执行相应的动作。\n
     * 例如，当设置DI0的触发动作为拖动示教(StandardInputAction::Handguide)时，
     * 用户设置DI0为高电平，机器人会进入拖动示教。
     * 设置DI0为低电平，机器人会退出拖动示教。
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param action: 触发动作
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setStandardDigitalInputAction(self: pyaubo_sdk.IoControl, arg0: int,
     * arg1: arcs::common_interface::StandardInputAction) -> int
     *
     * @par Lua函数原型
     * setStandardDigitalInputAction(index: number, action: number) -> nil
     *
     */
    int setStandardDigitalInputAction(int index, StandardInputAction action);

    /**
     * 设置工具数字输入触发动作
     *
     * @note
     * 当给输入设置为无触发动作(StandardInputAction::Default)时，
     * 用户设置工具数字输入值为高电平，不会触发机器人发生动作。\n
     * 当给输入设置触发动作时，用户设置工具数字输入值为高电平，会触发机器人执行相应的动作。\n
     * 例如，当设置TOOL_IO[0]的类型为输入而且触发动作为拖动示教(StandardInputAction::Handguide)时，
     * 用户设置TOOL_IO[0]为高电平，机器人会进入拖动示教。
     * 设置TOOL_IO[0]为低电平，机器人会退出拖动示教。
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param action: 触发动作
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolDigitalInputAction(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * arcs::common_interface::StandardInputAction) -> int
     *
     * @par Lua函数原型
     * setToolDigitalInputAction(index: number, action: number) -> nil
     *
     */
    int setToolDigitalInputAction(int index, StandardInputAction action);

    /**
     * 设置可配置数字输入触发动作
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param action: 触发动作
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @note 需要将可配置输入的安全输入动作设置为
     * SafetyInputAction::Unassigned时这个函数的配置才会生效
     *
     * @par Python函数原型
     * setConfigurableDigitalInputAction(self: pyaubo_sdk.IoControl, arg0: int,
     * arg1: arcs::common_interface::StandardInputAction) -> int
     *
     * @par Lua函数原型
     * setConfigurableDigitalInputAction(index: number, action: number) -> nil
     *
     */
    int setConfigurableDigitalInputAction(int index,
                                          StandardInputAction action);

    /**
     * 获取标准数字输入触发动作
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 标准数字输入触发动作
     *
     * @par Python函数原型
     * getStandardDigitalInputAction(self: pyaubo_sdk.IoControl, arg0: int) ->
     * arcs::common_interface::StandardInputAction
     *
     * @par Lua函数原型
     * getStandardDigitalInputAction(index: number) -> number
     *
     */
    StandardInputAction getStandardDigitalInputAction(int index);

    /**
     * 获取工具端数字输入触发动作
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 工具端数字输入触发动作
     *
     * @par Python函数原型
     * getToolDigitalInputAction(self: pyaubo_sdk.IoControl, arg0: int) ->
     * arcs::common_interface::StandardInputAction
     *
     * @par Lua函数原型
     * getToolDigitalInputAction(index: number) -> number
     *
     */
    StandardInputAction getToolDigitalInputAction(int index);

    /**
     * 获取可配置数字输入的输入触发动作
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 返回输入触发动作
     *
     * @par Python函数原型
     * getConfigurableDigitalInputAction(self: pyaubo_sdk.IoControl, arg0: int)
     * -> arcs::common_interface::StandardInputAction
     *
     * @par Lua函数原型
     * getConfigurableDigitalInputAction(index: number) -> number
     *
     */
    StandardInputAction getConfigurableDigitalInputAction(int index);

    /**
     * 设置所有数字输出状态选择为无
     *
     * @note
     * 当给输出状态设置为无(StandardOutputRunState::None)时，
     * 用户可以设置数字输出值。\n
     * 当给输出设置状态时，用户不可设置数字输出值，控制器会自动设置数字输出值。\n
     * 例如，当设置DO0的输出状态为高电平指示正在拖动示教(StandardOutputRunState::Handguiding)时，
     * 机器人进入拖动示教，DO0会自动变为高电平。
     * 机器人退出拖动示教，DO0会自动变为低电平。
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setDigitalOutputRunstateDefault(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * setDigitalOutputRunstateDefault() -> nil
     *
     */
    int setDigitalOutputRunstateDefault();

    /**
     * 设置标准数字输出状态选择
     *
     * @note
     * 当给输出状态设置为无(StandardOutputRunState::None)时，
     * 用户可以设置数字输出值。\n
     * 当给输出设置状态时，用户不可设置数字输出值，控制器会自动设置数字输出值。\n
     * 例如，当设置DO0的输出状态为高电平指示正在拖动示教(StandardOutputRunState::Handguiding)时，
     * 机器人进入拖动示教，DO0会自动变为高电平。
     * 机器人退出拖动示教，DO0会自动变为低电平。
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param runstate: 输出状态选择
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setStandardDigitalOutputRunstate(self: pyaubo_sdk.IoControl, arg0: int,
     * arg1: arcs::common_interface::StandardOutputRunState) -> int
     *
     * @par Lua函数原型
     * setStandardDigitalOutputRunstate(index: number, runstate: number) -> nil
     *
     */
    int setStandardDigitalOutputRunstate(int index,
                                         StandardOutputRunState runstate);

    /**
     * 设置工具端数字输出状态选择
     *
     * @note
     * 当给输出状态设置为无(StandardOutputRunState::None)时，
     * 用户可以设置数字输出值。\n
     * 当给输出设置状态时，用户不可设置数字输出值，控制器会自动设置数字输出值。\n
     * 例如，当设置TOOL_IO[0]类型为输出且输出状态为高电平指示正在拖动示教(StandardOutputRunState::Handguiding)时，
     * 机器人进入拖动示教，TOOL_IO[0]会自动变为高电平。
     * 机器人退出拖动示教，TOOL_IO[0]会自动变为低电平。
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param runstate: 输出状态选择
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolDigitalOutputRunstate(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * arcs::common_interface::StandardOutputRunState) -> int
     *
     * @par Lua函数原型
     * setToolDigitalOutputRunstate(index: number, runstate: number) -> nil
     *
     */
    int setToolDigitalOutputRunstate(int index,
                                     StandardOutputRunState runstate);

    /**
     * 设置可配置数字输出状态选择
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param runstate: 输出状态选择
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setConfigurableDigitalOutputRunstate(self: pyaubo_sdk.IoControl, arg0:
     * int, arg1: arcs::common_interface::StandardOutputRunState) -> int
     *
     * @par Lua函数原型
     * setConfigurableDigitalOutputRunstate(index: number, runstate: number) ->
     * nil
     *
     */
    int setConfigurableDigitalOutputRunstate(int index,
                                             StandardOutputRunState runstate);

    /**
     * 获取标准数字输出状态选择
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 输出状态选择
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardDigitalOutputRunstate(self: pyaubo_sdk.IoControl, arg0: int)
     * -> arcs::common_interface::StandardOutputRunState
     *
     * @par Lua函数原型
     * getStandardDigitalOutputRunstate(index: number) -> number
     *
     */
    StandardOutputRunState getStandardDigitalOutputRunstate(int index);

    /**
     * 获取工具端数字输出状态选择
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 输出状态选择
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolDigitalOutputRunstate(self: pyaubo_sdk.IoControl, arg0: int) ->
     * arcs::common_interface::StandardOutputRunState
     *
     * @par Lua函数原型
     * getToolDigitalOutputRunstate(index: number) -> number
     *
     */
    StandardOutputRunState getToolDigitalOutputRunstate(int index);

    /**
     * 获取可配置数字输出状态选择
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 输出状态选择
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getConfigurableDigitalOutputRunstate(self: pyaubo_sdk.IoControl, arg0:
     * int)
     * -> arcs::common_interface::StandardOutputRunState
     *
     * @par Lua函数原型
     * getConfigurableDigitalOutputRunstate(index: number) -> number
     *
     */
    StandardOutputRunState getConfigurableDigitalOutputRunstate(int index);

    /**
     * 设置标准模拟输出状态选择
     *
     * @note
     * 当给输出状态设置为无(StandardOutputRunState::None)时，
     * 用户可以设置模拟输出值。\n
     * 当给输出设置状态时，用户不可设置模拟输出值，控制器会自动设置模拟输出值。\n
     * 例如，当设置AO0的输出状态为高电平指示正在拖动示教(StandardOutputRunState::Handguiding)时，
     * 机器人进入拖动示教，AO0的值会自动变为最大值。
     * 机器人退出拖动示教，AO0的值会自动变为0。
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param runstate: 输出状态选择
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setStandardAnalogOutputRunstate(self: pyaubo_sdk.IoControl, arg0: int,
     * arg1: arcs::common_interface::StandardOutputRunState) -> int
     *
     * @par Lua函数原型
     * setStandardAnalogOutputRunstate(index: number, runstate: number) -> nil
     *
     */
    int setStandardAnalogOutputRunstate(int index,
                                        StandardOutputRunState runstate);

    /**
     * 设置工具端模拟输出状态选择
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param runstate: 输出状态选择
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolAnalogOutputRunstate(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * arcs::common_interface::StandardOutputRunState) -> int
     *
     * @par Lua函数原型
     * setToolAnalogOutputRunstate(index: number, runstate: number) -> nil
     *
     */
    int setToolAnalogOutputRunstate(int index, StandardOutputRunState runstate);

    /**
     * 获取标准模拟输出状态选择
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 标准模拟输出状态选择
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardAnalogOutputRunstate(self: pyaubo_sdk.IoControl, arg0: int) ->
     * arcs::common_interface::StandardOutputRunState
     *
     * @par Lua函数原型
     * getStandardAnalogOutputRunstate(index: number) -> number
     *
     */
    StandardOutputRunState getStandardAnalogOutputRunstate(int index);

    /**
     * 获取工具端模拟输出状态选择
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 工具端模拟输出状态选择
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolAnalogOutputRunstate(self: pyaubo_sdk.IoControl, arg0: int) ->
     * arcs::common_interface::StandardOutputRunState
     *
     * @par Lua函数原型
     * getToolAnalogOutputRunstate(index: number) -> number
     *
     */
    StandardOutputRunState getToolAnalogOutputRunstate(int index);

    /**
     * 设置标准模拟输入的范围
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param domain: 输入的范围
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setStandardAnalogInputDomain(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * int) -> int
     *
     * @par Lua函数原型
     * setStandardAnalogInputDomain(index: number, domain: number) -> nil
     *
     */
    int setStandardAnalogInputDomain(int index, int domain);

    /**
     * 设置工具端模拟输入的范围
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param domain: 输入的范围
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolAnalogInputDomain(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * int) -> int
     *
     * @par Lua函数原型
     * setToolAnalogInputDomain(index: number, domain: number) -> nil
     *
     */
    int setToolAnalogInputDomain(int index, int domain);

    /**
     * 获取标准模式输入范围
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 标准模式输入范围
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardAnalogInputDomain(self: pyaubo_sdk.IoControl, arg0: int) ->
     * int
     *
     * @par Lua函数原型
     * getStandardAnalogInputDomain(index: number) -> number
     *
     */
    int getStandardAnalogInputDomain(int index);

    /**
     * 获取工具端模式输入范围
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 工具端模式输入范围
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolAnalogInputDomain(self: pyaubo_sdk.IoControl, arg0: int) -> int
     *
     * @par Lua函数原型
     * getToolAnalogInputDomain(index: number) -> number
     *
     */
    int getToolAnalogInputDomain(int index);

    /**
     * 设置标准模拟输出的范围
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param domain: 输出的范围
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setStandardAnalogOutputDomain(self: pyaubo_sdk.IoControl, arg0: int,
     * arg1: int) -> int
     *
     * @par Lua函数原型
     * setStandardAnalogOutputDomain(index: number, domain: number) -> nil
     *
     */
    int setStandardAnalogOutputDomain(int index, int domain);

    /**
     * 设置工具端模拟输出范围
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param domain: 输出的范围
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolAnalogOutputDomain(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * int) -> int
     *
     * @par Lua函数原型
     * setToolAnalogOutputDomain(index: number, domain: number) -> nil
     *
     */
    int setToolAnalogOutputDomain(int index, int domain);

    /**
     * 获取标准模拟输出范围
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 标准模拟输出范围
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardAnalogOutputDomain(self: pyaubo_sdk.IoControl, arg0: int) ->
     * int
     *
     * @par Lua函数原型
     * getStandardAnalogOutputDomain(index: number) -> number
     *
     */
    int getStandardAnalogOutputDomain(int index);

    /**
     * 获取工具端模拟输出范围
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @return 工具端模拟输出范围
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolAnalogOutputDomain(self: pyaubo_sdk.IoControl, arg0: int) -> int
     *
     * @par Lua函数原型
     * getToolAnalogOutputDomain(index: number) -> number
     *
     */
    int getToolAnalogOutputDomain(int index);

    /**
     * 设置工具端电源电压值(单位V)
     *
     * @param domain: 工具端电源电压值，可选三个档位，分别为0、12和24。\n
     *  0表示0V, 12表示12V, 24表示24V。
     *
     * @return 成功返回0; 失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolVoltageOutputDomain(self: pyaubo_sdk.IoControl, arg0: int) -> int
     *
     * @par Lua函数原型
     * setToolVoltageOutputDomain(domain: number) -> nil
     *
     */
    int setToolVoltageOutputDomain(int domain);

    /**
     * 获取工具端电源电压值(单位V)
     *
     * @return 工具端电源电压值(单位V)
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolVoltageOutputDomain(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getToolVoltageOutputDomain() -> number
     *
     */
    int getToolVoltageOutputDomain();

    /**
     * 设置标准数字输出值
     *
     * @param index:  表示IO口的管脚，

     * @param value: 输出值
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setStandardDigitalOutput(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * bool) -> int
     *
     * @par Lua函数原型
     * setStandardDigitalOutput(index: number, value: boolean) -> nil
     *
     */
    int setStandardDigitalOutput(int index, bool value);

    /**
     * 设置数字输出脉冲
     *
     * @param index
     * @param value
     * @param duration
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setStandardDigitalOutputPulse(self: pyaubo_sdk.IoControl, arg0: int,
     * arg1: bool, arg2: float) -> int
     *
     * @par Lua函数原型
     * setStandardDigitalOutputPulse(index: number, value: boolean, duration:
     * number) -> nil
     *
     */
    int setStandardDigitalOutputPulse(int index, bool value, double duration);

    /**
     * 设置工具端数字输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param value: 数字输出值
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolDigitalOutput(self: pyaubo_sdk.IoControl, arg0: int, arg1: bool)
     * -> int
     *
     * @par Lua函数原型
     * setToolDigitalOutput(index: number, value: boolean) -> nil
     *
     */
    int setToolDigitalOutput(int index, bool value);

    /**
     * 设置工具端数字输出脉冲
     *
     * @param index
     * @param value
     * @param duration
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolDigitalOutputPulse(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * bool, arg2: float) -> int
     *
     * @par Lua函数原型
     * setToolDigitalOutputPulse(index: number, value: boolean, duration:
     * number) -> nil
     *
     */
    int setToolDigitalOutputPulse(int index, bool value, double duration);

    /**
     * 设置可配置数字输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param value: 数字输出值
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setConfigurableDigitalOutput(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * bool) -> int
     *
     * @par Lua函数原型
     * setConfigurableDigitalOutput(index: number, value: boolean) -> nil
     *
     */
    int setConfigurableDigitalOutput(int index, bool value);

    /**
     * 设置可配置数字输出脉冲
     *
     * @param index
     * @param value
     * @param duration
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setConfigurableDigitalOutputPulse(self: pyaubo_sdk.IoControl, arg0: int,
     * arg1: bool, arg2: float) -> int
     *
     * @par Lua函数原型
     * setConfigurableDigitalOutputPulse(index: number, value: boolean,
     * duration: number) -> nil
     *
     */
    int setConfigurableDigitalOutputPulse(int index, bool value,
                                          double duration);

    /**
     * 设置标准模拟输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param value: 模拟输出值
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setStandardAnalogOutput(self: pyaubo_sdk.IoControl, arg0: int, arg1:
     * float) -> int
     *
     * @par Lua函数原型
     * setStandardAnalogOutput(index: number, value: number) -> nil
     *
     */
    int setStandardAnalogOutput(int index, double value);

    /**
     * 设置工具端模拟输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     * @param value: 模拟输出
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_REQUEST_IGNORE
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setToolAnalogOutput(self: pyaubo_sdk.IoControl, arg0: int, arg1: float)
     * -> int
     *
     * @par Lua函数原型
     * setToolAnalogOutput(index: number, value: number) -> nil
     *
     */
    int setToolAnalogOutput(int index, double value);

    /**
     * 获取标准数字输入值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 高电平返回true; 低电平返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardDigitalInput(self: pyaubo_sdk.IoControl, arg0: int) -> bool
     *
     * @par Lua函数原型
     * getStandardDigitalInput(index: number) -> boolean
     *
     */
    bool getStandardDigitalInput(int index);

    /**
     * 获取所有的标准数字输入值
     *
     * @return 所有的标准数字输入值 \n
     * 例如，当返回值是2863267846时,换成2进制后是10101010101010100000000000000110。
     * 后16位就是所有的标准数字输入状态值，
     * 最后一位表示DI00的输入状态值,倒数第二位表示DI01的输入状态值，以此类推。\n
     * 1表示高电平状态，0表示低电平状态
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardDigitalInputs(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStandardDigitalInputs() -> number
     *
     */
    uint32_t getStandardDigitalInputs();

    /**
     * 获取工具端数字输入值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 高电平返回true; 低电平返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolDigitalInput(self: pyaubo_sdk.IoControl, arg0: int) -> bool
     *
     * @par Lua函数原型
     * getToolDigitalInput(index: number) -> boolean
     *
     */
    bool getToolDigitalInput(int index);

    /**
     *  获取所有的工具端数字输入值
     *
     * @return 返回所有的工具端数字输入值
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolDigitalInputs(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getToolDigitalInputs() -> number
     *
     */
    uint32_t getToolDigitalInputs();

    /**
     * 获取可配置数字输入值
     *
     * @note 可用于获取安全IO的输入值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 高电平返回true; 低电平返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getConfigurableDigitalInput(self: pyaubo_sdk.IoControl, arg0: int) ->
     * bool
     *
     * @par Lua函数原型
     * getConfigurableDigitalInput(index: number) -> boolean
     *
     */
    bool getConfigurableDigitalInput(int index);

    /**
     * 获取所有的可配置数字输入值
     *
     * @note 可用于获取安全IO的输入值
     *
     * @return 所有的可配置数字输入值\n
     * 例如，当返回值是2863267846时,换成2进制后是10101010101010100000000000000110。
     * 后16位就是所有的输入状态值，
     * 最后一位表示管脚0的输入状态值,倒数第二位表示管脚1的输入状态值，以此类推。\n
     * 1表示高电平状态，0表示低电平状态
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getConfigurableDigitalInputs(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getConfigurableDigitalInputs() -> number
     *
     */
    uint32_t getConfigurableDigitalInputs();

    /**
     * 获取标准数字输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 高电平返回true; 低电平返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardDigitalOutput(self: pyaubo_sdk.IoControl, arg0: int) -> bool
     *
     * @par Lua函数原型
     * getStandardDigitalOutput(index: number) -> boolean
     *
     */
    bool getStandardDigitalOutput(int index);

    /**
     * 获取所有的标准数字输出值
     *
     * @return 所有的标准数字输出值 \n
     * 例如，当返回值是2863267846时,换成2进制后是10101010101010100000000000000110。
     * 后16位就是所有的标准数字输出状态值，
     * 最后一位表示DI00的输出状态值,倒数第二位表示DI01的输出状态值，以此类推。\n
     * 1表示高电平状态，0表示低电平状态.
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardDigitalOutputs(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStandardDigitalOutputs() -> number
     *
     */
    uint32_t getStandardDigitalOutputs();

    /**
     * 获取工具端数字输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 高电平返回true; 低电平返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolDigitalOutput(self: pyaubo_sdk.IoControl, arg0: int) -> bool
     *
     * @par Lua函数原型
     * getToolDigitalOutput(index: number) -> boolean
     *
     */
    bool getToolDigitalOutput(int index);

    /**
     * 获取所有的工具端数字输出值
     *
     * @return 所有的工具端数字输出值
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolDigitalOutputs(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getToolDigitalOutputs() -> number
     *
     */
    uint32_t getToolDigitalOutputs();

    /**
     * 获取可配值数字输出值
     *
     * @note 可用于获取安全IO的输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 高电平返回true; 低电平返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getConfigurableDigitalOutput(self: pyaubo_sdk.IoControl, arg0: int) ->
     * bool
     *
     * @par Lua函数原型
     * getConfigurableDigitalOutput(index: number) -> boolean
     *
     */
    bool getConfigurableDigitalOutput(int index);

    /**
     * 获取所有的可配值数字输出值
     *
     * @note 可用于获取安全IO的输出值
     *
     * @return 所有的可配值数字输出\n
     * 例如，当返回值是2863267846时,换成2进制后是10101010101010100000000000000110。
     * 后16位就是所有的输出值，
     * 最后一位表示管脚0的输出状态值,倒数第二位表示管脚1的输出状态值，以此类推。\n
     * 1表示高电平状态，0表示低电平状态.
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getConfigurableDigitalOutputs(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getConfigurableDigitalOutputs() -> number
     *
     */
    uint32_t getConfigurableDigitalOutputs();

    /**
     * 获取标准模拟输入值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 标准模拟输入值
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardAnalogInput(self: pyaubo_sdk.IoControl, arg0: int) -> float
     *
     * @par Lua函数原型
     * getStandardAnalogInput(index: number) -> number
     *
     */
    double getStandardAnalogInput(int index);

    /**
     * 获取工具端模拟输入值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 工具端模拟输入值
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolAnalogInput(self: pyaubo_sdk.IoControl, arg0: int) -> float
     *
     * @par Lua函数原型
     * getToolAnalogInput(index: number) -> number
     *
     */
    double getToolAnalogInput(int index);

    /**
     * 获取标准模拟输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 标准模拟输出值
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStandardAnalogOutput(self: pyaubo_sdk.IoControl, arg0: int) -> float
     *
     * @par Lua函数原型
     * getStandardAnalogOutput(index: number) -> number
     *
     */
    double getStandardAnalogOutput(int index);

    /**
     * 获取工具端模拟输出值
     *
     * @param index: 表示IO口的管脚，管脚编号从0开始。
     * 例如，0表示第一个管脚。
     *
     * @return 工具端模拟输出值
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolAnalogOutput(self: pyaubo_sdk.IoControl, arg0: int) -> float
     *
     * @par Lua函数原型
     * getToolAnalogOutput(index: number) -> number
     *
     */
    double getToolAnalogOutput(int index);

    /**
     * 获取联动输入数量
     *
     * @return 联动输入数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStaticLinkInputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStaticLinkInputNum() -> number
     *
     */
    int getStaticLinkInputNum();

    /**
     * 获取联动输出数量
     *
     * @return 联动输出数量
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStaticLinkOutputNum(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStaticLinkOutputNum() -> number
     *
     */
    int getStaticLinkOutputNum();

    /**
     * 获取所有的联动输入值
     *
     * @return 所有的联动输入值\n
     * 例如，当返回值是2863267846时,换成2进制后是10101010101010100000000000000110。
     * 后16位就是所有的联动输入状态值，
     * 最后一位表示管脚0的输入状态值,倒数第二位表示管脚1的输入状态值，以此类推。\n
     * 1表示高电平状态，0表示低电平状态.
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStaticLinkInputs(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStaticLinkInputs() -> number
     *
     */
    uint32_t getStaticLinkInputs();

    /**
     * 获取所有的联动输出值
     *
     * @return 返回所有的联动输出值 \n
     * 例如，当返回值是2863267846时,换成2进制后是10101010101010100000000000000110。
     * 后16位就是所有的联动输出状态值，
     * 最后一位表示管脚0的输出状态值,倒数第二位表示管脚1的输出状态值，以此类推。\n
     * 1表示高电平状态，0表示低电平状态.
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getStaticLinkOutputs(self: pyaubo_sdk.IoControl) -> int
     *
     * @par Lua函数原型
     * getStaticLinkOutputs() -> number
     *
     */
    uint32_t getStaticLinkOutputs();

    /**
     * 机器人是否配置了编码器
     *
     * @return 机器人配置编码器返回 true, 反之返回 false
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    bool hasEncoderSensor();

    /**
     * 设置编码器的解码方式
     *
     * @param type
     * @param range_id
     * 0 is a 32 bit signed encoder, range [-2147483648, 2147483647]
     * 1 is a 8 bit unsigned encoder, range [0, 255]
     * 2 is a 16 bit unsigned encoder, range [0, 65535]
     * 3 is a 24 bit unsigned encoder, range [0, 16777215]
     * 4 is a 32 bit unsigned encoder, range [0, 4294967295]
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_NO_ACCESS
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setEncDecoderType(int type, int range_id);

    /**
     * 设置脉冲数
     *
     * @param rick
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_NO_ACCESS
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setEncTickCount(int tick);

    /**
     * 获取编码器的解码方式
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_NO_ACCESS
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int getEncDecoderType();

    /**
     * 获取脉冲数
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_NO_ACCESS
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int getEncTickCount();

    /**
     * 防止在计数超出范围时计数错误
     *
     * @param delta_count
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_NO_ACCESS
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int unwindEncDeltaTickCount(int delta_count);

    /**
     * 获取末端按钮状态
     *
     * @return 按下返回true; 否则返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getToolButtonStatus() -> bool
     *
     * @par Lua函数原型
     * getToolButtonStatus() -> boolean
     *
     */
    bool getToolButtonStatus();

protected:
    void *d_;
};
using IoControlPtr = std::shared_ptr<IoControl>;

// clang-format off
#define IoControl_DECLARES                                                   \
    _FUNC(IoControl, 0, getStandardDigitalInputNum)                          \
    _FUNC(IoControl, 0, getToolDigitalInputNum)                              \
    _FUNC(IoControl, 0, getConfigurableDigitalInputNum)                      \
    _FUNC(IoControl, 0, getStandardDigitalOutputNum)                         \
    _FUNC(IoControl, 0, getToolDigitalOutputNum)                             \
    _INST(IoControl, 2, setToolIoInput, index, input)                        \
    _FUNC(IoControl, 1, isToolIoInput, index)                                \
    _FUNC(IoControl, 0, getConfigurableDigitalOutputNum)                     \
    _FUNC(IoControl, 0, getStandardAnalogInputNum)                           \
    _FUNC(IoControl, 0, getToolAnalogInputNum)                               \
    _FUNC(IoControl, 0, getStandardAnalogOutputNum)                          \
    _FUNC(IoControl, 0, getToolAnalogOutputNum)                              \
    _INST(IoControl, 0, setDigitalInputActionDefault)                        \
    _INST(IoControl, 2, setStandardDigitalInputAction, index, action)        \
    _INST(IoControl, 2, setToolDigitalInputAction, index, action)            \
    _INST(IoControl, 2, setConfigurableDigitalInputAction, index, action)    \
    _FUNC(IoControl, 1, getStandardDigitalInputAction, index)                \
    _FUNC(IoControl, 1, getToolDigitalInputAction, index)                    \
    _FUNC(IoControl, 1, getConfigurableDigitalInputAction, index)            \
    _INST(IoControl, 0, setDigitalOutputRunstateDefault)                     \
    _INST(IoControl, 2, setStandardDigitalOutputRunstate, index, runstate)   \
    _INST(IoControl, 2, setToolDigitalOutputRunstate, index, runstate)       \
    _INST(IoControl, 2, setConfigurableDigitalOutputRunstate, index, runstate) \
    _FUNC(IoControl, 1, getStandardDigitalOutputRunstate, index)             \
    _FUNC(IoControl, 1, getToolDigitalOutputRunstate, index)                 \
    _FUNC(IoControl, 1, getConfigurableDigitalOutputRunstate, index)         \
    _INST(IoControl, 2, setStandardAnalogOutputRunstate, index, runstate)    \
    _INST(IoControl, 2, setToolAnalogOutputRunstate, index, runstate)        \
    _FUNC(IoControl, 1, getStandardAnalogOutputRunstate, index)              \
    _FUNC(IoControl, 1, getToolAnalogOutputRunstate, index)                  \
    _INST(IoControl, 2, setStandardAnalogInputDomain, index, domain)         \
    _INST(IoControl, 2, setToolAnalogInputDomain, index, domain)             \
    _FUNC(IoControl, 1, getStandardAnalogInputDomain, index)                 \
    _FUNC(IoControl, 1, getToolAnalogInputDomain, index)                     \
    _INST(IoControl, 2, setStandardAnalogOutputDomain, index, domain)        \
    _INST(IoControl, 2, setToolAnalogOutputDomain, index, domain)            \
    _INST(IoControl, 1, setToolVoltageOutputDomain, domain)                  \
    _FUNC(IoControl, 0, getToolVoltageOutputDomain)                          \
    _FUNC(IoControl, 1, getStandardAnalogOutputDomain, index)                \
    _FUNC(IoControl, 1, getToolAnalogOutputDomain, index)                    \
    _INST(IoControl, 2, setStandardDigitalOutput, index, value)              \
    _INST(IoControl, 3, setStandardDigitalOutputPulse, index, value, duration) \
    _INST(IoControl, 2, setToolDigitalOutput, index, value)                  \
    _INST(IoControl, 3, setToolDigitalOutputPulse, index, value, duration)   \
    _INST(IoControl, 2, setConfigurableDigitalOutput, index, value)          \
    _INST(IoControl, 3, setConfigurableDigitalOutputPulse, index, value, duration) \
    _INST(IoControl, 2, setStandardAnalogOutput, index, value)               \
    _INST(IoControl, 2, setToolAnalogOutput, index, value)                   \
    _FUNC(IoControl, 1, getStandardDigitalInput, index)                      \
    _FUNC(IoControl, 0, getStandardDigitalInputs)                            \
    _FUNC(IoControl, 1, getToolDigitalInput, index)                          \
    _FUNC(IoControl, 0, getToolDigitalInputs)                                \
    _FUNC(IoControl, 1, getConfigurableDigitalInput, index)                  \
    _FUNC(IoControl, 0, getConfigurableDigitalInputs)                        \
    _FUNC(IoControl, 1, getStandardAnalogInput, index)                       \
    _FUNC(IoControl, 1, getToolAnalogInput, index)                           \
    _FUNC(IoControl, 1, getStandardDigitalOutput, index)                     \
    _FUNC(IoControl, 0, getStandardDigitalOutputs)                           \
    _FUNC(IoControl, 1, getToolDigitalOutput, index)                         \
    _FUNC(IoControl, 0, getToolDigitalOutputs)                               \
    _FUNC(IoControl, 1, getConfigurableDigitalOutput, index)                 \
    _FUNC(IoControl, 0, getConfigurableDigitalOutputs)                       \
    _FUNC(IoControl, 1, getStandardAnalogOutput, index)                      \
    _FUNC(IoControl, 1, getToolAnalogOutput, index)                          \
    _FUNC(IoControl, 0, getStaticLinkInputNum)                               \
    _FUNC(IoControl, 0, getStaticLinkOutputNum)                              \
    _FUNC(IoControl, 0, getStaticLinkInputs)                                 \
    _FUNC(IoControl, 0, getStaticLinkOutputs)                                \
    _FUNC(IoControl, 0, hasEncoderSensor)                                    \
    _FUNC(IoControl, 2, setEncDecoderType, type, range_id)                   \
    _FUNC(IoControl, 1, setEncTickCount, tick)                               \
    _FUNC(IoControl, 0, getEncDecoderType)                                   \
    _FUNC(IoControl, 0, getEncTickCount)                                     \
    _FUNC(IoControl, 1, unwindEncDeltaTickCount, delta_count)                \
    _FUNC(IoControl, 0, getToolButtonStatus)

// clang-format on
} // namespace common_interface
} // namespace arcs

#endif // AUBO_SDK_IO_CONTROL_INTERFACE_H
