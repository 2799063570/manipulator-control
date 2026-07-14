#include "aubo_sdk/rpc.h"
#include <math.h>
#ifdef WIN32
#include <Windows.h>
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

// 圆弧运动
void exampleMoveArc(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂运动的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 设置工具中心点
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 关节角，单位: 弧度
    std::vector<double> joint_angle = { 0.0,
                                        -15.0 * (M_PI / 180),
                                        100.0 * (M_PI / 180),
                                        25.0 * (M_PI / 180),
                                        90.0 * (M_PI / 180),
                                        0.0 };
    // 接口调用: 关节运动到初始位置
    robot_interface->getMotionControl()->moveJoint(
        joint_angle, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);

    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到初始位置成功" << std::endl;
    } else {
        std::cout << "关节运动到初始位置失败" << std::endl;
    }

    // 位姿
    std::vector<double> pose1 = {
        0.440164, -0.119, 0.2976, 2.456512, 0, 1.5708
    };

    std::vector<double> pose2 = { 0.440164, -0.00249391, 0.398658,
                                  2.45651,  0,           1.5708 };

    std::vector<double> pose3 = { 0.440164, 0.166256, 0.297599,
                                  2.45651,  0,        1.5708 };

    // 接口调用: 直线运动到圆弧的起始点——pose1
    robot_interface->getMotionControl()->moveLine(pose1, 1.2, 0.25, 0.025, 0);

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "直线运动到圆弧的起始点成功" << std::endl;
    } else {
        std::cout << "直线运动到圆弧的起始点失败" << std::endl;
    }

    // 接口调用: 圆弧运动
    robot_interface->getMotionControl()->moveCircle(pose2, pose3, 1.2, 0.25, 0,
                                                    0);

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "圆弧运动成功" << std::endl;
    } else {
        std::cout << "圆弧运动失败" << std::endl;
    }
}

// 圆弧运动
void exampleMoveArc2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂运动的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 设置工具中心点
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 关节角，单位: 弧度
    std::vector<double> joint_angle = { 0.0,
                                        -15.0 * (M_PI / 180),
                                        100.0 * (M_PI / 180),
                                        25.0 * (M_PI / 180),
                                        90.0 * (M_PI / 180),
                                        0.0 };
    // 接口调用: 关节运动到初始位置
    robot_interface->getMotionControl()->moveJoint(
        joint_angle, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);

    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到初始位置成功" << std::endl;
    } else {
        std::cout << "关节运动到初始位置失败" << std::endl;
    }

    // 位姿
    std::vector<double> pose1 = {
        0.440164, -0.119, 0.2976, 2.456512, 0, 1.5708
    };

    std::vector<double> pose2 = { 0.440164, -0.00249391, 0.398658,
                                  2.45651,  0,           1.5708 };

    std::vector<double> pose3 = { 0.440164, 0.166256, 0.297599,
                                  2.45651,  0,        1.5708 };

    // 接口调用: 直线运动到圆弧的起始点——pose1
    robot_interface->getMotionControl()->moveLine(pose1, 1.2, 0.25, 0.025, 0);

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "直线运动到圆弧的起始点成功" << std::endl;
    } else {
        std::cout << "直线运动到圆弧的起始点失败" << std::endl;
    }

    CircleParameters param;
    param.pose_via = pose2;
    param.pose_to = pose3;
    param.a = 1.2;
    param.v = 0.25;

    // 接口调用: 圆弧运动
    robot_interface->getMotionControl()->moveCircle2(param);

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "圆弧运动成功" << std::endl;
    } else {
        std::cout << "圆弧运动失败" << std::endl;
    }
}

