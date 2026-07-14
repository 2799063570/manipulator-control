/** @file  runtime_machine.h
 *  @brief 脚本解释器运行时接口，
 *  可以实现脚本解释器的暂停、脚本解释器的设置/取消断点
 */
#ifndef AUBO_SDK_RUNTIME_MACHINE_INTERFACE_H
#define AUBO_SDK_RUNTIME_MACHINE_INTERFACE_H

#include <memory>
#include <aubo/global_config.h>
#include <aubo/type_def.h>

namespace arcs {
namespace common_interface {

/**
 * The RuntimeMachine class
 */
class ARCS_ABI_EXPORT RuntimeMachine
{
public:
    RuntimeMachine();
    virtual ~RuntimeMachine();

    /**
     * 返回 task_id
     */
    int newTask(bool daemon = false);

    /**
     * 删除 task，会终止正在执行的运动
     */
    int deleteTask(int tid);

    /**
     * 等待 task 自然结束
     *
     * @param tid
     * @return
     */
    int detachTask(int tid);

    /**
     * 判断任务是否存活
     *
     * @param tid
     * @return
     */
    bool isTaskAlive(int tid);

    /**
     * 切换当前线程，切换之后接下来的指令将被插入切换后的线程中
     *
     * @param tid
     * @return
     */
    int switchTask(int tid);

    /**
     * 标记记下来的指令的行号和注释
     *
     * @param lineno
     * @param comment
     * @return
     */
    int setLabel(int lineno, const std::string &comment);

    /**
     * 向aubo_control日志中添加注释
     * 使用 setLabel 替换
     *
     * @param tid 指令的线程ID
     * @param lineno 行号
     * @param comment 注释
     * @return
     *
     * @par Python函数原型
     * setPlanContext(self: pyaubo_sdk.RuntimeMachine, arg0: int, arg1: int,
     * arg2: str) -> int
     *
     * @par Lua函数原型
     * setPlanContext(tid: number, lineno: number, comment: string) -> number
     *
     */
    ARCS_DEPRECATED int setPlanContext(int tid, int lineno,
                                       const std::string &comment);

    /**
     * 空操作
     *
     * @return
     */
    int nop();

    /**
     * 获取耗时的接口执行状态, 如 setPersistentParameters
     *
     * @return 指令名字, 执行状态
     * 执行状态: EXECUTING/FINISHED
     *
     * @par Python函数原型
     * getExecutionStatus(self: pyaubo_sdk.RuntimeMachine) -> Tuple[str, str]
     *
     * @par Lua函数原型
     * getExecutionStatus() -> string
     *
     */
    std::tuple<std::string, std::string> getExecutionStatus();

    /**
     * 跳转到指定行号
     *
     * @param lineno
     * @return
     *
     * @par Python函数原型
     * gotoLine(self: pyaubo_sdk.RuntimeMachine, arg0: int) -> int
     *
     * @par Lua函数原型
     * gotoLine(lineno: number) -> number
     *
     */
    int gotoLine(int lineno);

    /**
     * 获取当前运行上下文
     *
     * @param tid 任务编号
     * 如果指定(不是-1)，返回对应任务的运行上下文；如果不指定(是-1)，返回正在运行的线程的运行上下文
     *
     * @return
     *
     * @par Python函数原型
     * getPlanContext(self: pyaubo_sdk.RuntimeMachine) -> Tuple[int, int, str]
     *
     * @par Lua函数原型
     * getPlanContext() -> number
     *
     */
    std::tuple<int, int, std::string> getPlanContext(int tid = -1);

    /**
     * 获取提前运行规划器的上下文信息
     *
     * @param tid 任务编号
     * 如果指定(不是-1)，返回对应任务运行规划器的上下文信息；如果不指定(是-1)，返回正在运行的线程运行规划器的上下文信息
     *
     * @return
     */
    std::tuple<int, int, std::string> getAdvancePlanContext(int tid = -1);

    /**
     * 获取AdvanceRun的程序指针
     *
     * @return
     */
    int getAdvancePtr(int tid = -1);

    /**
     * 获取机器人运动的程序指针
     *
     * @param tid 任务编号
     * 如果指定(不是-1)，返回对应任务的程序指针；如果不指定(是-1)，返回正在运行线程的程序指针
     *
     * @return
     */
    int getMainPtr(int tid = -1);

    /**
     * 获取最近解释过的指令指针
     *
     * @param tid
     * @return
     */
    int getInterpPtr(int tid);

