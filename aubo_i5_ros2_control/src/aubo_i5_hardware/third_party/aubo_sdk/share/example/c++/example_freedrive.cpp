#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::aubo_sdk;

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif
    // RPC 客户端对象
    auto rpc_cli = std::make_shared<arcs::aubo_sdk::RpcClient>();
    // 接口调用: 设置 RPC 超时
    rpc_cli->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc_cli->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");
    // 接口调用: 获取机器人的名字
    auto robot_name = rpc_cli->getRobotNames().front();
    auto impl = rpc_cli->getRobotInterface(robot_name);

    // 接口调用: 使能拖动示教模式
    impl->getRobotManage()->freedrive(true);

    // 等待进入拖动示教
    int i = 0;
    while (!impl->getRobotManage()->isFreedriveEnabled()) {
        if (i++ > 5) {
            std::cerr << "使能拖动示教模式失败" << std::endl;
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    std::cout << "使能拖动示教模式成功" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(25));

    // 接口调用: 退出拖动示教模式
    impl->getRobotManage()->freedrive(false);

    // 等待退出拖动示教模式
    while (impl->getRobotManage()->isFreedriveEnabled()) {
        if (i++ > 5) {
            std::cerr << "拖动示教模式失能失败" << std::endl;
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "退出拖动示教模式成功" << std::endl;

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
