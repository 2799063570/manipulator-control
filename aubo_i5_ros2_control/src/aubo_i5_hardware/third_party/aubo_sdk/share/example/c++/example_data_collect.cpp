#include <cstring>
#include <fstream>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#endif

#include "aubo_sdk/rpc.h"

using namespace arcs::aubo_sdk;
using namespace arcs::common_interface;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrival(RobotInterfacePtr impl)
{
    //接口调用: 获取当前的运动指令 ID
    int exec_id = impl->getMotionControl()->getExecId();

    int cnt = 0;
    // 在等待机械臂开始运动时，获取exec_id最大的重试次数
    int max_retry_count = 50;

    // 等待机械臂开始运动
    while (exec_id == -1) {
        if (cnt++ > max_retry_count) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        exec_id = impl->getMotionControl()->getExecId();
    }

    // 等待机械臂动作完成
    while (exec_id != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        exec_id = impl->getMotionControl()->getExecId();
    }

    return 0;
}

// 判断缓存是否有效
int isBufferValid(RobotInterfacePtr impl)
{
    // 调用pathBufferValid最大的重试次数
    int max_retry_count = 5;
    // 调用pathBufferValid的次数
    int cnt = 0;
    bool isValid = impl->getMotionControl()->pathBufferValid("rec");

    while (!isValid) {
        if (cnt++ > max_retry_count) {
            return -1;
        }
        isValid = impl->getMotionControl()->pathBufferValid("rec");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 0;
}

class TrajectoryIo
{
public:
    // 构造函数，接受要打开的文件名作为参数
    TrajectoryIo(const char *filename)
    {
        input_file_.open(filename, std::ios::in);
    }

    // 检查文件是否成功打开
    bool open()
    {
        if (!input_file_.is_open()) {
            std::cerr << "无法打开轨迹文件. 请检查输入的文件路径是否正确."
                      << std::endl;
            return false;
        }
        return true;
    }
    ~TrajectoryIo() { input_file_.close(); }

    // 解析文件中的轨迹数据，
    // 并将其转换为一个二维的 std::vector。
    // 它逐行读取文件内容，将每行数据解析为一组 double 数值，
    // 并将这些数值存储在一个嵌套的二维向量中。
    std::vector<std::vector<double>> parse()
    {
        std::vector<std::vector<double>> res;
        std::string tmp;
        int linenum = 1;
        while (std::getline(input_file_, tmp, '\n')) {
            try {
                auto q = split(tmp, ",");
                res.push_back(q);
            } catch (const char *p) {
                std::cerr << "Line: " << linenum << " \"" << p << "\""
                          << " is not a number of double" << std::endl;
                break;
            }
            linenum++;
        }
        return res;
    }

    // 切割字符串并转换为 double 类型
    std::vector<double> split(const std::string &str, const char *delim)
    {
        std::vector<double> res;
        if ("" == str) {
            return res;
        }
        // 先将要切割的字符串从string类型转换为char*类型
        char *strs = new char[str.length() + 1]; // 不要忘了
        std::strcpy(strs, str.c_str());

        char *p = std::strtok(strs, delim);
        char *endp = nullptr;
        while (p) {
            double v = std::strtod(p, &endp);
            if (endp[0] != 0 && endp[0] != '\r') {
                delete[] strs;
                strs = nullptr;
                throw p;
            }
            res.push_back(v); // 存入结果数组
            p = std::strtok(nullptr, delim);
        }

        if (strs) {
            delete[] strs;
            strs = nullptr;
        }

        return res;
    }

private:
    std::ifstream input_file_; // 输入文件流
};

int runTraj(RpcClientPtr rpc, const std::string &traj_name,
            const std::string &record_name)
{
    // 读取轨迹文件
    auto filename = traj_name;
    TrajectoryIo input(filename.c_str());

    // 尝试打开轨迹文件，如果无法打开，直接返回
    if (!input.open()) {
        return 0;
    }

    // 解析轨迹数据
    auto traj = input.parse();

    auto traj_sz = traj.size();
    if (traj_sz == 0) {
        std::cerr << "轨迹文件中的路点数量为0" << std::endl;
        return 0;
    } else {
        std::cout << " 加载的路点数量为: " << traj.size() << std::endl;
    }

    // 接口调用: 获取机器人的名字
    auto robot_name = rpc->getRobotNames().front();

    auto robot_interface = rpc->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 关节运动到轨迹文件中的第一个路点
    robot_interface->getMotionControl()->moveJoint(traj[0], 30 * (M_PI / 180),
                                                   30 * (M_PI / 180), 0., 0.);

    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到轨迹文件中的第一个路点成功" << std::endl;
    } else {
        std::cout << "关节运动到轨迹文件中的第一个路点失败" << std::endl;
    }

    // 接口调用: 开启规划器运行
    rpc->getRuntimeMachine()->start();
    auto cur_plan_context = rpc->getRuntimeMachine()->getPlanContext();
    // 接口调用: 获取新的线程id
    auto task_id = rpc->getRuntimeMachine()->newTask();
    rpc->getRuntimeMachine()->setPlanContext(
        task_id, std::get<1>(cur_plan_context), std::get<2>(cur_plan_context));

    // 接口调用: 清除缓存"rec"
    robot_interface->getMotionControl()->pathBufferFree("rec");

    // 接口调用: 新建一个缓存"rec"，并指定轨迹运动类型和轨迹点数量
    robot_interface->getMotionControl()->pathBufferAlloc("rec", 2, traj_sz);

    // 将轨迹文件中的路点分组添加到路径缓存中，
    // 以10个点为1组，
    // 如果未添加的路点数量小于或者等于10时，则作为最后一组来添加
    size_t offset = 10;
    auto it = traj.begin();
    while (true) {
        // 接口调用: 添加轨迹路点道缓存路径中
        robot_interface->getMotionControl()->pathBufferAppend(
            "rec", std::vector<std::vector<double>>{ it, it + 10 });
        it += 10;
        if (offset + 10 >= traj_sz) {
            // 接口调用: 添加轨迹路点道缓存路径中
            robot_interface->getMotionControl()->pathBufferAppend(
                "rec", std::vector<std::vector<double>>{ it, traj.end() });
            break;
        }

        offset += 10;
    }

    // 轨迹文件中的采样间隔
    double interval = 0.005;
    //    double interval = 0.02;

    // 接口调用: 计算、优化等耗时操作，来对轨迹进行优化
    robot_interface->getMotionControl()->pathBufferEval("rec", {}, {},
                                                        interval);

    // 判断路径缓存是否有效
    if (isBufferValid(robot_interface) == -1) {
        std::cerr << "路径缓存无效，无法进行轨迹运动" << std::endl;
    } else {
        robot_interface->getRobotManage()->startRecord(record_name);
        // 接口调用: 执行轨迹运动
        robot_interface->getMotionControl()->movePathBuffer("rec");

        // 等待轨迹运动完成
        ret = waitArrival(rpc->getRobotInterface(robot_name));
        if (ret == -1) {
            std::cerr << "轨迹运动失败" << std::endl;
        } else {
            std::cout << "轨迹运动结束" << std::endl;
        }
    }
    robot_interface->getRobotManage()->stopRecord();
    // 接口调用: 停止规划器运行
    rpc->getRuntimeMachine()->stop();
    // 接口调用: 删除线程
    rpc->getRuntimeMachine()->deleteTask(task_id);
}

