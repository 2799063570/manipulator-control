#include <iostream>
#include <thread>
#include <chrono>
#include "aubo_sdk/rpc.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void robotMotionControl(RpcClientPtr cli)
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

    std::vector<double> joint_angle4 = {
        41.04 * (M_PI / 180), -27.03 * (M_PI / 180), 115.35 * (M_PI / 180),
        52.37 * (M_PI / 180), 90.0 * (M_PI / 180),   11.64 * (M_PI / 180)
    };

    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 开启运行时(RuntimeMachine)
    cli->getRuntimeMachine()->start();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(1);

    // 接口调用: 获取运行时的状态
    auto runtime_status = cli->getRuntimeMachine()->getRuntimeState();

    while (runtime_status != RuntimeState::Stopped) {
        // 接口调用: 关节运动
        robot_interface->getMotionControl()->moveJoint(
            joint_angle1, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        // 接口调用: 关节运动
        robot_interface->getMotionControl()->moveJoint(
            joint_angle2, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        // 接口调用: 关节运动
        robot_interface->getMotionControl()->moveJoint(
            joint_angle3, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        // 接口调用: 关节运动
        robot_interface->getMotionControl()->moveJoint(
            joint_angle4, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        // 接口调用: 获取运行时(RuntimeMachine)状态
        runtime_status = cli->getRuntimeMachine()->getRuntimeState();
    }

    // 接口调用: 停止关节运动
    robot_interface->getMotionControl()->stopJoint(30);
}

// 控制暂停/恢复/停止
void controlOperations(RpcClientPtr rpc_cli, bool &exit_flag)
{
    std::string input;
    while (!exit_flag) {
        std::cout
            << "请输入命令(p/r/s): p表示暂停运动，r表示恢复运动，s表示停止运动"
            << std::endl;

        std::cin >> input;

        if (input == "p") {
            // 接口调用: 暂停运行时
            rpc_cli->getRuntimeMachine()->pause();
            std::cout << "已暂停运行时" << std::endl;
        } else if (input == "r") {
            // 接口调用: 恢复运行时
            rpc_cli->getRuntimeMachine()->resume();
            std::cout << "已恢复运行时" << std::endl;
        } else if (input == "s") {
            // 接口调用: 停止运行时
            // 注意: abort并不能停止机械臂运动
            rpc_cli->getRuntimeMachine()->abort();
            exit_flag = true; // 设置退出标志以结束线程
            std::cout << "已停止运行时" << std::endl;
        } else {
            std::cout << "无效命令，请重新输入" << std::endl;
        }
    }
}

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    auto rpc_cli = std::make_shared<RpcClient>();
    // 接口调用: 设置 RPC 超时
    rpc_cli->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc_cli->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    // 退出线程标志
    bool exit_flag = false;

    // 创建控制机器人运动和操作的线程
    std::thread motion_thread(robotMotionControl, rpc_cli);
    std::thread control_thread(controlOperations, rpc_cli, std::ref(exit_flag));

    // 等待线程完成
    motion_thread.join();
    control_thread.join();

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
