#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrival(RobotInterfacePtr impl)
{
    const int max_retry_count = 5;
    int cnt = 0;

    // 接口调用: 获取当前的运动指令 ID
    int exec_id = impl->getMotionControl()->getExecId();

    // 等待机械臂开始运动
    while (exec_id == -1) {
        if (cnt++ > max_retry_count) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        exec_id = impl->getMotionControl()->getExecId();
    }

    // 等待机械臂动作完成
    while (impl->getMotionControl()->getExecId() != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}

// 示例1: TCP在基坐标系/用户坐标系下沿Z方向做偏移运动
void exampleMovelOffset1(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 接口调用: 获取TCP在Base坐标系下的当前位姿
    std::vector<double> current_tcp_on_base(6, 0.0);
    current_tcp_on_base = robot_interface->getRobotState()->getTcpPose();

    // 接口调用:获取TCP在基坐标系/用户坐标系下沿着Z+轴偏移0.1m的目标位姿（相对于基坐标系）
    std::vector<double> target_tcp_on_base(6, 0.0);
    std::vector<double> path_offset = { 0, 0, 0.1, 0, 0, 0 };
    target_tcp_on_base =
        cli->getMath()->poseAdd(current_tcp_on_base, path_offset);

    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.75);

    // 接口调用: 直线运动到目标位置
    robot_interface->getMotionControl()->moveLine(target_tcp_on_base, 1.2, 0.25,
                                                  0.025, 0);
    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "直线运动到目标位置成功！" << std::endl;
    } else {
        std::cout << "直线运动到目标位置失败！" << std::endl;
    }
}

// 示例2: TCP在工具坐标系下沿Z方向做偏移运动
void exampleMovelOffset2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 接口调用: 获取TCP在Base坐标系下的当前位姿
    std::vector<double> current_tcp_on_base(6, 0.0);
    current_tcp_on_base = robot_interface->getRobotState()->getTcpPose();

    // 接口调用:获取TCP在工具坐标系下沿着Z+轴偏移0.1m的目标位姿（相对于基坐标系）
    std::vector<double> target_tcp_on_base(6, 0.0);
    std::vector<double> path_offset = { 0, 0, 0.1, 0, 0, 0 };
    target_tcp_on_base =
        cli->getMath()->poseTrans(current_tcp_on_base, path_offset);

    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.75);

    // 接口调用: 直线运动到目标位置
    robot_interface->getMotionControl()->moveLine(target_tcp_on_base, 1.2, 0.25,
                                                  0.025, 0);
    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "直线运动到目标位置成功！" << std::endl;
    } else {
        std::cout << "直线运动到目标位置失败！" << std::endl;
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

    // 示例1: TCP在基坐标系/用户坐标系下沿Z方向做偏移运动
    exampleMovelOffset1(rpc_cli);

    // 示例2: TCP在工具坐标系下沿Z方向做偏移运动
    exampleMovelOffset2(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