#define LOCAL_IP "192.168.1.15"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    auto rpc = std::make_shared<RpcClient>();
    // 接口调用: 设置 RPC 超时
    rpc->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc->login("aubo", "123456");
    auto robot_name = rpc->getRobotNames().front();

    auto robot_interface = rpc->getRobotInterface(robot_name);

    // 设置滤波参数
    std::stringstream ss;
    ss << "[filter_freq]" << std::endl;
    ss << "    vel_filter_freq  = [100, 100, 100, 100, 100, 100]" << std::endl;
    ss << "    acc_filter_freq  = [100, 100, 100, 100, 100, 100]" << std::endl;
    ss << "    curr_filter_freq = [100, 100, 100, 100, 100, 100]" << std::endl;
    robot_interface->getRobotConfig()->setHardwareCustomParameters(ss.str());

    // 设置负载参数
    robot_interface->getRobotConfig()->setPayload(
        4.1768, { 0, 0.1411, 0.0541 }, {},
        { 0.1114, 0, 0, 0.0312, 0.0363, 0.1125 });

    // 机器人序列号
    std::string robot_serial = "AB1115312D000083";

    // 轨迹名字
    std::string filename1 = "1_低速";
    std::cout << "开始轨迹: " << filename1 << std::endl;

    // 开始运行轨迹
    runTraj(rpc, "../trajs/collision_traj/" + filename1 + ".csv",
            robot_serial + "_" + filename1 + ".csv");

    std::string filename2 = "2_中速";
    std::cout << "开始轨迹: " << filename2 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename2 + ".csv",
            robot_serial + "_" + filename2 + ".csv");

    std::string filename3 = "3_高速";
    std::cout << "开始轨迹: " << filename3 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename3 + ".csv",
            robot_serial + "_" + filename3 + ".csv");

    std::string filename4 = "4_低加速度";
    std::cout << "开始轨迹: " << filename4 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename4 + ".csv",
            robot_serial + "_" + filename4 + ".csv");

    std::string filename5 = "5_中加速度";
    std::cout << "开始轨迹: " << filename5 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename5 + ".csv",
            robot_serial + "_" + filename5 + ".csv");

    std::string filename6 = "6_大加速度";
    std::cout << "开始轨迹: " << filename6 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename6 + ".csv",
            robot_serial + "_" + filename6 + ".csv");

    std::string filename7 = "7_恒低速";
    std::cout << "开始轨迹: " << filename7 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename7 + ".csv",
            robot_serial + "_" + filename7 + ".csv");

    std::string filename8 = "8_恒中速";
    std::cout << "开始轨迹: " << filename8 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename8 + ".csv",
            robot_serial + "_" + filename8 + ".csv");

    std::string filename9 = "9_恒高速";
    std::cout << "开始轨迹: " << filename9 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename9 + ".csv",
            robot_serial + "_" + filename9 + ".csv");

    std::string filename11 = "11_trajectory_vel2_acc5";
    std::cout << "开始轨迹: " << filename11 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename11 + ".txt",
            robot_serial + "_" + filename11 + ".csv");

    std::string filename12 = "12_trajectory_vel2_acc20";
    std::cout << "开始轨迹: " << filename12 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename12 + ".txt",
            robot_serial + "_" + filename12 + ".csv");

    std::string filename13 = "13_trajectory_vel2_acc30";
    std::cout << "开始轨迹: " << filename13 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename13 + ".txt",
            robot_serial + "_" + filename13 + ".csv");

    std::string filename14 = "14_trajectory_vel2.2_acc10";
    std::cout << "开始轨迹: " << filename14 << std::endl;
    runTraj(rpc, "../trajs/collision_traj/" + filename14 + ".txt",
            robot_serial + "_" + filename14 + ".csv");
}
