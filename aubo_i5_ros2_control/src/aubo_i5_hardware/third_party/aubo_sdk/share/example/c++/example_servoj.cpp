#include <cstring>
#include <fstream>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#endif

#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"
#include "aubo_sdk/script.h"

using namespace arcs::aubo_sdk;
using namespace arcs::common_interface;

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

const char *SERVOJ_SCRIPT = R"(local aubo = require('aubo')
    local sched = sched or aubo.sched
    local math = aubo.math or math
    local sync = sched.sync
return function(api)
    local names = api:getRobotNames()
    robot = api:getRobotInterface(names[1])
    local function servoj(...)
        return robot:getMotionControl():servoJoint(...)
    end
    local function get_target()
        local target_q = robot:getRobotState():getJointPositions()
        for i=1,6 do
            target_q[i] = api:getRegisterControl():getDoubleInput(i-1)
        end
        return target_q
    end

    local running = true
    robot:getMotionControl():setServoMode(true)
    api:getRegisterControl():setInt32Input(0, 1)
    while running do
        local flag = api:getRegisterControl():getInt32Input(0)
        if flag == 6 then
            servoj(get_target(), 0.0, 0.0, 0.005, 0.0, 0.0)
        elseif flag == -1 then
            running = false
        end
        sync()
    end
    while robot:getRobotState():isSteady() == false do
    end
    -- 当机械臂在运动时，setServoMode会失败
    robot:getMotionControl():setServoMode(false)
end


)";

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

    auto rtde_cli = std::make_shared<RtdeClient>();
    // 接口调用: 连接到 RTDE 服务
    rtde_cli->connect(LOCAL_IP, 30010);
    // 接口调用: 登录
    rtde_cli->login("aubo", "123456");

    auto script_cli = std::make_shared<ScriptClient>();
    // 接口调用: 连接到 SCRIPT 服务
    script_cli->connect(LOCAL_IP, 30002);
    // 接口调用: 登录
    script_cli->login("aubo", "123456");

    // 接口调用: 设置话题
    int chanel_out = rtde_cli->setTopic(
        true,
        { "input_int_registers_0", "input_double_registers_0",
          "input_double_registers_1", "input_double_registers_2",
          "input_double_registers_3", "input_double_registers_4",
          "input_double_registers_5" },
        200, 0);

    // 读取轨迹文件
    auto filename = "../trajs/record6.offt";
    TrajectoryIo input(filename);

    // 尝试打开轨迹文件，如果无法打开，直接返回
    if (!input.open()) {
        return 0;
    }

    // 解析轨迹数据
    auto traj = input.parse();

    // 检查轨迹文件中是否有路点，
    // 如果数量为 0，输出错误消息并返回
    auto traj_sz = traj.size();
    if (traj_sz == 0) {
        std::cerr << "轨迹文件中的路点数量为0." << std::endl;
        return 0;
    }

    // 接口调用: 获取机器人的名字
    auto robot_name = rpc_cli->getRobotNames().front();

    auto robot_interface = rpc_cli->getRobotInterface(robot_name);

    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(0.3);

    // 接口调用: 关节运动到轨迹文件中的第一个路点
    rpc_cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(traj[0], 30 * (M_PI / 180), 30 * (M_PI / 180), 0., 0.);

    // 阻塞
    int ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到轨迹文件中的第一个路点成功" << std::endl;
    } else {
        std::cout << "关节运动到轨迹文件中的第一个路点失败" << std::endl;
    }

    // 接口调用: 执行本地脚本
    script_cli->sendString(SERVOJ_SCRIPT);

    size_t i = 1;
    while (1) {
        // 接口调用: 发布话题
        rtde_cli->publish(chanel_out, [&](OutputBuilder &ob) {
            ob.push(6);
            ob.push(traj[i][0]);
            ob.push(traj[i][1]);
            ob.push(traj[i][2]);
            ob.push(traj[i][3]);
            ob.push(traj[i][4]);
            ob.push(traj[i][5]);
            std::cout << i << ": " << traj[i] << std::endl;
            i++;
        });

        if (i == traj_sz) {
            // 接口调用: 发布话题
            rtde_cli->publish(chanel_out, [&](OutputBuilder &ob) {
                std::cout << i << ": end" << std::endl;
                ob.push(-1);
                ob.push(0);
                ob.push(0);
                ob.push(0);
                ob.push(0);
                ob.push(0);
                ob.push(0);
                i++;
            });
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // 阻塞已保证执行完脚本中的setServoMode(false)来退出伺服模式
    while (robot_interface->getMotionControl()->isServoModeEnabled() == true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    std::cout << "servoj运动结束" << std::endl;

    // 接口调用: 取消话题
    rtde_cli->removeTopic(false, chanel_out);

    // 接口调用: RPC 退出登录
    rpc_cli->logout();
    // 接口调用: RPC 断开连接
    rpc_cli->disconnect();
    // 接口调用: RTDE 退出登录
    rtde_cli->logout();
    // 接口调用: RTDE 断开连接
    rtde_cli->disconnect();
    // 接口调用: SCRIPT 退出登录
    script_cli->logout();
    // 接口调用: SCRIPT 断开连接
    script_cli->disconnect();

    return 0;
}
