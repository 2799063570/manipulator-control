#include <cstring>
#include <fstream>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#endif

#include <nlohmann/json.hpp>
#include "aubo_sdk/rpc.h"

using namespace arcs::aubo_sdk;
using namespace arcs::common_interface;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace nlohmann;

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

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    // 读取轨迹文件
    std::ifstream f("../trajs/aubo-joint-test-0929-10.armplay");

    if (!f.is_open()) {
        std::cerr << "无法打开轨迹文件. 请检查输入的文件路径是否正确."
                  << std::endl;
        return 0;
    }

    // 解析轨迹文件
    auto input = json::parse(f);

    std::vector<std::vector<double>> traj = input["jointlist"];
    auto traj_sz = traj.size();
    if (traj_sz == 0) {
        std::cerr << "轨迹文件中的路点数量为0" << std::endl;
        return 0;
    } else {
        std::cout << " 加载的路点数量为: " << traj.size() << std::endl;
    }

    auto rpc = std::make_shared<RpcClient>();
    // 接口调用: 设置 RPC 超时
    rpc->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc->login("aubo", "123456");

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
        std::cout << "添加轨迹路点 " << offset << std::endl;
        // 接口调用: 添加轨迹路点道缓存路径中
        robot_interface->getMotionControl()->pathBufferAppend(
            "rec", std::vector<std::vector<double>>{ it, it + 10 });
        it += 10;
        if (offset + 10 >= traj_sz) {
            std::cout << "添加轨迹路点 " << traj_sz << std::endl;
            // 接口调用: 添加轨迹路点道缓存路径中
            robot_interface->getMotionControl()->pathBufferAppend(
                "rec", std::vector<std::vector<double>>{ it, traj.end() });
            break;
        }

        offset += 10;
    }

    // 轨迹文件中的采样间隔
    double interval = input["interval"];
    //    double interval = 0.02;

    // 接口调用: 计算、优化等耗时操作，来对轨迹进行优化
    robot_interface->getMotionControl()->pathBufferEval("rec", {}, {},
                                                        interval);

    // 判断路径缓存是否有效
    if (isBufferValid(robot_interface) == -1) {
        std::cerr << "路径缓存无效，无法进行轨迹运动" << std::endl;
    } else {
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

    // 接口调用: 停止规划器运行
    rpc->getRuntimeMachine()->stop();
    // 接口调用: 删除线程
    rpc->getRuntimeMachine()->deleteTask(task_id);

    // 接口调用: RPC 退出登录
    rpc->logout();
    // 接口调用: RPC 断开连接
    rpc->disconnect();

    return 0;
}
