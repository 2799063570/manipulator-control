#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

// 等待机械臂进入目标模式
void waitForRobotMode(RobotInterfacePtr robot_interface,
                      RobotModeType target_mode)
{
    // 接口调用: 获取当前机械臂的模式
    auto current_mode = robot_interface->getRobotState()->getRobotModeType();

    while (current_mode != target_mode) {
        std::cout << "机械臂当前模式:" << current_mode << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        current_mode = robot_interface->getRobotState()->getRobotModeType();
    }
}

void exampleStartup(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置负载
    double mass = 0.0;
    std::vector<double> cog(3, 0.0);
    std::vector<double> aom(3, 0.0);
    std::vector<double> inertia(6, 0.0);
    robot_interface->getRobotConfig()->setPayload(mass, cog, aom, inertia);

    // 接口调用: 获取机械臂当前模式
    auto robot_mode = robot_interface->getRobotState()->getRobotModeType();

    if (robot_mode == RobotModeType::Running) {
        std::cout << "机械臂已松刹车，处于运行模式" << std::endl;

    } else {
        // 接口调用: 机械臂发起上电请求
        robot_interface->getRobotManage()->poweron();

        // 等待机械臂进入空闲模式
        waitForRobotMode(robot_interface, RobotModeType::Idle);

        std::cout << "机械臂上电成功，当前模式:"
                  << robot_interface->getRobotState()->getRobotModeType()
                  << std::endl;

        // 接口调用: 机械臂发起松刹车请求
        cli->getRobotInterface(robot_name)->getRobotManage()->startup();

        // 等待机械臂进入运行模式
        waitForRobotMode(robot_interface, RobotModeType::Running);

        std::cout << "机械臂松刹车成功，当前模式:"
                  << robot_interface->getRobotState()->getRobotModeType()
                  << std::endl;
    }
}

void examplePoweroff(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 机械臂断电
    robot_interface->getRobotManage()->poweroff();

    // 等待机械臂进入断电模式
    waitForRobotMode(robot_interface, RobotModeType::PowerOff);

    std::cout << "机械臂断电成功，当前模式:"
              << robot_interface->getRobotState()->getRobotModeType()
              << std::endl;
}

/**
 * 功能: 机械臂上电与断电
 * 步骤:
 * 第一步: 设置 RPC 超时、连接到 RPC 服务、登录
 * 第二步: 设置负载、上电和松刹车
 * 第三步: 机械臂断电
 * 第四步: RPC 退出登录、断开连接
 */

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

    // 机械臂上电
    exampleStartup(rpc_cli);

    // 机械臂断电
    //    examplePoweroff(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