    /**
     * 加载本地工程文件
     * Lua 脚本，只需要给出文件名字，不需要后缀，需要从 ${ARCS_WS}/program
     * 目录中查找
     *
     * @param program
     * @return
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"RuntimeMachine.loadProgram","params":["demo"],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int loadProgram(const std::string &program);

    /**
     * 运行已经加载的工程文件
     *
     * @return
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"RuntimeMachine.runProgram","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int runProgram();

    /**
     * 开始运行时
     *
     * @return
     *
     * @par Python函数原型
     * start(self: pyaubo_sdk.RuntimeMachine) -> int
     *
     * @par Lua函数原型
     * start() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"RuntimeMachine.start","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int start();

    /**
     * 停止运行时即脚本运行，无法停止运行时状态为 Stopped 时的机器人运动
     *
     * 如果考虑停止机器人所有运动，可以调用 RuntimeMachine::abort 接口
     *
     * @return
     *
     * @par Python函数原型
     * stop(self: pyaubo_sdk.RuntimeMachine) -> int
     *
     * @par Lua函数原型
     * stop() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"RuntimeMachine.stop","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int stop();

    /**
     * 终止机器人运行.
     *
     * 如果只是考虑停止运行时，可以调用 RuntimeMachine::stop 接口
     *
     * 如果脚本运行时处于 Running 状态，则终止运行时；如果运行时处于 Stopped
     * 且机器人正在移动，则停止机器人移动；如果此时力控开启了，则机器人停止力控
     *
     * @return
     *
     * @par Python函数原型
     * abort(self: pyaubo_sdk.RuntimeMachine) -> int
     *
     * @par Lua函数原型
     * abort() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"RuntimeMachine.abort","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int abort();

    /**
     * 暂停解释器
     *
     * @return
     *
     * @par Python函数原型
     * pause(self: pyaubo_sdk.RuntimeMachine) -> int
     *
     * @par Lua函数原型
     * pause() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"RuntimeMachine.pause","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int pause();

    /**
     * 单步运行
     *
     * @return
     *
     * @par Python函数原型
     * step(self: pyaubo_sdk.RuntimeMachine) -> int
     *
     * @par Lua函数原型
     * step() -> number
     *
     */
    int step();

    /**
     * 恢复解释器
     *
     * @return
     *
     * @par Python函数原型
     * resume(self: pyaubo_sdk.RuntimeMachine) -> int
     *
     * @par Lua函数原型
     * resume() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"RuntimeMachine.resume","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int resume();

    /**
     * 恢复解释器之前等待恢复前之前的序列完成
     *
     * @param wait
     * @return
     */
    int setResumeWait(bool wait);

    /**
     * 获取规划器的状态
     *
     * @return
     *
     * @par Python函数原型
     * getStatus(self: pyaubo_sdk.RuntimeMachine) ->
     * arcs::common_interface::RuntimeState
     *
     * @par Lua函数原型
     * getStatus() -> number
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"RuntimeMachine.getStatus","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"Running"}
     *
     */
    ARCS_DEPRECATED RuntimeState getStatus();
    RuntimeState getRuntimeState();

    /**
     * 设置断点
     *
     * @param lineno
     * @return
     *
     * @par Python函数原型
     * setBreakPoint(self: pyaubo_sdk.RuntimeMachine, arg0: int) -> int
     *
     * @par Lua函数原型
     * setBreakPoint(lineno: number) -> number
     *
     */
    int setBreakPoint(int lineno);

    /**
     * 移除断点
     *
     * @param lineno
     * @return
     *
     * @par Python函数原型
     * removeBreakPoint(self: pyaubo_sdk.RuntimeMachine, arg0: int) -> int
     *
     * @par Lua函数原型
     * removeBreakPoint(lineno: number) -> number
     *
     */
    int removeBreakPoint(int lineno);

    /**
     * 清除所有断点
     *
     * @return
     *
     * @par Python函数原型
     * clearBreakPoints(self: pyaubo_sdk.RuntimeMachine) -> int
     *
     * @par Lua函数原型
     * clearBreakPoints() -> number
     *
     */
    int clearBreakPoints();

    /**
     * 定时器开始
     *
     * @param name
     * @return
     *
     * @par Python函数原型
     * timerStart(self: pyaubo_sdk.RuntimeMachine, arg0: str) -> int
     *
     * @par Lua函数原型
     * timerStart(name: string) -> nil
     *
     */
    int timerStart(const std::string &name);

    /**
     * 定时器结束
     *
     * @param name
     * @return
     *
     * @par Python函数原型
     * timerStop(self: pyaubo_sdk.RuntimeMachine, arg0: str) -> int
     *
     * @par Lua函数原型
     * timerStop(name: string) -> nil
     *
     */
    int timerStop(const std::string &name);

