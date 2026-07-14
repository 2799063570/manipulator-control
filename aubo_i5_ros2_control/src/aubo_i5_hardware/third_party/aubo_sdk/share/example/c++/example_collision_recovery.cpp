#include "aubo_sdk/rpc.h"
#include <aubo/error_stack/error_stack.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <math.h>
using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 计算当前关节角与目标关节角的绝对差值
double distance(const std::vector<double> &a, const std::vector<double> &b)
{
    double res = 0.;
    if (a.size() != b.size()) {
        return -1;
    }

    for (int i = 0; i < (int)a.size(); i++) {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrt(res);
}

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
void waitArrival(RobotInterfacePtr impl, std::vector<double> target)
{
    while (1) {
        // 接口调用: 获取当前的关节角
        std::vector cur = impl->getRobotState()->getJointPositions();
        // 当前关节角与目标关节角的绝对差值小于 0.0001时，跳出循环
        double dis = distance(cur, target);
        if (dis < 0.0001) {
            break;
        }
        printf("当前关节角:%f,%f,%f,%f,%f,%f\n", cur.at(0), cur.at(1),
               cur.at(2), cur.at(3), cur.at(4), cur.at(5));
        printf("目标关节角:%f,%f,%f,%f,%f,%f\n", target.at(0), target.at(1),
               target.at(2), target.at(3), target.at(4), target.at(5));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// 关节运动恢复
void collision_recovery_movej(RpcClientPtr cli)

{
    // 接口调用: 获取机器人的名字
    auto name = cli->getRobotNames().front();
    auto impl = cli->getRobotInterface(name);

    // 接口调用: 获取碰撞后要恢复到的位置
    auto joint = impl->getMotionControl()->getPauseJointPositions();

    // 打印要恢复的位置
    printf("要恢复的位置:%f,%f,%f,%f,%f,%f\n", joint.at(0), joint.at(1),
           joint.at(2), joint.at(3), joint.at(4), joint.at(5));

    // 接口调用: 模拟碰撞后的示教运动
    impl->getMotionControl()->resumeSpeedLine({ 0, 0, -0.05, 0, 0, 0 }, 1.2,
                                              100);
#ifdef WIN32
    Sleep(1000 * 1);
#else
    usleep(1000 * 1000 * 1);
#endif
    // 接口调用: 停止示教运动
    impl->getMotionControl()->resumeStopLine(10, 10);
#ifdef WIN32
    Sleep(1000 * 1);
#else
    usleep(1000 * 1000 * 1);
#endif

    // 接口调用: 关节运动到暂停点
    impl->getMotionControl()->resumeMoveJoint(joint, 1, 1, 0);
    // 阻塞
    waitArrival(impl, joint);

    // 接口调用: 恢复规划器运行
    cli->getRuntimeMachine()->resume();
}

// 直线运动恢复
void collision_recovery_movel(RpcClientPtr cli)

{
    // 接口调用: 获取机器人的名字
    auto name = cli->getRobotNames().front();
    auto impl = cli->getRobotInterface(name);

    // 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
    std::vector<double> tcp_offset(6, 0.0);
    impl->getRobotConfig()->setTcpOffset(tcp_offset);

    // 接口调用: 获取碰撞后要恢复到的位置
    auto joint = impl->getMotionControl()->getPauseJointPositions();

    // 打印要恢复的位置
    printf("要恢复的关节角位置:%f,%f,%f,%f,%f,%f\n", joint.at(0), joint.at(1),
           joint.at(2), joint.at(3), joint.at(4), joint.at(5));

    // 接口调用: 模拟碰撞后的示教运动
    impl->getMotionControl()->resumeSpeedLine({ 0, 0, -0.05, 0, 0, 0 }, 1.2,
                                              100);
#ifdef WIN32
    Sleep(1000 * 1);
#else
    usleep(1000 * 1000 * 1);
#endif
    // 接口调用: 停止示教运动
    impl->getMotionControl()->resumeStopLine(10, 10);
#ifdef WIN32
    Sleep(1000 * 1);
#else
    usleep(1000 * 1000 * 1);
#endif

    // 接口调用: 正解获得暂停点的位置姿态
    auto result = impl->getRobotAlgorithm()->forwardKinematics(joint);

    if (0 == std::get<1>(result)) {
        // 接口调用: 直线运动到暂停点
        impl->getMotionControl()->resumeMoveLine(std::get<0>(result), 1.2, 0.25,
                                                 0.0);
    }
    // 阻塞
    waitArrival(impl, joint);
    std::cout << "直线运动到暂停点" << std::endl;

    // 接口调用: 恢复规划器运行
    cli->getRuntimeMachine()->resume();
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

    // 关节运动恢复
    collision_recovery_movej(rpc_cli);

    // 直线运动恢复
    //    collision_recovery_movel(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
