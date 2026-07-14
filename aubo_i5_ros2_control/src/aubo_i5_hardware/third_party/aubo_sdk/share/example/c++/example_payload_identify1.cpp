#include <mutex>
#include <fstream>
#include <math.h>
#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;
std::mutex rtde_mtx_;

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const std::vector<T> &list)
{
    for (size_t i = 0; i < list.size(); i++) {
        os << list.at(i);
        if (i != (list.size() - 1)) {
            os << ",";
        }
    }
    return os;
}
// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrivel(RobotInterfacePtr impl)
{
    int cnt = 0;
    while (impl->getMotionControl()->getExecId() == -1) {
        if (cnt++ > 5) {
            std::cout << "Motion fail!" << std::endl;
            exit(-1);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    auto id = impl->getMotionControl()->getExecId();
    while (1) {
        auto id1 = impl->getMotionControl()->getExecId();
        if (id != id1) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}
void runTraj(RpcClientPtr cli, std::vector<double> q1, std::vector<double> q2,
             std::vector<double> q3, std::string file_name)
{
    auto robot_name = cli->getRobotNames().front();
    cli->getRobotInterface(robot_name)
        ->getRobotManage()
        ->startRecord(file_name);
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(q1, 10 * (M_PI / 180), 5 * (M_PI / 180), 0, 0);
    waitArrivel(cli->getRobotInterface(robot_name));
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(q2, 10 * (M_PI / 180), 5 * (M_PI / 180), 0, 0);
    waitArrivel(cli->getRobotInterface(robot_name));
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(q3, 10 * (M_PI / 180), 5 * (M_PI / 180), 0, 0);
    waitArrivel(cli->getRobotInterface(robot_name));
    cli->getRobotInterface(robot_name)->getRobotManage()->stopRecord();
}

void examplePayloadIdentify1(RpcClientPtr cli)
{
    std::vector<double> point1 = { -0.261799, 0.261799, 1.309,
                                   1.0472,    1.39626,  0.0 };
    std::vector<double> point2 = { -0.628319, 0.471239, 1.65806,
                                   -0.471239, 0.0,      0.0 };
    std::vector<double> point3 = { -0.628319, 0.366519, 1.74533,
                                   -0.10472,  1.5708,   0.0 };

    std::string file = "test_data.csv";
    runTraj(cli, point1, point2, point3, file);
    auto robot_name = cli->getRobotNames().front();
    auto ret = cli->getRobotInterface(robot_name)
                   ->getRobotAlgorithm()
                   ->payloadIdentify1(file);
    while (1) {
        auto ret = cli->getRobotInterface(robot_name)
                       ->getRobotAlgorithm()
                       ->payloadCalculateFinished();
        if (ret == 0) {
            std::cout << "负载计算完成！" << std::endl;
            break;
        } else if (ret == 1) {
            std::cout << "负载正在计算..." << std::endl;
        } else {
            std::cout << "负载计算失败，ret=" << ret << std::endl;
            exit(-1);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    auto result = cli->getRobotInterface(robot_name)
                      ->getRobotAlgorithm()
                      ->getPayloadIdentifyResult();
    std::cout << "计算结果为:" << std::endl;
    std::cout << "mass: " << std::get<0>(result) << std::endl;
    std::cout << "com: " << std::get<1>(result) << std::endl;
    std::cout << "inertia: " << std::get<3>(result) << std::endl;
}

/**
 * 功能: 负载辨识
 * 步骤:
 * 第一步: 连接到 RPC 服务、登录
 * 第二步: 机械臂上电
 * 第三步: 设置三个目标点
 * 第四步: 关节运动至目标点, 采集关节位置、关节速度、关节加速度、关节电流
 * 第五步: 计算辨识结果
 */
#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    auto rpc_cli = std::make_shared<RpcClient>();
    rpc_cli->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc_cli->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    auto rtde_cli = std::make_shared<RtdeClient>();
    // 接口调用: 连接到 RTDE 服务
    rtde_cli->connect(LOCAL_IP, 30010);
    // 接口调用: 登录
    rtde_cli->login("aubo", "123456");

    examplePayloadIdentify1(rpc_cli);
}