    /**
     * 定时器重置
     *
     * @param name
     * @return
     *
     * @par Python函数原型
     * timerReset(self: pyaubo_sdk.RuntimeMachine, arg0: str) -> int
     *
     * @par Lua函数原型
     * timerReset(name: string) -> nil
     *
     */
    int timerReset(const std::string &name);

    /**
     * 定时器删除
     *
     * @param name
     * @return
     *
     * @par Python函数原型
     * timerDelete(self: pyaubo_sdk.RuntimeMachine, arg0: str) -> int
     *
     * @par Lua函数原型
     * timerDelete(name: string) -> nil
     *
     */
    int timerDelete(const std::string &name);

    /**
     * 获取定时器数值
     *
     * @param name
     * @return
     *
     * @par Python函数原型
     * getTimer(self: pyaubo_sdk.RuntimeMachine, arg0: str) -> float
     *
     * @par Lua函数原型
     * getTimer(name: string) -> number
     *
     */
    double getTimer(const std::string &name);

    /**
     * 开始配置触发
     *
     * @param distance
     * @param delay
     * @return
     */
    int triggBegin(double distance, double delay);

    /**
     * 终止配置触发
     *
     * @return
     */
    int triggEnd();

    /**
     * 返回自动分配的中断号
     *
     * @param distance
     * @param delay
     * @param intnum
     * @return
     */
    int triggInterrupt(double distance, double delay);

    /**
     * 获取所有的中断号列表
     *
     * @return
     */
    std::vector<int> getTriggInterrupts();

protected:
    void *d_;
};

using RuntimeMachinePtr = std::shared_ptr<RuntimeMachine>;

// clang-format off
#define RuntimeMachine_DECLARES                                     \
    _FUNC(RuntimeMachine, 1, newTask, daemon)                       \
    _FUNC(RuntimeMachine, 1, deleteTask, tid)                       \
    _FUNC(RuntimeMachine, 1, detachTask, tid)                       \
    _FUNC(RuntimeMachine, 1, isTaskAlive, tid)                      \
    _INST(RuntimeMachine, 0, nop)                                   \
    _FUNC(RuntimeMachine, 1, switchTask, tid)                       \
    _FUNC(RuntimeMachine, 2, setLabel, tid, lineno)                 \
    _FUNC(RuntimeMachine, 3, setPlanContext, tid, lineno, comment)  \
    _FUNC(RuntimeMachine, 1, gotoLine, lineno)                      \
    _FUNC(RuntimeMachine, 1, getAdvancePlanContext, tid)            \
    _FUNC(RuntimeMachine, 1, getAdvancePtr, tid)                    \
    _FUNC(RuntimeMachine, 1, getMainPtr, tid)                       \
    _FUNC(RuntimeMachine, 1, getInterpPtr, tid)                     \
    _FUNC(RuntimeMachine, 1, getPlanContext, tid)                   \
    _FUNC(RuntimeMachine, 0, getExecutionStatus)                    \
    _FUNC(RuntimeMachine, 1, loadProgram, program)                  \
    _FUNC(RuntimeMachine, 0, runProgram)                            \
    _FUNC(RuntimeMachine, 0, start)                                 \
    _FUNC(RuntimeMachine, 0, stop)                                  \
    _FUNC(RuntimeMachine, 0, abort)                                 \
    _FUNC(RuntimeMachine, 0, pause)                                 \
    _FUNC(RuntimeMachine, 0, step)                                  \
    _FUNC(RuntimeMachine, 1, setResumeWait, wait)                   \
    _FUNC(RuntimeMachine, 0, resume)                                \
    _FUNC(RuntimeMachine, 0, getStatus)                             \
    _FUNC(RuntimeMachine, 0, getRuntimeState)                       \
    _FUNC(RuntimeMachine, 1, setBreakPoint, lineno)                 \
    _FUNC(RuntimeMachine, 1, removeBreakPoint, lineno)              \
    _FUNC(RuntimeMachine, 0, clearBreakPoints)                      \
    _INST(RuntimeMachine, 1, timerStart, name)                      \
    _INST(RuntimeMachine, 1, timerStop, name)                       \
    _INST(RuntimeMachine, 1, timerReset, name)                      \
    _INST(RuntimeMachine, 1, timerDelete, name)                     \
    _FUNC(RuntimeMachine, 1, getTimer, name)                        \
    _FUNC(RuntimeMachine, 2, triggBegin, distance, delay)           \
    _FUNC(RuntimeMachine, 0, triggEnd)                              \
    _FUNC(RuntimeMachine, 2, triggInterrupt, distance, delay)       \
    _FUNC(RuntimeMachine, 0, getTriggInterrupts)

// clang-format on
} // namespace common_interface
} // namespace arcs
#endif // AUBO_SDK_RUNTIME_MACHINE_H
