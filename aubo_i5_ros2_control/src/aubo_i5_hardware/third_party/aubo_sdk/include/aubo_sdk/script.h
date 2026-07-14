/** @file  script.h
 *  @brief 用于SCRIPT模块的交互，如向服务器发送脚本
 */
#ifndef AUBO_SDK_SCRIPT_H
#define AUBO_SDK_SCRIPT_H

#include <string>
#include <memory>
#include <functional>
#include <aubo/global_config.h>

namespace arcs {
namespace aubo_sdk {

class ScriptWriter
{
public:
    virtual ~ScriptWriter() = default;

    virtual ScriptWriter &append(const std::string &line) = 0;
    virtual ScriptWriter &append(const char *buf, size_t len) = 0;
    virtual ScriptWriter &moveJoint() = 0;
    virtual ScriptWriter &moveLine() = 0;
    virtual ScriptWriter &ifCondition() = 0;
    virtual ScriptWriter &elseCondition() = 0;
    virtual ScriptWriter &elseIfCondition() = 0;
    virtual ScriptWriter &whileCondition() = 0;
    virtual ScriptWriter &end() = 0;
};

/// SCRIPT客户端
class ARCS_ABI ScriptClient
{
public:
    enum Event
    {
        Connected,
        Disconnected,
    };

    ScriptClient(int mode = 0);
    ~ScriptClient();

    /**
     * 设置日志处理器
     *
     * 此函数可设置自定义的日志处理函数来处理日志消息。 \n
     * Aubo SDK 有一套默认的日志系统，按照默认的格式输出到默认的文件。
     * 如果用户不希望采用默认的格式或者不希望输出到默认的文件，那就可以通过这个接口重新自定义格式，或者输出路径。
     * 这个函数可以将用户自定义的日志系统与 Aubo SDK 默认的日志系统合并。
     *
     * @note setLogHandler函数要放在即将触发的日志之前，
     * 否则会按照默认的形式输出日志。
     *
     * @param handler 日志处理函数 \n
     * 此日志处理函数的下定义如下: \n
     * void handler(int level, const char* filename, int line, const
     * std::string& message) \n
     * level 表示日志等级 \n
     * &nbsp; 0: LOGLEVEL_FATAL 严重的错误 \n
     * &nbsp; 1: LOGLEVEL_ERROR      错误 \n
     * &nbsp; 2: LOGLEVEL_WARNING    警告 \n
     * &nbsp; 3: LOGLEVEL_INFO       通知 \n
     * &nbsp; 4: LOGLEVEL_DEBUG      调试 \n
     * &nbsp; 5: LOGLEVEL_BACKTRACE  跟踪 \n
     * filename 表示文件名 \n
     * line 表示代码行号 \n
     * message 表示日志信息 \n
     * @return 无
     */
    void setLogHandler(
        std::function<void(int /*level*/, const char * /*filename*/,
                           int /*line*/, const std::string & /*message*/)>
            handler);

    /**
     * 连接到服务器
     *
     * @param ip IP地址
     * @param port 端口号。SCRIPT端口号为30004
     * @retval 0 连接成功
     * @retval 1 在执行函数前，已连接
     * @retval -1 连接失败
     */
    int connect(const std::string &ip = "", int port = 0);

    /**
     * 是否处于连接状态
     *
     * @retval true 已连接
     * @retval false 未连接
     */
    bool hasConnected() const;

    /**
     * 登录
     *
     * @param usrname 用户名
     * @param passwd 密码
     * @retval 0 成功
     * @retval -1 失败
     */
    int login(const std::string &usrname, const std::string &passwd);

    /**
     * 返回客户端是否登录
     *
     * @retval true 已登录
     * @retval false 未登录
     */
    bool hasLogined();

    /**
     * 登出
     *
     * @return 0
     */
    int logout();

    /**
     * 断开连接
     *
     * @retval 0 成功
     * @retval -1 失败
     */
    int disconnect();

    /**
     * 发送脚本文件
     *
     * 远程调用机器人的脚本
     *
     * @param path 文件在机器人端的路径
     * @retval 0 成功
     * @retval -1 失败
     */
    int sendFile(const std::string &path);

    /**
     * 发送脚本内容
     *
     * 调用本地的脚本
     *
     * @param script 脚本内容
     * @retval 0 成功
     * @retval -1 失败
     */
    int sendString(const std::string &script);

    /**
     * 使用ScriptWriter构建服务器脚本
     *
     * @param chunck_name
     * @param cb
     * @retval 0 成功
     * @retval -1 失败
     */
    int send(const std::string &chunck_name,
             std::function<int(ScriptWriter &)> cb);

    /**
     * 设置服务器脚本的全局变量
     *
     * @param cb 脚本的全局变量
     * @return 0
     */
    int subscribeVariableUpdate(
        std::function<void(const std::string &, const std::string &)> cb);

    /**
     * 设置服务器脚本的错误码
     *
     * @param cb 脚本的错误码
     * @return 0
     */
    [[deprecated]] int subscribeScriptError(
        std::function<void(const std::string &)> cb);

    int subscribeScriptError2(
        std::function<void(const std::string &, const std::string &)> cb);

    /**
     * 设置事件处理
     *
     * @param cb
     * @return
     */
    int setEventHandler(std::function<void(int /*event*/)> cb);

private:
    class Impl;
    Impl *impl;
};
using ScriptClientPtr = std::shared_ptr<ScriptClient>;

} // namespace aubo_sdk
} // namespace arcs

#ifdef __cplusplus
extern "C" {
#endif

ARCS_ABI arcs::aubo_sdk::ScriptClient *createScriptClient(int mode = 0);
ARCS_ABI void destroyScriptClient(arcs::aubo_sdk::ScriptClient *cli);

#ifdef __cplusplus
}
#endif
#endif
