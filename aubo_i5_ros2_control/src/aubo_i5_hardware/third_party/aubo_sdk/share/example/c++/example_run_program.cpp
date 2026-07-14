#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void exampleRunProgram(RpcClientPtr cli)
{
    // 示教器工程文件名
    std::string program_name = "test";
    // 接口调用: 加载示教器编程的.pro工程
    // 工程文件需要放在在/root/arcs_ws/program目录下
    // 只需要输入文件名，不需要添加后缀
    cli->getRuntimeMachine()->loadProgram(program_name);
    // 接口调用: 运行工程
    cli->getRuntimeMachine()->runProgram();

    RuntimeState program_status;
    while (1) {
        // 接口调用: 获取工程运行状态
        program_status = cli->getRuntimeMachine()->getRuntimeState();
        std::cout << "当前工程运行状态：" << program_status << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
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

    // 运行示教器工程
    exampleRunProgram(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
