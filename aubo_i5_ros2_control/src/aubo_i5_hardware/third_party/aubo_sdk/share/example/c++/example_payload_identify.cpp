#include <mutex>
#include <fstream>
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

void waitMovePathBufferFinished(RobotInterfacePtr impl)
{
    while (impl->getMotionControl()->getExecId() == -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    while (1) {
        auto id = impl->getMotionControl()->getExecId();
        if (id == -1) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void generateTraj(RpcClientPtr cli, TrajConfig &traj_conf)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();

    cli->getRobotInterface(robot_name)
        ->getRobotAlgorithm()
        ->generatePayloadIdentifyTraj("identify_traj", traj_conf);
    while (1) {
        auto ret = cli->getRobotInterface(robot_name)
                       ->getRobotAlgorithm()
                       ->payloadIdentifyTrajGenFinished();
        if (ret == 0) {
            std::cout << "负载辨识轨迹生成完成！" << std::endl;
            break;
        } else if (ret == 1) {
            std::cout << "负载辨识轨迹正在生成中..." << std::endl;
        } else {
            std::cout << "轨迹生成失败，ret=" << ret << std::endl;
            exit(-1);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    if (cli->getRobotInterface(robot_name)
            ->getMotionControl()
            ->pathBufferEval("identify_traj", {}, {}, 0.005) < 0) {
        std::cout << "pathBufferEval error!" << std::endl;
        exit(-1);
    }

    while (!cli->getRobotInterface(robot_name)
                ->getMotionControl()
                ->pathBufferValid("identify_traj")) {
        std::cout << "轨迹优化中..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    std::cout << "负载辨识轨迹生成完成！" << std::endl;
}

void runTraj(RpcClientPtr cli, RtdeClientPtr rtde_cli,
             const std::string &data_file_name, TrajConfig &traj_conf)
{
    std::ofstream file(data_file_name, std::ios::out | std::ios::trunc);
    auto robot_name = cli->getRobotNames().front();
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(traj_conf.init_joint, 5.0, 3.0, 0, 0);
    waitArrivel(cli->getRobotInterface(robot_name));
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->movePathBuffer("identify_traj");
    cli->getRobotInterface(robot_name)
        ->getRobotManage()
        ->startRecord(data_file_name);
    waitMovePathBufferFinished(cli->getRobotInterface(robot_name));
    std::cout << "轨迹运行完成" << std::endl;
    cli->getRobotInterface(robot_name)->getRobotManage()->stopRecord();
}

void examplePayloadIdentify(RpcClientPtr cli, RtdeClientPtr rtde_cli)
{
    // 生成激励轨迹
    TrajConfig traj_conf;
    traj_conf.move_axis = PayloadIdentifyMoveAxis::Joint_3_6;
    traj_conf.max_velocity = { 3.0, 3.0 }; // 维度与运动的关节数量一致
    traj_conf.max_acceleration = { 5.0, 5.0 }; // 维度与运动的关节数量一致
    traj_conf.lower_joint_bound = { -1.5, -2 };
    traj_conf.upper_joint_bound = { 1.5, 2 };
    traj_conf.init_joint = { -0.0, -0.2618, 1.047, -0.1745, 1.57, 0.0 };
    generateTraj(cli, traj_conf);
    std::cout << "请卸下负载，按 Enter 键开始运行轨迹:" << std::endl;
    std::cin.get();
    runTraj(cli, rtde_cli, "data_no_payload.csv", traj_conf);
    std::cout << "请安装负载，按 Enter 键开始运行轨迹:" << std::endl;
    std::cin.get();
    runTraj(cli, rtde_cli, "data_with_payload.csv", traj_conf);

    auto robot_name = cli->getRobotNames().front();
    auto ret =
        cli->getRobotInterface(robot_name)
            ->getRobotAlgorithm()
            ->payloadIdentify("data_no_payload.csv", "data_with_payload.csv");
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
 * 第三步: 设置激励轨迹的限制，生成激励轨迹
 * 第四步: 空载运行离线轨迹，通过rtde采集关节位置、关节速度、关节电流
 * 第五步: 带载运行离线轨迹，通过rtde采集关节位置、关节速度、关节电流
 * 第六步: 计算辨识结果
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

    examplePayloadIdentify(rpc_cli, rtde_cli);
}
