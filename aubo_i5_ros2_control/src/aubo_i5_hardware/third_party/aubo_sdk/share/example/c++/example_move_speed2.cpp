#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"
#include <aubo/error_stack/error_stack.h>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 模板函数: 打印类型为std::vector<T>的变量
template <typename T>
void printVec(std::vector<T> param, std::string name)
{
    std::cout << name << ": ";

    for (int i = 0; i < param.size(); i++) {
        std::cout << param.at(i);
        if (i != param.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

// 定义坐标系类型的枚举
enum CoordinateSystem
{
    BASE_COORDINATE_SYSTEM = 1,
    TOOL_COORDINATE_SYSTEM = 2,
    USER_COORDINATE_SYSTEM = 3
};

// 定义枚举类型表示动作类型
enum ActionType
{
    POSITION_MOVE,    // 位置移动
    ORIENTATION_MOVE, // 姿态旋转
    STOP_MOVE,        // 停止运动
    EXIT_PROGRAM      // 退出程序
};

// 定义结构体表示用户输入
struct UserInput
{
    ActionType actionType;  //　动作类型
    char axis;              //　坐标轴
    bool positiveDirection; //　坐标轴方向
};

std::vector<double> selectedCoordinate(CoordinateSystem coordinate,
                                       RpcClientPtr impl)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = impl->getRobotNames().front();

    std::vector<double> frame(6, 0.);

    switch (coordinate) {
    case BASE_COORDINATE_SYSTEM:
        frame = std::vector<double>(6, 0.);
        break;
    case TOOL_COORDINATE_SYSTEM:
        frame =
            impl->getRobotInterface(robot_name)->getRobotState()->getTcpPose();
        frame[0] = 0;
        frame[1] = 0;
        frame[2] = 0;
        break;
    case USER_COORDINATE_SYSTEM:
        // TCP 在基坐标系下的位姿
        std::vector<double> coord_p0(6), coord_p1(6), coord_p2(6);
        coord_p0[0] = 0.49946;
        coord_p0[1] = 0.32869;
        coord_p0[2] = 0.35522;
        coord_p0[3] = 2.414;
        coord_p0[4] = 0.0;
        coord_p0[5] = 2.357;

        coord_p1[0] = 0.58381;
        coord_p1[1] = 0.41325;
        coord_p1[2] = 0.22122;
        coord_p1[3] = 2.414;
        coord_p1[4] = 0.0;
        coord_p1[5] = 2.357;

        coord_p2[0] = 0.58826;
        coord_p2[1] = 0.41772;
        coord_p2[2] = 0.46730;
        coord_p2[3] = 2.414;
        coord_p2[4] = 0.0;
        coord_p2[5] = 2.357;

        // 接口调用: 获取用户坐标系相对于基坐标系的位姿
        auto [user_on_base, ret] = impl->getMath()->calibrateCoordinate(
            { coord_p0, coord_p1, coord_p2 }, 0);

        frame = user_on_base;
        frame[0] = 0;
        frame[1] = 0;
        frame[2] = 0;
        break;
    }
    return frame;
}

// 在不同坐标系下的位置姿态示教
void exampleSpeedLine(RpcClientPtr impl)
{
    int input_coord;
    std::cout << "请选择坐标系：" << std::endl;
    std::cout << "1. 基坐标系" << std::endl;
    std::cout << "2. 工具坐标系" << std::endl;
    std::cout << "3. 用户坐标系" << std::endl;
    std::cout << "请输入选项编号：";
    std::cin >> input_coord;

    if (input_coord != 1 && input_coord != 2 && input_coord != 3) {
        std::cerr << "输入值无效" << std::endl;
        std::cerr << "以下为有效输入值: "
                     "1、2、3"
                  << std::endl;
        return;
    }

    CoordinateSystem selected_coord;

    switch (input_coord) {
    case 1:
        selected_coord = BASE_COORDINATE_SYSTEM;
        break;

    case 2:
        selected_coord = TOOL_COORDINATE_SYSTEM;
        break;

    case 3:
        selected_coord = USER_COORDINATE_SYSTEM;
        break;
    }

    // 输入字符串
    std::string input_axis;

    // 定义输入字符串和键值映
    std::map<std::string, UserInput> keymap = {
        { "x+", { POSITION_MOVE, 'x', true } },
        { "x-", { POSITION_MOVE, 'x', false } },
        { "y+", { POSITION_MOVE, 'y', true } },
        { "y-", { POSITION_MOVE, 'y', false } },
        { "z+", { POSITION_MOVE, 'z', true } },
        { "z-", { POSITION_MOVE, 'z', false } },
        { "rx+", { ORIENTATION_MOVE, 'x', true } },
        { "rx-", { ORIENTATION_MOVE, 'x', false } },
        { "ry+", { ORIENTATION_MOVE, 'y', true } },
        { "ry-", { ORIENTATION_MOVE, 'y', false } },
        { "rz+", { ORIENTATION_MOVE, 'z', true } },
        { "rz-", { ORIENTATION_MOVE, 'z', false } },
        { "s", { STOP_MOVE, 'x', true } },
        { "exit", { EXIT_PROGRAM, 'x', true } }
    };

    // 初始化循环控制变量
    bool continue_loop = true;

    int cnt = 0;
    while (continue_loop) {
        std::cout << "请输入机械臂要运动的轴: " << std::endl;
        // 显示有效输入提示
        if (cnt++ == 0) {
            std::cout << "有效输入值如下: "
                         "x+、x-、y+、y-、z+、z-、rx+、rx-、ry+、ry-、rz+"
                         "、rz-、s、exit"
                      << std::endl;
            std::cout
                << "x+表示x轴正方向做直线运动，x-"
                   "表示x轴负方向做直线运动，y+"
                   "表示y轴正方向做直线运动，y-"
                   "表示y轴负方向做直线运动，z+表示z轴正方向做直线运动，z-"
                   "表示z轴负方向做直线运动，rx+"
                   "表示x轴正方向做旋转运动，rx-"
                   "表示x轴负方向做旋转运动，ry+"
                   "表示y轴正方向做旋转运动，ry-"
                   "表示y轴负方向做旋转运动，rz+"
                   "表示z轴正方向做旋转运动，rz-"
                   "表示z轴负方向做旋转运动，s表示停止运动，exit表示退出循"
                   "环"
                << std::endl;
        }
        std::cin >> input_axis;

        // 定义有效的输入值集合
        std::unordered_set<std::string> validInputs = {
            "x+",  "x-",  "y+",  "y-",  "z+",  "z-", "rx+",
            "rx-", "ry+", "ry-", "rz+", "rz-", "s",  "exit"
        };

        if (validInputs.find(input_axis) == validInputs.end()) {
            std::cerr << "输入值无效" << std::endl;
            std::cerr << "以下为有效输入值: "
                         "x+、x-、y+、y-、z+、z-、rx+、rx-、ry+、ry-、rz+"
                         "、rz-、s、exit"
                      << std::endl;
            continue;
        }

        // 接口调用: 获取机器人的名字
        auto robot_name = impl->getRobotNames().front();

        auto robot_interface = impl->getRobotInterface(robot_name);

        // 接口调用: 设置机械臂的速度比率
        robot_interface->getMotionControl()->setSpeedFraction(0.75);

        UserInput userInput = keymap[input_axis];

        std::vector<double> speed(6, 0.);

        // speedLine速度
        double tcp_speed = 0.25;
        double a = 1.2;
        tcp_speed = userInput.positiveDirection ? tcp_speed : -tcp_speed;

        std::vector<double> frame(6, 0.);

        if (userInput.actionType == EXIT_PROGRAM) {
            // 退出循环
            continue_loop = false;
        } else if (userInput.actionType == STOP_MOVE) {
            // 接口调用: 停止位置姿态示教
            robot_interface->getMotionControl()->stopLine(10, 10);

        } else if (userInput.actionType == POSITION_MOVE) {
            frame = selectedCoordinate(selected_coord, impl);

            // 位置移动
            switch (userInput.axis) {
            case 'x':
                speed[0] = tcp_speed;
                break;
            case 'y':
                speed[1] = tcp_speed;
                break;
            case 'z':
                speed[2] = tcp_speed;
                break;
            }

            speed = impl->getMath()->poseTrans(frame, speed);
            speed[3] = 0;
            speed[4] = 0;
            speed[5] = 0;

            robot_interface->getMotionControl()->speedLine(speed, a, 100);
        } else if (userInput.actionType == ORIENTATION_MOVE) {
            frame = selectedCoordinate(selected_coord, impl);
            // 姿态旋转
            switch (userInput.axis) {
            case 'x':
                speed[0] = tcp_speed;
                break;
            case 'y':
                speed[1] = tcp_speed;
                break;
            case 'z':
                speed[2] = tcp_speed;
                break;
            }

            speed = impl->getMath()->poseTrans(frame, speed);

            speed[3] = speed[0];
            speed[4] = speed[1];
            speed[5] = speed[2];
            speed[0] = 0;
            speed[1] = 0;
            speed[2] = 0;

            robot_interface->getMotionControl()->speedLine(speed, a, 100);
        }
    }
}

// 打印日志信息
void printlog(int level, const char *source, int code, std::string content)
{
    static const char *level_names[] = { "Critical", "Error", "Warning",
                                         "Info",     "Debug", "BackTrace" };
    fprintf(stderr, "[%s] %s - %d %s\n", level_names[level], source, code,
            content.c_str());
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

    auto rtde_cli = std::make_shared<RtdeClient>();
    // 接口调用: 连接到 RTDE 服务
    rtde_cli->connect(LOCAL_IP, 30010);
    // 接口调用: 登录
    rtde_cli->login("aubo", "123456");

    // 接口调用: 设置 RTDE 话题
    int topic = rtde_cli->setTopic(false, { "R1_message" }, 200, 0);
    if (topic < 0) {
        std::cout << "设置话题失败!" << std::endl;
        return -1;
    }

    // 接口调用: 订阅话题
    rtde_cli->subscribe(topic, [](InputParser &parser) {
        arcs::common_interface::RobotMsgVector msgs;
        msgs = parser.popRobotMsgVector();
        for (size_t i = 0; i < msgs.size(); i++) {
            auto &msg = msgs[i];
            std::string error_content =
                arcs::error_stack::errorCode2Str(msg.code);
            for (auto it : msg.args) {
                auto pos = error_content.find("{}");
                if (pos != std::string::npos) {
                    error_content.replace(pos, 2, it);
                } else {
                    break;
                }
            }
            // 打印日志信息
            printlog(msg.level, msg.source.c_str(), msg.code, error_content);
        }
    });

    // 在不同坐标系下的位置姿态示教
    exampleSpeedLine(rpc_cli);

    // 接口调用: 取消话题
    rtde_cli->removeTopic(false, topic);
    // 接口调用: 退出登录
    rtde_cli->logout();
    // 接口调用: 断开连接
    rtde_cli->disconnect();

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