// 圆运动
void exampleMoveC(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂运动的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 设置工具中心点
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 关节角，单位: 弧度
    std::vector<double> joint_angle = { 0.0,
                                        -15.0 * (M_PI / 180),
                                        100.0 * (M_PI / 180),
                                        25.0 * (M_PI / 180),
                                        90.0 * (M_PI / 180),
                                        0.0 };
    // 接口调用: 关节运动到初始位置
    robot_interface->getMotionControl()->moveJoint(
        joint_angle, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);

    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到初始位置成功" << std::endl;
    } else {
        std::cout << "关节运动到初始位置失败" << std::endl;
    }

    // 位姿
    std::vector<double> pose1 = {
        0.440164, -0.119, 0.2976, 2.456512, 0, 1.5708
    };

    std::vector<double> pose2 = { 0.440164, -0.00249391, 0.398658,
                                  2.45651,  0,           1.5708 };

    std::vector<double> pose3 = { 0.440164, 0.166256, 0.297599,
                                  2.45651,  0,        1.5708 };

    // 接口调用: 直线运动到圆的起始点——pose1
    robot_interface->getMotionControl()->moveLine(pose1, 1.2, 0.25, 0.025, 0);

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "直线运动到圆的起始点成功" << std::endl;
    } else {
        std::cout << "直线运动到圆的起始点失败" << std::endl;
    }

    // 接口调用: 工具姿态相对于圆弧路径点坐标系保持不变
    robot_interface->getMotionControl()->setCirclePathMode(0);

    // 计算另一半圆弧的中间点
    auto result =
        cli->getMath()->calculateCircleFourthPoint(pose1, pose2, pose3, 1);

    if (std::get<1>(result) == 0) {
        std::cout << "计算另一半圆弧的中间点失败，无法完成圆运动" << std::endl;
    } else {
        auto pose4 = std::get<0>(result);
        // 圆运动执行3圈
        for (int i = 0; i < 3; i++) {
            // 接口调用: 圆弧运动
            robot_interface->getMotionControl()->moveCircle(pose2, pose3, 1.2,
                                                            0.25, 0, 0);

            // 阻塞
            ret = waitArrival(robot_interface);
            if (ret == -1) {
                std::cout << "圆运动失败" << std::endl;
            }

            // 执行另一半圆弧运动
            robot_interface->getMotionControl()->moveCircle(pose4, pose1, 1.2,
                                                            0.25, 0, 0);

            // 阻塞
            ret = waitArrival(robot_interface);
            if (ret == 0) {
                std::cout << "圆运动成功" << std::endl;
            } else {
                std::cout << "圆运动失败" << std::endl;
            }
        }
    }
}

// 圆运动
void exampleMoveC2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂运动的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 设置工具中心点
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 关节角，单位: 弧度
    std::vector<double> joint_angle = { 0.0,
                                        -15.0 * (M_PI / 180),
                                        100.0 * (M_PI / 180),
                                        25.0 * (M_PI / 180),
                                        90.0 * (M_PI / 180),
                                        0.0 };
    // 接口调用: 关节运动到初始位置
    robot_interface->getMotionControl()->moveJoint(
        joint_angle, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);

    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到初始位置成功" << std::endl;
    } else {
        std::cout << "关节运动到初始位置失败" << std::endl;
    }

    // 位姿
    std::vector<double> pose1 = {
        0.440164, -0.119, 0.2976, 2.456512, 0, 1.5708
    };

    std::vector<double> pose2 = { 0.440164, -0.00249391, 0.398658,
                                  2.45651,  0,           1.5708 };

    std::vector<double> pose3 = { 0.440164, 0.166256, 0.297599,
                                  2.45651,  0,        1.5708 };

    // 接口调用: 直线运动到圆的起始点——pose1
    robot_interface->getMotionControl()->moveLine(pose1, 1.2, 0.25, 0.025, 0);

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "直线运动到圆的起始点成功" << std::endl;
    } else {
        std::cout << "直线运动到圆的起始点失败" << std::endl;
    }

    // 接口调用: 工具姿态相对于圆弧路径点坐标系保持不变
    robot_interface->getMotionControl()->setCirclePathMode(0);

    // 计算另一半圆弧的中间点
    auto result =
        cli->getMath()->calculateCircleFourthPoint(pose1, pose2, pose3, 1);
    if (std::get<1>(result) == 0) {
        std::cout << "计算另一半圆弧的中间点失败，无法完成圆运动" << std::endl;
    } else {
        auto pose4 = std::get<0>(result);
        // 圆运动执行3圈
        for (int i = 0; i < 3; i++) {
            CircleParameters param1;
            param1.pose_via = pose2;
            param1.pose_to = pose3;
            param1.a = 1.2;
            param1.v = 0.25;
            param1.blend_radius = 0.0;
            // 接口调用: 圆弧运动
            robot_interface->getMotionControl()->moveCircle2(param1);

            // 阻塞
            ret = waitArrival(robot_interface);
            if (ret == -1) {
                std::cout << "圆运动失败" << std::endl;
            }

            // 执行另一半圆弧运动
            CircleParameters param2;
            param2.pose_via = pose4;
            param2.pose_to = pose1;
            param2.a = 1.2;
            param2.v = 0.25;
            param2.blend_radius = 0.0;
            robot_interface->getMotionControl()->moveCircle2(param2);

            // 阻塞
            waitArrival(cli->getRobotInterface(robot_name));
            if (ret == 0) {
                std::cout << "圆运动成功" << std::endl;
            } else {
                std::cout << "圆运动失败" << std::endl;
            }
        }
    }
}

