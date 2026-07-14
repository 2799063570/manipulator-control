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

void exampleMoveSpiral1(RpcClientPtr cli)
{
    // 关节角，单位: 弧度
    std::vector<double> start_q = {
        -0.04723656192459002, -0.3147961077805709, 2.122706013891513,
        1.388465989105001,    1.59432305862766,    -0.04096820515125211
    };

    // 位姿
    std::vector<double> start_p = { 0.4992287020163843,    -0.1430213015569043,
                                    0.1929798566172833,    2.619351290960044,
                                    3.577791911729708e-05, 1.570799472227775 };

    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 接口调用: 关节运动到起始位置
    robot_interface->getMotionControl()->moveJoint(start_q, 80 * (M_PI / 180),
                                                   60 * (M_PI / 180), 0, 0);
    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到起始位置成功！" << std::endl;
    } else {
        std::cout << "关节运动到起始位置失败！" << std::endl;
    }

    // 螺旋线运动圆心位置
    std::vector<double> center_position = { 0.4992155149838216,
                                            -0.08141538203368692,
                                            0.1929808028919965 };
    std::vector<double> center_pose(6, 0.);

    // 螺旋线运动基于基坐标系的XY平面
    // 圆心位置
    for (int i = 0; i < 3; i++) {
        center_pose[i] = center_position[i];
    }
    // 圆心姿态
    for (int i = 3; i < 6; i++) {
        center_pose[i] = 0.;
    }

    // 设置螺旋线运动参数
    SpiralParameters param;
    param.spiral = 0.005; // 每转一圈螺旋运动对应圆半径增加的步长
    param.helix = 0.005;    // 螺旋运动在z轴方向上的步长
    param.angle = 6 * M_PI; // 螺旋运动角度范围
    param.plane = 0;        // 基于XY平面
    param.frame = center_pose;

    robot_interface->getMotionControl()->moveSpiral(param, 0, 0.25, 1.2, 0);
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "螺旋线运动成功" << std::endl;
    } else {
        std::cout << "螺旋线运动失败" << std::endl;
    }
}

void exampleMoveSpiral2(RpcClientPtr cli)
{
    // 关节角，单位: 弧度
    std::vector<double> start_q = {
        -0.04723656192459002, -0.3147961077805709, 2.122706013891513,
        1.388465989105001,    1.59432305862766,    -0.04096820515125211
    };

    // 位姿
    std::vector<double> start_p = { 0.4992287020163843,    -0.1430213015569043,
                                    0.1929798566172833,    2.619351290960044,
                                    3.577791911729708e-05, 1.570799472227775 };

    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    auto robot_interface = cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
    std::vector<double> tcp_offset(6, 0.0);
    robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

    // 接口调用: 关节运动到起始位置
    robot_interface->getMotionControl()->moveJoint(start_q, 80 * (M_PI / 180),
                                                   60 * (M_PI / 180), 0, 0);
    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到起始位置成功！" << std::endl;
    } else {
        std::cout << "关节运动到起始位置失败！" << std::endl;
    }

    // 螺旋线运动圆心位置
    std::vector<double> center_position = { 0.4992155149838216,
                                            -0.08141538203368692,
                                            0.1929808028919965 };
    std::vector<double> center_pose(6, 0.);

    // 螺旋线运动基于TCP坐标系的XY平面
    // 圆心位置
    for (int i = 0; i < 3; i++) {
        center_pose[i] = center_position[i];
    }
    // 圆心姿态
    auto tcp_pose = robot_interface->getRobotState()->getTcpPose();
    for (int i = 3; i < 6; i++) {
        center_pose[i] = tcp_pose[i];
    }

    // 设置螺旋线运动参数
    SpiralParameters param;
    param.spiral = 0.005; // 每转一圈螺旋运动对应圆半径增加的步长
    param.helix = 0.005;     // 螺旋运动在z轴方向上的步长
    param.angle = 10 * M_PI; // 螺旋运动角度范围
    param.plane = 0;         // 基于XY平面
    param.frame = center_pose;

    robot_interface->getMotionControl()->moveSpiral(param, 0, 0.25, 1.2, 0);
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "螺旋线运动成功" << std::endl;
    } else {
        std::cout << "螺旋线运动失败" << std::endl;
    }
}

/**
 * 功能: 机械臂螺旋线运动
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
 * 第二步: 设置运动速度比率和工具中心点
 * 第三步: 先关节运动到起始位置，然后进行螺旋线运动
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

    // 螺旋线运动
    exampleMoveSpiral1(rpc_cli);

    // 螺旋线运动
    // exampleMoveSpiral2(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
