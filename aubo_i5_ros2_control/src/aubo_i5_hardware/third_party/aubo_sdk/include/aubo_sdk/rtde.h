/** @file  rtde.h
 *  @brief 用于RPC模块的交互，如订阅、发布等功能
 */
#ifndef AUBO_SDK_RTDE_H
#define AUBO_SDK_RTDE_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <map>
#include <unordered_map>

#include <aubo/type_def.h>
#include <aubo/global_config.h>

namespace arcs {
namespace aubo_sdk {

class RtdeClient;

/// 向输出数据中增加
class ARCS_ABI OutputBuilder
{
public:
    OutputBuilder();
    ~OutputBuilder();

    OutputBuilder &push(int val);
    OutputBuilder &push(double val);
    OutputBuilder &push(const std::vector<double> &val);
    OutputBuilder &push(const std::tuple<int, bool> &val);
    OutputBuilder &push(int16_t &val);
    OutputBuilder &push(const std::vector<int16_t> &val);
    OutputBuilder &push(const std::vector<int> &val);
    OutputBuilder &push(const std::string &val);
    OutputBuilder &push(char val);
    OutputBuilder &push(const common_interface::RtdeRecipe &val);

private:
    friend RtdeClient;
    class Impl;
    Impl *impl;
};

/// 解析输入
class ARCS_ABI InputParser
{
public:
    InputParser();
    ~InputParser();

    bool popBool();
    int popInt32();
    int64_t popInt64();
    int16_t popInt16();
    double popDouble();
    char popChar();
    std::vector<int> popVectorInt();
    std::vector<int16_t> popVectorInt16();
    std::vector<double> popVectorDouble();
    std::vector<std::vector<double>> popVectorVectorDouble();
    std::vector<common_interface::JointStateType> popVectorJointStateType();
    common_interface::RobotModeType popRobotModeType();
    common_interface::OperationalModeType popOperationalModeType();
    common_interface::SafetyModeType popSafetyModeType();
    common_interface::RuntimeState popRuntimeState();
    common_interface::RobotMsgVector popRobotMsgVector();
    common_interface::Payload popPayload();

private:
    friend RtdeClient;
    class Impl;
    Impl *impl;
};

/// RTDE客户端
class ARCS_ABI RtdeClient
{
public:
    enum Event
    {
        Connected,
        Disconnected,
    };

    /**
     * RTDE客户端初始化
     *
     * @param mode 设置通讯方式， 0 tcp通讯 1 uds通讯
     */
    RtdeClient(int mode = 0);
    ~RtdeClient();

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
     * @param port 端口号，RTDE 端口号为30010
     * @retval 0 连接成功
     * @retval 1 在执行函数前，已连接
     * @retval -1 连接失败
     */
    int connect(const std::string &ip = "", int port = 0);

    /**
     * socket 是否已连接
     *
     * @retval true 已连接socket
     * @retval false 未连接socket
     */
    bool hasConnected() const;

    /**
     * socket 是否已连接
     *
     * @param callback
     * @retval true 已连接socket
     * @retval false 未连接socket
     */
    bool hasConnected1(std::function<void(bool)> callback);

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
     * 是否已经登录
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
     * 获取协议版本号
     *
     * @return 协议版本号
     */
    int getProtocolVersion();

    /**
     * 获取输入列表
     *
     * @return 输入列表
     */
    std::map<std::string, int> getInputMaps();

    /**
     * 获取输出列表
     *
     * @return 输出列表
     */
    std::map<std::string, int> getOutputMaps();

    /**
     * 设置话题
     *
     * @param to_server 数据流向。
     * true 表示客户端给服务器发送消息，false 表示服务器给客户端发送消息
     * @param names 服务器推送的信息列表
     * @param freq 服务器推送信息的频率
     * @param expected_chanel 通道。
     * 取值范围：0~99，
     * 发布不同的话题走不同的通道
     * @retval expected_chanel参数的值 成功
     * @retval -1 失败
     */
    int setTopic(bool to_server, const std::vector<std::string> &names,
                 double freq, int expected_chanel);

    /**
     * 取消订阅
     *
     * @param to_server 数据流向
     * true 表示客户端给服务器发送消息，false 表示服务器给客户端发送消息
     * @param chanel 通道
     * @retval 0 成功
     * @retval 1 失败
     */
    int removeTopic(bool to_server, int chanel);

    /**
     * 获取已注册的输入菜单
     *
     * @return 已注册的输入菜单
     */
    std::unordered_map<int, common_interface::RtdeRecipe>
    getRegisteredInputRecipe();

    /**
     * 获取已注册的输出菜单
     *
     * @return 已注册的输出菜单
     */
    std::unordered_map<int, common_interface::RtdeRecipe>
    getRegisteredOutputRecipe();

    /**
     * 订阅 subscribe from output
     *
     * @param chanel 通道
     * @param callback 回调函数，用于处理订阅的输入信息。\n
     * 回调函数的定义如下：
     * void callback(InputParser &parser)
     * @return 0
     */
    int subscribe(int chanel, std::function<void(InputParser &)> callback);

    /**
     * 发布 publish to input
     *
     * @param chanel 通道
     * @param callback 回调函数，用于构建发布的输出信息。\n
     * 回调函数的定义如下：
     * void callback(OutputBuilder &builder)
     * @retval 0 成功
     * @retval -1 失败
     */
    int publish(int chanel, std::function<void(OutputBuilder &)> callback);

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
using RtdeClientPtr = std::shared_ptr<RtdeClient>;

} // namespace aubo_sdk
} // namespace arcs

#ifdef __cplusplus
extern "C" {
#endif

ARCS_ABI arcs::aubo_sdk::RtdeClient *createRtdeClient(int mode = 0);
ARCS_ABI void destroyRtdeClient(arcs::aubo_sdk::RtdeClient *cli);

#ifdef __cplusplus
}
#endif
#endif
