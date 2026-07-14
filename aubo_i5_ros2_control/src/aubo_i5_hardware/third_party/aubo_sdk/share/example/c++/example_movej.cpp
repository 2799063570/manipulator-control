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

void exampleMovej(RpcClientPtr cli)
{
    // 关节角，单位: 弧度
    std::vector<double> joint_angle1 = {
        0.0 * (M_PI / 180),  -15.0 * (M_PI / 180), 100.0 * (M_PI / 180),
        25.0 * (M_PI / 180), 90.0 * (M_PI / 180),  0.0 * (M_PI / 180)
    };

    std::vector<double> joint_angle2 = {
        35.92 * (M_PI / 180),  -11.28 * (M_PI / 180), 59.96 * (M_PI / 180),
        -18.76 * (M_PI / 180), 90.0 * (M_PI / 180),   35.92 * (M_PI / 180)
    };

    std::vector<double> joint_angle3 = {
        41.04 * (M_PI / 180), -7.65 * (M_PI / 180), 98.80 * (M_PI / 180),
        16.44 * (M_PI / 180), 90.0 * (M_PI / 180),  11.64 * (M_PI / 180)
    };

    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 关节运动
    robot_interface->getMotionControl()->moveJoint(
        joint_angle1, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);
    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到路点1成功" << std::endl;
    } else {
        std::cout << "关节运动到路点1失败" << std::endl;
    }

    // 接口调用: 关节运动
    robot_interface->getMotionControl()->moveJoint(
        joint_angle2, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);
    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到路点2成功" << std::endl;
    } else {
        std::cout << "关节运动到路点2失败" << std::endl;
    }

    // 接口调用: 关节运动
    robot_interface->getMotionControl()->moveJoint(
        joint_angle3, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);
    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到路点3成功" << std::endl;
    } else {
        std::cout << "关节运动到路点3失败" << std::endl;
    }
}

/**
 * 功能: 机械臂关节运动
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
 * 第二步: 设置运动速度比率、以关节运动的方式依次经过3个路点
 * 第三步: RPC 退出登录、断开连接
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

    // 关节运动
    exampleMovej(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
