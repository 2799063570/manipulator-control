#include <mutex>
#include <aubo/robot/robot_state.h>
#include "aubo_sdk/rtde.h"
#include "aubo_sdk/rpc.h"
#include "thread"

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;
using RtdeRecipeMap =
    std::unordered_map<int, arcs::common_interface::RtdeRecipe>;
std::mutex rtde_mtx_;

// topic1
int line_{ -1 };
std::vector<double> actual_q_{ std::vector<double>(6, 0.) };
std::vector<double> actual_current_{ std::vector<double>(6, 0.) };
std::vector<double> actual_TCP_pose_{ std::vector<double>(6, 0.) };
RobotModeType robot_mode_ = RobotModeType::NoController;
SafetyModeType safety_mode_ = SafetyModeType::Normal;
RuntimeState runtime_state_ = RuntimeState::Stopped;

// topic2
double actual_main_voltage_;
double actual_robot_voltage_;
std::vector<double> joint_temperatures_{ std::vector<double>(6, 0.) };
std::vector<JointStateType> joint_mode_{ std::vector<JointStateType>(
    6, JointStateType::Idle) };

// topic3
uint64_t out_bit_lower_ = 0;
uint64_t out_bit_upper_ = 0;
int out_int_0_ = -1;
float out_float_0_ = -0.1;
double out_double_0_ = -0.1;
uint64_t in_bit_lower_ = 0;
uint64_t in_bit_upper_ = 0;
int in_int_0_ = -1;
float in_float_0_ = -0.1;
double in_double_1_ = -0.1;
double motion_progress = 0;
int16_t int16_0_ = 0;
std::vector<int16_t> vec_int16_(64, 0);

template <typename T>
void printVec(std::vector<T> param, std::string name)
{
    std::cout << "@:" << name << std::endl;
    for (int i = 0; i < param.size(); i++) {
        std::cout << param.at(i) << ",";
    }
    std::cout << std::endl;
}

template <typename T>
void print2Vec(std::vector<T> param, std::string name)
{
    for (size_t i = 0; i < param.size(); i++) {
        if (0 == i) {
            std::cout << "@:" << name << " offset:" << param.at(i).offset
                      << std::endl;
        }

        for (size_t j = 0; j < param.at(i).value.size(); j++) {
            std::cout << param.at(i).value.at(j);
        }

        if ((param.size() - 1) == i) {
            std::cout << std::endl;
        }
    }
}

template <typename T>
void printVecVec(std::vector<std::vector<T>> param, std::string name)
{
    for (size_t i = 0; i < param.size(); i++) {
        if (0 == i) {
            std::cout << "@:" << name << " size:" << param.size() << std::endl;
        }

        for (size_t j = 0; j < param.at(i).size(); j++) {
            std::cout << param.at(i).at(j) << ",";
        }

        std::cout << std::endl;
    }
}

template <typename T>
void printSingle(T param, std::string name)
{
    std::cout << "@:" << name << " " << param << std::endl;
}

bool bit_value(uint64_t v, int index)
{
    return (v & ((uint64_t)1 << index)) ? true : false;
}

void printRecipe(RtdeRecipeMap &recipe)
{
    for (auto it = recipe.begin(); it != recipe.end(); ++it) {
        std::cout << "chanel:" << it->first << std::endl;
        for (auto tmp : it->second.segments) {
            std::cout << tmp << "  ";
        }
        std::cout << std::endl;
    }
}

