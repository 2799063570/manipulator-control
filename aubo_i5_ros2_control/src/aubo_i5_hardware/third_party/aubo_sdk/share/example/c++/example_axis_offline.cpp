#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

#include "unistd.h"
#include <cstring>
#include <fstream>

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const std::vector<T> &vd)
{
    if (vd.size() == 0) {
        os << "<none>";
        return os;
    }
    for (size_t i = 0; i < vd.size(); i++) {
        os << vd[i];
        if (i < vd.size() - 1) {
            os << ", ";
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

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrival(RobotInterfacePtr impl)
{
    // 接口调用: 获取当前的运动指令 ID
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

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitAxesArrival(const std::vector<AxisInterfacePtr> &axis,
                    const std::vector<std::string> names,
                    const std::vector<double> way)
{
    int cnt_steady = 0;
    int cnt = 0;

    while (true) {
        for (size_t i = 0; i < names.size(); i++) {
            std::cout << "real pos: " << axis[i]->getExtAxisPosition()
                      << " cmd pos: " << way[i] << std::endl;
            if (std::abs(axis[i]->getExtAxisPosition() - way[i]) < 10e-5) {
                cnt_steady++;
            }
        }
        if (cnt_steady == (int)names.size()) {
            return 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if (cnt++ > 1000) {
            cnt = 0;
            return -1;
        }
    }

    return 0;
}

#define LOCAL_IP "172.19.19.112"

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

    rpc_cli->connect("172.19.19.112", 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    auto robot_name = rpc_cli->getRobotNames().front();

    auto robot_interface = rpc_cli->getRobotInterface(robot_name);
    // 接口调用: 设置机械臂的速度比率
    robot_interface->getMotionControl()->setSpeedFraction(1);

    auto filename = "../../example/c++/trajs/waypoints_dof-9_0-90-1.offt";
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

    std::vector<double> servo_r_j(6, 0.0);
    std::vector<double> servo_ex_j(3, 0.0);

    std::vector<AxisInterfacePtr> axis;
    auto names = rpc_cli->getAxisNames();

    axis.resize(names.size());
    for (size_t i = 0; i < names.size(); i++) {
        axis[i] = rpc_cli->getAxisInterface(names[i]);
    }

    printf("name: %s\n", names[0].c_str());
    axis[1]->followAnotherAxis(names[0], 0.0, 0.0);

    for (int i = 0; i < 6; i++) {
        servo_r_j[i] = traj[0][i];
    }

    for (int i = 0; i < 3; i++) {
        servo_ex_j[i] = traj[0][i + 6];
    }

    for (size_t i = 0; i < names.size(); i++) {
        // ervo_ex_j[i] = -3.0;
        axis[i]->moveExtJoint(servo_ex_j[i], 1.0, 1.0, 0.5);
    }

    int ret = waitAxesArrival(axis, names, servo_ex_j);
    if (ret == 0) {
        std::cout << "外部轴关节运动到路点1成功" << std::endl;
    } else {
        std::cout << "外部轴关节运动到路点1失败" << std::endl;
    }

    // 接口调用: 关节运动到轨迹文件中的第一个路点
    robot_interface->getMotionControl()->moveJoint(servo_r_j, 30 * (M_PI / 180),
                                                   30 * (M_PI / 180), 0., 0.);

    // 阻塞
    ret = waitArrival(robot_interface);
    if (ret == 0) {
        std::cout << "关节运动到轨迹文件中的第一个路点成功" << std::endl;
    } else {
        std::cout << "关节运动到轨迹文件中的第一个路点失败" << std::endl;
    }

    // 接口调用: 开启servo模式
    robot_interface->getMotionControl()->setServoModeSelect(3);
    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // 等待进入 servo 模式
    int i = 0;
    while (!robot_interface->getMotionControl()->isServoModeEnabled()) {
        if (i++ > 5) {
            std::cout << "Servo 模式使能失败! 当前servo状态为 "
                      << rpc_cli->getRobotInterface(robot_name)
                             ->getMotionControl()
                             ->isServoModeEnabled()
                      << std::endl;
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    for (size_t i = 1; i < traj.size(); i++) {
        for (size_t j = 0; j < 6; j++) {
            servo_r_j[j] = traj[i][j];
        }
        for (size_t j = 0; j < 3; j++) {
            servo_ex_j[j] = traj[i][j + 6];
        }
        std::cout << "servo_r_j: " << servo_r_j << std::endl;
        // 接口调用: 关节伺服运动
        int ret = rpc_cli->getRobotInterface(robot_name)
                      ->getMotionControl()
                      ->servoJointWithAxes(servo_r_j, servo_ex_j, 10.0, 3.1, 10,
                                           0.1, 200);
        if (ret == 2) {
            i--;
        }

        usleep(1000 * 2);
    }
    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
