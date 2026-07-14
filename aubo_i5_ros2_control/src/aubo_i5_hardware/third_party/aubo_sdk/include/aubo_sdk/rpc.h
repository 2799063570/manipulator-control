/** @file  rpc.h
 *  @brief 用于RPC模块的交互，如登录、连接等功能
 */

#ifndef AUBO_SDK_RPC_H
#define AUBO_SDK_RPC_H

#include <memory>

#include <aubo/aubo_api.h>
#include <aubo/global_config.h>

namespace arcs {
namespace aubo_sdk {
using namespace arcs::common_interface;

/// RPC客户端
class ARCS_ABI RpcClient : public AuboApi
{
public:
    enum Event
    {
        Connected,
        Disconnected,
    };

    /**
     * @brief RpcClient
     * @param mode 0-TCP 1-UDS
     */
    RpcClient(int mode = 0);
    ~RpcClient();

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
     * 连接到RPC服务
     *
     * @param ip IP地址
     * @param port 端口号，RPC的端口号是30004
     * @param ip和port为空时，采用unix domain sockets通讯方式
     * @retval 0 RPC连接成功
     * @retval -8 RPC连接失败，RPC连接被拒绝
     * @retval -15 RPC连接失败，SDK版本与Server版本不兼容
     */
    int connect(const std::string &ip = "", int port = 0);

    /**
     * 断开RPC连接
     *
     * @retval 0 成功
     * @retval -1 失败
     */
    int disconnect();

    /**
     * 判断是否连接RPC
     *
     * @retval true 已连接RPC
     * @retval false 未连接RPC
     */
    bool hasConnected() const;

    /**
     * 登录
     *
     * @param usrname 用户名
     * @param passwd 密码
     * @return 0
     */
    int login(const std::string &usrname, const std::string &passwd);

    /**
     * 登出
     *
     * @return 0
     */
    int logout();

    /**
     * 判断是否登录
     *
     * @retval true 已登录
     * @retval false 未登录
     */
    bool hasLogined();

    /**
     * 设置RPC请求超时时间
     *
     * @param timeout 请求超时时间，单位 ms
     * @return 0
     */
    int setRequestTimeout(int timeout = 10);

    /**
     * 设置事件处理
     *
     * @param cb
     * @return
     */
    int setEventHandler(std::function<void(int /*event*/)> cb);

    /**
     * 是否关闭异常抛出
     *
     * @param enable
     * @return
     */
    int setExceptionFree(bool enable);

    /**
     * 返回错误代码
     *
     * @return
     */
    int errorCode() const;
};
using RpcClientPtr = std::shared_ptr<RpcClient>;

} // namespace aubo_sdk
} // namespace arcs

#ifdef __cplusplus
extern "C" {
#endif

ARCS_ABI arcs::aubo_sdk::RpcClient *createRpcClient(int mode = 0);
ARCS_ABI void destroyRpcClient(arcs::aubo_sdk::RpcClient *cli);

#ifdef __cplusplus
}
#endif
#endif