void exampleMoveC3(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂运动的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(1.0);

    // 接口调用: 设置工具中心点
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 接口调用: 开启规划器运行
    cli->getRuntimeMachine()->start();
    auto cur_plan_context = cli->getRuntimeMachine()->getPlanContext();
    // 接口调用: 获取新的线程id
    auto task_id = cli->getRuntimeMachine()->newTask();
    cli->getRuntimeMachine()->setPlanContext(
        task_id, std::get<1>(cur_plan_context), std::get<2>(cur_plan_context));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 关节角，单位: 弧度
    std::vector<double> joint_angle = { 0.0,
                                        -15.0 * (M_PI / 180),
                                        100.0 * (M_PI / 180),
                                        25.0 * (M_PI / 180),
                                        90.0 * (M_PI / 180),
                                        0.0 };
    // 接口调用: 关节运动到初始位置
    robot_interface->getMotionControl()->moveJoint(
        joint_angle, 80 * (M_PI / 180), 60 * (M_PI / 180), 0, 0);

    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到初始位置成功" << std::endl;
    } else {
        std::cout << "关节运动到初始位置失败" << std::endl;
    }

    // 位姿
    std::vector<double> pose1 = {
        0.440164, -0.119, 0.2976, 2.456512, 0, 1.5708
    };

    std::vector<double> pose2 = { 0.440164, -0.00249391, 0.398658,
                                  2.45651,  0,           1.5708 };

    std::vector<double> pose3 = { 0.440164, 0.166256, 0.297599,
                                  2.45651,  0,        1.5708 };

    // 接口调用: 直线运动到圆的起始点——pose1
    robot_interface->getMotionControl()->moveLine(pose1, 1.2, 0.25, 0.025, 0);

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "直线运动到圆的起始点成功" << std::endl;
    } else {
        std::cout << "直线运动到圆的起始点失败" << std::endl;
    }

    // 接口调用: 工具姿态相对于圆弧路径点坐标系保持不变
    robot_interface->getMotionControl()->setCirclePathMode(0);

    // 计算另一半圆弧的中间点
    auto result =
        cli->getMath()->calculateCircleFourthPoint(pose1, pose2, pose3, 1);

    // 圆运动期间连续
    if (std::get<1>(result) == 0) {
        std::cout << "计算另一半圆弧的中间点失败，无法完成圆运动" << std::endl;
    } else {
        auto pose4 = std::get<0>(result);
        // 圆运动执行3圈
        for (int i = 0; i < 3; i++) {
            // 接口调用: 圆弧运动
            robot_interface->getMotionControl()->moveCircle(pose2, pose3, 1.2,
                                                            0.25, 0.025, 0);

            // 执行另一半圆弧运动
            robot_interface->getMotionControl()->moveCircle(pose4, pose1, 1.2,
                                                            0.25, 0.025, 0);
        }
    }

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "圆运动成功" << std::endl;
    } else {
        std::cout << "圆运动失败" << std::endl;
    }
    // 接口调用: 停止规划器运行
    cli->getRuntimeMachine()->stop();
    // 接口调用: 删除线程
    cli->getRuntimeMachine()->deleteTask(task_id);
}

/**
 * 功能: 机械臂圆弧运动与圆运动
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
 * 第二步: 圆弧运动exampleMoveArc——用moveCircle接口实现
 * 第三步: 圆弧运动exampleMoveArc2——用moveCircle2接口实现
 * 第四步: 圆运动exampleMoveC——用moveCircle接口实现
 * 第五步: 圆运动exampleMoveC2——用moveCircle2接口实现
 * 第六步: RPC 退出登录、断开连接
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

    // 圆弧运动
    exampleMoveArc(rpc_cli);

    // 圆弧运动
    exampleMoveArc2(rpc_cli);

    // 圆运动
    exampleMoveC(rpc_cli);

    // 圆运动
    exampleMoveC2(rpc_cli);

    // 圆运动
    exampleMoveC3(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
