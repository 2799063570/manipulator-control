#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

// 消除弹窗示例1：通过alarm接口消除弹窗
void examplePopupDismiss1(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 消除所有的弹窗
    robot_interface->getTrace()->alarm(TraceLevel::INFO, 2, { "" });
}

// 消除弹窗示例2：通过IO输入消除弹窗
// 注意：要使IO生效,必须让机械臂进入联动模式
void examplePopupDismiss2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 设置DI00的动作为消除弹窗。
    StandardInputAction input_action = StandardInputAction::PopupDismiss;
    robot->getIoControl()->setStandardDigitalInputAction(0, input_action);

    std::cout << "说明:当DI00为高电平(DI00和0V短接)时,消除弹窗。" << std::endl;
}

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    auto rpc_cli = std::make_shared<RpcClient>();
    // 接口调用: 设置 RPC 超时
    rpc_cli->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc_cli->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    // 消除弹窗示例1：通过alarm接口消除弹窗
    examplePopupDismiss1(rpc_cli);

    // 消除弹窗示例2：通过IO输入消除弹窗
    // 注意：要使IO生效,必须让机械臂进入联动模式
    // examplePopupDismiss2(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