// 配置输出
void configSubscribe(RtdeClientPtr cli)
{
    // 接口调用: 设置 topic1
    int topic1 =
        cli->setTopic(false,
                      { "R1_actual_q", "R1_actual_current", "R1_robot_mode",
                        "R1_safety_mode", "runtime_state", "line_number",
                        "R1_actual_TCP_pose", "R1_motion_progress" },
                      50, 0);
    // 接口调用: 订阅
    cli->subscribe(topic1, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        actual_q_ = parser.popVectorDouble();
        actual_current_ = parser.popVectorDouble();
        robot_mode_ = parser.popRobotModeType();
        safety_mode_ = parser.popSafetyModeType();
        runtime_state_ = parser.popRuntimeState();
        line_ = parser.popInt32();
        actual_TCP_pose_ = parser.popVectorDouble();
        motion_progress = parser.popDouble();
    });

    // 接口调用: 设置 topic2
    int topic2 =
        cli->setTopic(false,
                      { "R1_joint_temperatures", "R1_joint_mode",
                        "R1_actual_main_voltage", "R1_actual_robot_voltage" },
                      1, 1);
    // 接口调用: 订阅
    cli->subscribe(topic2, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        joint_temperatures_ = parser.popVectorDouble();
        joint_mode_ = parser.popVectorJointStateType();
        actual_main_voltage_ = parser.popDouble();
        actual_robot_voltage_ = parser.popDouble();
    });

    // 接口调用: 设置 topic3
    int topic3 = cli->setTopic(
        false,
        { "output_bit_registers_0_to_63", "output_bit_registers_64_to_127",
          "output_int_registers_0", "output_float_registers_0",
          "output_double_registers_0", "input_bit_registers_r0_to_63",
          "input_bit_registers_r64_to_127", "input_int_registers_r0",
          "input_float_registers_r0", "input_double_registers_r1",
          "input_int16_registers_r0", "input_int16_registers_0_to_63" },
        1, 12);
    // 接口调用: 订阅
    cli->subscribe(topic3, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        out_bit_lower_ = parser.popInt64();
        out_bit_upper_ = parser.popInt64();
        out_int_0_ = parser.popInt32();
        out_float_0_ = parser.popDouble();
        out_double_0_ = parser.popDouble();
        in_bit_lower_ = parser.popInt64();
        in_bit_upper_ = parser.popInt64();
        in_int_0_ = parser.popInt32();
        in_float_0_ = parser.popDouble();
        in_double_1_ = parser.popDouble();
        int16_0_ = parser.popInt16();
        vec_int16_ = parser.popVectorInt16();
    });
}

// 设置rtde输入
void setInput(RtdeClientPtr cli)
{
    // 输入
    // 单一设置输入
    // 接口调用: 发布
    //    cli->publish(arcs::common_interface::RtdeInput::input_bit_registers0_to_31,
    //                 [](arcs::aubo_sdk::OutputBuilder &ro) { ro.push(0x00ff);
    //                 });

    //    cli->publish(arcs::common_interface::RtdeInput::input_bit_registers32_to_63,
    //                 [](arcs::aubo_sdk::OutputBuilder &ro) { ro.push(0x00);
    //                 });

    //    cli->publish(
    //        arcs::common_interface::RtdeInput::input_bit_registers64_to_127,
    //        [](arcs::aubo_sdk::OutputBuilder &ro) { ro.push(0x00); });

    //    cli->publish(arcs::common_interface::RtdeInput::input_int_registers_0,
    //                 [](arcs::aubo_sdk::OutputBuilder &ro) { ro.push(12); });

    //    cli->publish(arcs::common_interface::RtdeInput::input_float_registers_0,
    //                 [](arcs::aubo_sdk::OutputBuilder &ro) { ro.push(1.11111);
    //                 });

    //    cli->publish(arcs::common_interface::RtdeInput::input_double_registers_0,
    //                 [](arcs::aubo_sdk::OutputBuilder &ro) { ro.push(2.22222);
    //                 });

    // 组合设置输入
    int topic5 = cli->setTopic(
        true,
        { "input_bit_registers0_to_31", "input_bit_registers32_to_63",
          "input_bit_registers64_to_127", "input_int_registers_0" },
        1, 5);

    cli->publish(5, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        ro.push(0x00ff);
        ro.push(0x00);
        ro.push(0x00);
        ro.push(44);
    });

    int topic6 =
        cli->setTopic(true,
                      { "input_float_registers_0", "input_double_registers_1",
                        "input_int16_registers_0" },
                      1, 6);

    cli->publish(6, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        ro.push(3.1);
        ro.push(4.1);
        ro.push(5);
    });

    int topic7 = cli->setTopic(true, { "R1_standard_digital_output" }, 1, 7);

    cli->publish(topic7, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        ro.push(std::tuple<int, bool>(1, true));
    });

    int topic8 = cli->setTopic(true, { "input_int16_registers0_to_63" }, 1, 8);

    cli->publish(topic8, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        ro.push(std::vector<int16_t>(64, 1));
    });
}

