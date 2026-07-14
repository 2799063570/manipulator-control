#include <math.h>
#include "aubo_sdk/rpc.h"
#include <fstream>
#ifdef WIN32
#include <Windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define GRAVITY 9.81 // 重力加速度
//#define EMBEDDED
#define ENABLE_JOINT_ADMITANCE
std::ofstream file("force.csv", std::ios::app);

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrivel(RobotInterfacePtr impl)
{
    int cnt = 0;
    while (impl->getMotionControl()->getExecId() == -1) {
        if (cnt++ > 5) {
            std::cout << "Motion fail!" << std::endl;
            return -1;
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

inline double calculateDistance(
    const std::vector<double> &p1,
    const std::vector<double> &p2 = std::vector<double>(6, 0))
{
    if ((6 == p1.size()) && (6 == p2.size())) {
        double sum = 0.;
        for (int i = 0; i < 3; i++) {
            sum += pow(p1[i] - p2[i], 2);
        }
        return std::sqrt(sum);
    }
    return 0;
}

void tcpSensorTest(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->selectTcpForceSensor("kw_ftsensor");
    while (1) {
        auto sensor_data = cli->getRobotInterface(robot_name)
                               ->getRobotState()
                               ->getTcpForceSensors();

        std::cout << "--------------------------------------" << std::endl;
        for (int i = 0; i < sensor_data.size(); i++) {
            std::cout << "标定前force: " << i + 1 << ": " << sensor_data[i]
                      << std::endl;
        }
        std::cout << "--------------------------------------" << std::endl;
        file << sensor_data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

ForceSensorCalibResult tcpSensorcalibration(
    RpcClientPtr cli, std::vector<std::vector<double>> joints)
{
    auto robot_name = cli->getRobotNames().front();

    printf("goto p0\n");
    // 接口调用: 关节运动
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(joints[0], 10 * (M_PI / 180), 5 * (M_PI / 180), 0, 0);
    // 阻塞
    waitArrivel(cli->getRobotInterface(robot_name));
    //等待机器人停稳
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //记录关节位置和tcp force
    auto q1 = cli->getRobotInterface(robot_name)
                  ->getRobotState()
                  ->getJointPositions();
    auto tcp_force1 = cli->getRobotInterface(robot_name)
                          ->getRobotState()
                          ->getTcpForceSensors();

    printf("goto p1\n");
    // 接口调用: 关节运动
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(joints[1], 20 * (M_PI / 180), 10 * (M_PI / 180), 0, 0);
    // 阻塞
    waitArrivel(cli->getRobotInterface(robot_name));
    //等待机器人停稳
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //记录关节位置和tcp force
    auto q2 = cli->getRobotInterface(robot_name)
                  ->getRobotState()
                  ->getJointPositions();
    auto tcp_force2 = cli->getRobotInterface(robot_name)
                          ->getRobotState()
                          ->getTcpForceSensors();

    printf("goto p2\n");
    // 接口调用: 关节运动
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(joints[2], 20 * (M_PI / 180), 10 * (M_PI / 180), 0, 0);
    // 阻塞
    waitArrivel(cli->getRobotInterface(robot_name));
    //等待机器人停稳
    std::this_thread::sleep_for(std::chrono::seconds(1));

    //记录关节位置和tcp force
    auto q3 = cli->getRobotInterface(robot_name)
                  ->getRobotState()
                  ->getJointPositions();
    auto tcp_force3 = cli->getRobotInterface(robot_name)
                          ->getRobotState()
                          ->getTcpForceSensors();

    auto pose1 = cli->getRobotInterface(robot_name)
                     ->getRobotAlgorithm()
                     ->forwardKinematics(q1);

    auto pose2 = cli->getRobotInterface(robot_name)
                     ->getRobotAlgorithm()
                     ->forwardKinematics(q2);

    auto pose3 = cli->getRobotInterface(robot_name)
                     ->getRobotAlgorithm()
                     ->forwardKinematics(q3);

    std::vector<std::vector<double>> calib_forces{ tcp_force1, tcp_force2,
                                                   tcp_force3 };

    std::vector<std::vector<double>> calib_poses{ std::get<0>(pose1),
                                                  std::get<0>(pose2),
                                                  std::get<0>(pose3) };

    auto result = cli->getRobotInterface(robot_name)
                      ->getRobotAlgorithm()
                      ->calibrateTcpForceSensor(calib_forces, calib_poses);
    return result;
}

void exampleForceControl(RpcClientPtr cli)
{
    auto robot_name = cli->getRobotNames().front();
#ifdef EMBEDDED
    // 内置传感器
    std::vector<double> sensor_pose = { 0, 0, 0, 0, 0, 0 };
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->selectTcpForceSensor("embedded");
#else
    // 外置坤维传感器
    std::vector<double> sensor_pose = { 0, 0, 0.047, 0, 0, 0 };
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->selectTcpForceSensor("kw_ftsensor");
#endif

    // 设置传感器安装位姿
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->setTcpForceSensorPose(sensor_pose);
    // 设置TCP偏移
    std::vector<double> tcp_pose = { 0, 0, 0.080, 0, 0, 0 };
    // std::vector<double> tcp_pose = sensor_pose;
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->setTcpOffset(tcp_pose);
    std::vector<double> joint1 = { -0.261799, 0.261799, 1.309,
                                   1.0472,    1.39626,  0.0 };
    std::vector<double> joint2 = { -0.628319, 0.471239, 1.65806,
                                   -0.471239, 0.0,      0.0 };
    std::vector<double> joint3 = { -0.628319, 0.366519, 1.74533,
                                   -0.10472,  1.5708,   0.0 };
    // 力传感器标定 force_offset, com, mass,
    auto calib_result = tcpSensorcalibration(cli, { joint1, joint2, joint3 });
    std::cout << "force_offset: " << std::get<0>(calib_result) << std::endl;
    std::cout << "com: " << std::get<1>(calib_result) << std::endl;
    std::cout << "mass: " << std::get<2>(calib_result) << std::endl;

    if (calculateDistance(std::get<0>(calib_result)) < 0.0001) {
        std::cout << "标定错误，请检查传感器数据!" << std::endl;
        exit(-1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "go to start_jiont" << std::endl;
    std::vector<double> start_joint = {
        0 / 180 * M_PI,     -66.25 / 180 * M_PI, 103.72 / 180 * M_PI,
        79.71 / 180 * M_PI, 90.21 / 180 * M_PI,  0 / 180 * M_PI
    };
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(start_joint, 10 * (M_PI / 180), 5 * (M_PI / 180), 0, 0);
    waitArrivel(cli->getRobotInterface(robot_name));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    bool ideal_param = false;
    if (ideal_param) {
        std::cout << "使用理想参数" << std::endl;
        double mass = 15.15;
        std::vector<double> com = { 0, 0, 0.080 };

        std::vector<double> force_offset = { 2.03688, -2.52339, -1.21147,
                                             0.05,    0.008,    0.2 };
        // 设置负载
        cli->getRobotInterface(robot_name)
            ->getRobotConfig()
            ->setPayload(mass, com, { 0. }, { 0. });
        // 设置力传感器偏移
        cli->getRobotInterface(robot_name)
            ->getRobotConfig()
            ->setTcpForceOffset(force_offset);
    } else {
        std::cout << "使用辨识参数" << std::endl;
        // 设置负载
        cli->getRobotInterface(robot_name)
            ->getRobotConfig()
            ->setPayload(std::get<2>(calib_result), std::get<1>(calib_result),
                         { 0. }, { 0. });
        // 设置力传感器偏移
        cli->getRobotInterface(robot_name)
            ->getRobotConfig()
            ->setTcpForceOffset(std::get<0>(calib_result));
    }

#ifdef ENABLE_JOINT_ADMITANCE
    // i5 空载参数
    // M = [3.3, 3.3, 3.3, 0.6, 0.6, 0.6]
    // D = [10.0, 10.0, 10.0, 2.0, 2.0, 2.0]
    // K = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    // i20 16公斤负载参数
    std::vector<double> admittance_m = { 60.0, 60.0, 60.0, 6.0, 5.0, 4.0 };
    std::vector<double> admittance_d = {
        200.0, 200.0, 200.0, 30.0, 40.0, 15.0
    };
    std::vector<double> admittance_k = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

#else
    std::vector<double> admittance_m = { 8.0, 8.0, 8.0, 0.3, 0.3, 0.3 };
    std::vector<double> admittance_d = { 48.0, 48.0, 48.0, 2.0, 2.0, 2.0 };
    std::vector<double> admittance_k = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
#endif

    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setDynamicModel(admittance_m, admittance_d, admittance_k);
    std::vector<bool> compliance = { true, true, true, true, true, true };
    std::vector<double> target_wrench(6, 0.);
    std::vector<double> speed_limits(6, 2.0);
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(std::vector<double>(6, 0.), compliance, target_wrench,
                         speed_limits, TaskFrameType::NONE);
    std::vector<double> cutoff_freq = {
        30.0, 30.0, 30.0, 30.0, 30.0, 30.0,
    };
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setLpFilter(cutoff_freq);
    // tcpSensorTest(cli);

    std::cout << "Press the key 's'/'q' to enable/disable force control mode."
              << std::endl;
    while (1) {
        std::cout << "Please input your choose: " << std::endl;
        char key;
        std::cin >> key;
        switch (key) {
        case 's':
            if (cli->getRobotInterface(robot_name)
                    ->getForceControl()
                    ->isFcEnabled()) {
                std::cout << "The robot has already been force control mode. "
                             "Can't enable force control mode"
                          << std::endl;
                break;
            } else {
                //使能力控
                cli->getRobotInterface(robot_name)
                    ->getForceControl()
                    ->fcEnable();
                std::cout << "Enter force control mode" << std::endl;
            }
            break;
        case 'q':
            if (!cli->getRobotInterface(robot_name)
                     ->getForceControl()
                     ->isFcEnabled()) {
                std::cout << "The robot has already quit force control mode. "
                             "Can't disable force control mode"
                          << std::endl;
                break;
            } else {
                //退出力控
                cli->getRobotInterface(robot_name)
                    ->getForceControl()
                    ->fcDisable();
                std::cout << "Quit force control mode" << std::endl;
            }
            break;
        default:
            std::cout << "Please input 's' or 'q'." << std::endl;
            break;
        }
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
    rpc_cli->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc_cli->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    exampleForceControl(rpc_cli);
    // tcpSensorTest(rpc_cli);
}
