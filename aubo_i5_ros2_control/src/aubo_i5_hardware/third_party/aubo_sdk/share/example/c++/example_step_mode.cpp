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
    EXIT_PROGRAM      // 退出程序
};

// 定义结构体表示用户输入
struct UserInput
{
    ActionType actionType;  //　动作类型
    char axis;              //　坐标轴
    bool positiveDirection; //　坐标轴方向
};

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

std::vector<double> selectedCoordinate(CoordinateSystem coordinate,
                                       RpcClientPtr impl,
                                       std::vector<double> shift,
                                       std::vector<double> pose)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = impl->getRobotNames().front();

    std::vector<double> target_pose(6, 0.);

    switch (coordinate) {
    case BASE_COORDINATE_SYSTEM:
        // 基座标系下的示教运动, 基于固定坐标系旋转
        target_pose = impl->getMath()->poseTrans(shift, pose);
        break;
    case TOOL_COORDINATE_SYSTEM:
        // 末端坐标系, 基于末端坐标系旋转
        target_pose = impl->getMath()->poseTrans(pose, shift);
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

        user_on_base[0] = 0;
        user_on_base[1] = 0;
        user_on_base[2] = 0;

        auto user_on_base_inv = impl->getMath()->poseInverse(user_on_base);

        // 接口调用:获取 TCP 在用户坐标系下的当前位姿
        auto tcp_on_user = impl->getMath()->poseTrans(user_on_base_inv, pose);

        // 基于固定坐标系做姿态变换
        auto utt = impl->getMath()->poseTrans(shift, tcp_on_user);
        target_pose = impl->getMath()->poseTrans(user_on_base, utt);
        break;
    }
    return target_pose;
}

void exampleStepMode(RpcClientPtr impl)
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
            std::cout << "x+表示x轴正方向位置步进，x-"
                         "表示x轴负方向位置步进，y+"
                         "表示y轴正方向位置步进，y-"
                         "表示y轴负方向位置步进，z+表示z轴正方向位置步进，z-"
                         "表示z轴负方向位置步进，rx+"
                         "表示x轴正方向做姿态步进，rx-"
                         "表示x轴负方向做姿态步进，ry+"
                         "表示y轴正方向做姿态步进，ry-"
                         "表示y轴负方向做姿态步进，rz+"
                         "表示z轴正方向姿态步进，rz-"
                         "表示z轴负方向姿态步进，exit表示退出循"
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

        // 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
        std::vector<double> tcp_offset(6, 0.0);
        robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

        // 接口调用: 设置机械臂的速度比率
        robot_interface->getMotionControl()->setSpeedFraction(0.75);

        UserInput userInput = keymap[input_axis];

        std::vector<double> shift(6, 0.);

        // 位置步长，单位m
        double pos_dist = 0.005;
        pos_dist = userInput.positiveDirection ? pos_dist : -pos_dist;

        // 姿态步长，单位rad
        double ori_dist = 0.1;
        ori_dist = userInput.positiveDirection ? ori_dist : -ori_dist;

        // 接口调用: 获取TCP在Base坐标系下的当前位姿
        std::vector<double> current_tcp_on_base(6, 0.0);
        current_tcp_on_base = robot_interface->getRobotState()->getTcpPose();

        // 目标位姿
        std::vector<double> target_pose_on_base(6, 0.);

        if (userInput.actionType == EXIT_PROGRAM) {
            // 退出循环
            continue_loop = false;

        } else if (userInput.actionType == POSITION_MOVE) {
            // 位置移动
            switch (userInput.axis) {
            case 'x':
                shift[0] = pos_dist;
                break;
            case 'y':
                shift[1] = pos_dist;
                break;
            case 'z':
                shift[2] = pos_dist;
                break;
            }

            target_pose_on_base = selectedCoordinate(
                selected_coord, impl, shift, current_tcp_on_base);

            robot_interface->getMotionControl()->moveLine(target_pose_on_base,
                                                          1.2, 0.25, 0, 0);

            // 阻塞
            int ret = waitArrival(robot_interface);
            if (ret == 0) {
                std::cout << "直线运动到目标位置成功！" << std::endl;
            } else {
                std::cout << "直线运动到目标位置失败！" << std::endl;
            }
        } else if (userInput.actionType == ORIENTATION_MOVE) {
            // 姿态旋转
            switch (userInput.axis) {
            case 'x':
                shift[3] = ori_dist;
                break;
            case 'y':
                shift[4] = ori_dist;
                break;
            case 'z':
                shift[5] = ori_dist;
                break;
            }

            target_pose_on_base = selectedCoordinate(
                selected_coord, impl, shift, current_tcp_on_base);

            target_pose_on_base[0] = current_tcp_on_base[0];
            target_pose_on_base[1] = current_tcp_on_base[1];
            target_pose_on_base[2] = current_tcp_on_base[2];

            robot_interface->getMotionControl()->moveLine(target_pose_on_base,
                                                          1.2, 0.25, 0, 0);
            // 阻塞
            int ret = waitArrival(robot_interface);
            if (ret == 0) {
                std::cout << "直线运动到目标位置成功！" << std::endl;
            } else {
                std::cout << "直线运动到目标位置失败！" << std::endl;
            }
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

    // 在不同坐标系下的步进
    exampleStepMode(rpc_cli);

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
