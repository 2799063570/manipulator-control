/** @file  trace.h
 *  @brief 向控制器日志系统注入日志方面的接口
 */
#ifndef AUBO_SDK_TRACE_INTERFACE_H
#define AUBO_SDK_TRACE_INTERFACE_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>

#include <aubo/global_config.h>
#include <aubo/type_def.h>

namespace arcs {
namespace common_interface {

/**
 * 提供给控制器扩展程序的日志记录系统
 */
class ARCS_ABI_EXPORT Trace
{
public:
    Trace();
    virtual ~Trace();

    /**
     * 向 aubo_control 日志注入告警信息
     *
     * TraceLevel: \n
     *  0 - FATAL \n
     *  1 - ERROR \n
     *  2 - WARNING \n
     *  3 - INFO \n
     *  4 - DEBUG \n
     *
     * code定义参考 error_stack
     *
     * @param level
     * @param code
     * @param args
     * @return
     *
     * @par Python函数原型
     * alarm(self: pyaubo_sdk.Trace, arg0: arcs::common_interface::TraceLevel,
     * arg1: int, arg2: List[str]) -> int
     *
     * @par Lua函数原型
     * alarm(level: number, code: number, args: table) -> nil
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.Trace.alarm","params":["",1,["Error","Trajectory
     * planning failed!","1"]],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int alarm(TraceLevel level, int code,
              const std::vector<std::string> &args = {});

    /**
     * 打印文本信息到日志中
     *
     * @param msg 文本信息
     * @return
     *
     * @par Python函数原型
     * textmsg(self: pyaubo_sdk.Trace, arg0: str) -> int
     *
     * @par Lua函数原型
     * textmsg(msg: string) -> nil
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.Trace.textmsg","params":["test"],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int textmsg(const std::string &msg);

    /**
     * 通知上位机
     *
     * @param msg
     * @return
     */
    int notify(const std::string &msg);

    /**
     * 向连接的 RTDE 客户端发送弹窗请求
     *
     * @param level
     * @param title
     * @param msg
     * @param mode 模式 \n
     *   0: 普通模式 \n
     *   1: 阻塞模式 \n
     *   2: 输入模式 bool \n
     *   3: 输入模式 int \n
     *   4: 输入模式 double \n
     *   5: 输入模式 string \n
     * @return
     *
     * @par Python函数原型
     * popup(self: pyaubo_sdk.Trace, arg0: arcs::common_interface::TraceLevel,
     * arg1: str, arg2: str, arg3: int) -> int
     *
     * @par Lua函数原型
     * popup(level: number, title: string, msg: string, mode: number) -> nil
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.Trace.popup","params":["","Error","Trajectory
     * planning failed!",1],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":0}
     *
     */
    int popup(TraceLevel level, const std::string &title,
              const std::string &msg, int mode);

    /**
     * peek最新的 AlarmInfo(上次一获取之后)
     *
     * last_time设置为0时，可以获取到所有的AlarmInfo
     *
     * @param num
     * @param last_time
     * @return
     *
     * @par Python函数原型
     * peek(self: pyaubo_sdk.Trace, arg0: int, arg1: int) ->
     * List[arcs::common_interface::RobotMsg]
     *
     * @par Lua函数原型
     * peek(num: number, last_time: number) -> table
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"rob1.Trace.peek","params":[1,0],"id":1}
     *
     * @par JSON-RPC响应示例
     * {{"id":1,"jsonrpc":"2.0","result":[{"args":["RobotModeType.Running"],
     * "code":30045,"level":"INFO","source":"rob1","timestamp":5102883064300}]}
     *
     */
    RobotMsgVector peek(size_t num, uint64_t last_time = 0);

protected:
    void *d_;
};

using TracePtr = std::shared_ptr<Trace>;

// clang-format off
#define Trace_DECLARES                               \
    _INST(Trace, 3, alarm, level, code, args)        \
    _INST(Trace, 4, popup, level, title, msg, mode)  \
    _INST(Trace, 1, textmsg, msg)                    \
    _INST(Trace, 1, notify, msg)                     \
    _FUNC(Trace, 2, peek, num, last_time)
// clang-format on
} // namespace common_interface
} // namespace arcs
#endif