// 打印更新信息
void update()
{
    std::unique_lock<std::mutex> lck(rtde_mtx_);

    printVec<double>(actual_q_, "actual_q_");
    printVec<double>(actual_current_, "actual_current_");
    printVec<double>(actual_TCP_pose_, "actual_TCP_pose_");
    printSingle<int>((int)robot_mode_, "robot_mode_");
    printSingle<int>((int)safety_mode_, "safety_mode_");
    printSingle<int>((int)runtime_state_, "runtime_state_");
    printSingle<int>(line_, "line_");

    printVec<double>(joint_temperatures_, "joint_temperatures_");
    printSingle<double>(actual_main_voltage_, "actual_main_voltage_");
    printSingle(actual_robot_voltage_, "actual_robot_voltage_");

    for (size_t i = 0; i < joint_mode_.size(); i++) {
        if (0 == i) {
            std::cout << "@:joint_mode_" << std::endl;
        }

        std::cout << joint_mode_.at(i) << ",";

        if ((joint_mode_.size() - 1) == i) {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl << "output_bit_registers_0_to_63" << std::endl;
    for (int i = 0; i < 64; i++) {
        std::cout << bit_value(out_bit_lower_, i) << ",";
    }

    std::cout << std::endl << "output_bit_registers_64_to_127" << std::endl;
    for (int i = 0; i < 64; i++) {
        std::cout << bit_value(out_bit_upper_, i) << ",";
    }

    std::cout << std::endl << "output_int_registers_0" << std::endl;
    std::cout << out_int_0_;

    std::cout << std::endl << "output_float_registers_0" << std::endl;
    std::cout << out_float_0_;

    std::cout << std::endl << "output_double_registers_0" << std::endl;
    std::cout << out_double_0_;

    std::cout << std::endl << "input_bit_registers_0_to_63" << std::endl;
    for (int i = 0; i < 64; i++) {
        std::cout << bit_value(in_bit_lower_, i) << ",";
    }

    std::cout << std::endl << "input_bit_registers_64_to_127" << std::endl;
    for (int i = 0; i < 64; i++) {
        std::cout << bit_value(in_bit_upper_, i) << ",";
    }

    std::cout << std::endl << "input_int_registers_0" << std::endl;
    std::cout << in_int_0_;

    std::cout << std::endl << "input_float_registers_0" << std::endl;
    std::cout << in_float_0_;

    std::cout << std::endl << "input_double_registers_1" << std::endl;
    std::cout << in_double_1_ << std::endl;
}

/**
 * 功能: RTDE
 * 步骤:
 * 第一步: 连接 RPC 服务、机械臂登录
 * 第二步: 连接 RTDE 服务、机械臂登录
 * 第三步: 设置rtde输入
 * 第四步: 配置输出
 * 第五步: 打印更新信息
 */
#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
    auto rpc_cli = std::make_shared<RpcClient>();
    // 接口调用: 连接到 RPC 服务
    rpc_cli->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    auto rtde_cli = std::make_shared<RtdeClient>(2);
    // 接口调用: 连接到 RTDE UDP 服务
    rtde_cli->connect(LOCAL_IP, 30018);
    // 接口调用: 登录
    rtde_cli->login("aubo", "123456");

    // 设置rtde输入
    setInput(rtde_cli);

    // 配置输出
    configSubscribe(rtde_cli);

    std::cout << "=================output_recipe==================="
              << std::endl;
    auto output_recipe = rtde_cli->getRegisteredOutputRecipe();
    printRecipe(output_recipe);
    std::cout << "=================input_recipe===================="
              << std::endl;
    auto input_recipe = rtde_cli->getRegisteredInputRecipe();
    printRecipe(input_recipe);

    while (1) {
        // 打印更新信息
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
