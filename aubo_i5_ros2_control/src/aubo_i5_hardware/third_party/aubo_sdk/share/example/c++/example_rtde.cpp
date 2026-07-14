#include <mutex>
#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"
#include "thread"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;
std::mutex rtde_mtx_;

uint64_t in_bit_lower_ = 0;
uint64_t in_bit_upper_ = 0;
int in_int_0_ = -1;
float in_float_0_ = -0.1;
double in_double_0_ = -0.1;
int16_t int16_0_ = 0;
std::vector<int16_t> vec_int16_(64, 0);
uint64_t out_bit_lower_ = 0;
uint64_t out_bit_upper_ = 0;
int out_int_0_ = -1;
float out_float_0_ = -0.1;
double out_double_0_ = -0.1;

RobotModeType robot_mode_ = RobotModeType::NoController; // 机器人状态
SafetyModeType safety_mode_ = SafetyModeType::Normal;    // 安全状态
RuntimeState runtime_state_ = RuntimeState::Stopped; // 规划器运行状态
double actual_main_voltage_ = 0;                     // 主电压
double actual_robot_voltage_ = 0;                    // 机器人电压

std::vector<double> actual_q_{ std::vector<double>(
    6, 0.) }; // 当前关节角(单位:rad)
std::vector<double> actual_qd_{ std::vector<double>(
    6, 0.) }; // 当前关节速度(单位:rad/s)
std::vector<double> actual_TCP_pose_{ std::vector<double>(
    6, 0.) }; // 当前TCP位置(单位:m)和姿态(单位:rad)
std::vector<double> actual_TCP_speed_{ std::vector<double>(
    6, 0.) }; // 当前TCP位置速度(单位:m/s)和姿态速度(单位:rad/s)

std::vector<JointStateType> joint_mode_{ std::vector<JointStateType>(
    6, JointStateType::Idle) }; // 关节
std::vector<double> joint_temperatures_{ std::vector<double>(6,
                                                             0.) }; // 关节温度
std::vector<double> actual_joint_voltage_{ std::vector<double>(
    6, 0.) }; // 关节电压
std::vector<double> actual_current_{ std::vector<double>(6, 0.) }; // 关节电流

uint64_t standard_digital_input_bits_ = 0; // 所有的数字输入值（十进制）
uint64_t standard_digital_output_bits_ = 0; // 所有的数字输出值（十进制）
uint64_t tool_digital_input_bits_ = 0; // 所有的工具数字值（十进制）
uint64_t tool_digital_output_bits_ = 0; // 所有的工具数字值（十进制）
uint64_t configurable_digital_input_bits_ = 0; // 所有的安全输入值（十进制）
uint64_t configurable_digital_output_bits_ = 0; // 所有的安全输出值（十进制）
uint64_t link_digital_input_bits_ = 0; // 所有的联动输入值（十进制）
uint64_t link_digital_output_bits_ = 0; // 所有的联动输出值（十进制）
int standard_digital_input_num_ = 0;    // 数字输入数量
int standard_digital_output_num_ = 0;   // 数字输出数量
int tool_digital_input_num_ = 0;        // 工具数字输入数量
int tool_digital_output_num_ = 0;       // 工具数字输出数量
int configurable_digital_input_num_ = 0;            // 安全输入数量
int configurable_digital_output_num_ = 0;           // 安全输出数量
int link_digital_input_num_ = 0;                    // 联动输入数量
int link_digital_output_num_ = 0;                   // 联动输出数量
std::vector<double> standard_analog_input_values_;  // 所有的模拟输入值
std::vector<double> standard_analog_output_values_; // 所有的模拟输出值
std::vector<double> tool_analog_input_values_; // 所有的工具模拟输入值
std::vector<double> tool_analog_output_values_; // 所有的工具模拟输出值

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

// 打印二进制位
void printBit(uint64_t v, std::string name, int bit_length)
{
    std::cout << "@:" << name << std::endl;

    for (int i = 0; i < bit_length; i++) {
        std::cout << bit_value(v, i) << ",";
    }
    std::cout << std::endl;
}

// RTDE发布
void examplePublish(RtdeClientPtr cli)
{
    // 接口调用: 设置话题
    int topic1 = cli->setTopic(
        true,
        { "input_bit_registers0_to_31", "input_bit_registers32_to_63",
          "input_bit_registers64_to_127", "input_int_registers_0",
          "input_float_registers_0", "input_double_registers_0",
          "input_int16_registers_0", "input_int16_registers0_to_63" },
        1, 1);

    // 接口调用: 发布话题
    cli->publish(topic1, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        ro.push(0x00ff);
        ro.push(0x00);
        ro.push(0x00);
        ro.push(44);
        ro.push(3.1);
        ro.push(4.1);
        ro.push(5);
        ro.push(std::vector<int16_t>(64, 1));
    });

    // 接口调用: 设置话题
    int topic2 = cli->setTopic(true, { "R1_standard_digital_output" }, 1, 2);

    // 接口调用: 发布话题
    cli->publish(topic2, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        // 设置DO01为高电平
        ro.push(std::tuple<int, bool>(1, true));
    });

    // 接口调用: 设置话题
    int topic3 = cli->setTopic(true, { "R1_standard_analog_output" }, 1, 3);
    // 接口调用: 发布话题
    cli->publish(topic3, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        // 设置AO0和AO1均为3.3
        ro.push(std::vector<double>(2, 3.3));
    });

    // 接口调用: 设置话题
    int topic4 = cli->setTopic(true, { "R1_speed_slider_fraction" }, 1, 4);
    // 接口调用: 发布话题
    cli->publish(topic4, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        // 设置运行速度比例为75%
        ro.push(0.75);
    });

    // 接口调用: 设置话题
    int topic5 = cli->setTopic(
        true,
        { "R1_standard_digital_output_mask", "R1_standard_digital_output" }, 1,
        5);
    // 接口调用: 发布话题
    cli->publish(topic5, [&](arcs::aubo_sdk::OutputBuilder &ro) {
        // 设置数字输出(DO)掩码为255，对应二进制是11111111,
        // 1表示DO通道打开，0表示DO通道关闭
        ro.push(255);
        // 设置数字输出(DO)的输出值为255，对应二进制是11111111,
        // 1表示高电平，0表示低电平
        ro.push(255);
    });
}

// RPC获取IO数量
void exampleIoNum(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    standard_digital_input_num_ =
        robot->getIoControl()->getStandardDigitalInputNum();
    standard_digital_output_num_ =
        robot->getIoControl()->getStandardDigitalOutputNum();

    tool_digital_input_num_ = robot->getIoControl()->getToolDigitalInputNum();
    tool_digital_output_num_ = robot->getIoControl()->getToolDigitalOutputNum();

    configurable_digital_input_num_ =
        robot->getIoControl()->getConfigurableDigitalInputNum();
    configurable_digital_output_num_ =
        robot->getIoControl()->getConfigurableDigitalOutputNum();

    link_digital_input_num_ = robot->getIoControl()->getStaticLinkInputNum();
    link_digital_output_num_ = robot->getIoControl()->getStaticLinkOutputNum();
}

// RTDE订阅
void exampleSubscribe(RtdeClientPtr cli)
{
    // 接口调用: 设置话题
    int topic5 = cli->setTopic(
        false,
        { "input_bit_registers_r0_to_63", "input_bit_registers_r64_to_127",
          "input_int_registers_r0", "input_float_registers_r0",
          "input_double_registers_r0", "input_int16_registers_r0",
          "input_int16_registers_0_to_63" },
        1, 5);
    // 接口调用: 订阅
    cli->subscribe(topic5, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        in_bit_lower_ = parser.popInt64();
        in_bit_upper_ = parser.popInt64();
        in_int_0_ = parser.popInt32();
        in_float_0_ = parser.popDouble();
        in_double_0_ = parser.popDouble();
        int16_0_ = parser.popInt16();
        vec_int16_ = parser.popVectorInt16();
    });

    // 接口调用: 设置话题
    int topic6 = cli->setTopic(
        false,
        { "output_bit_registers_0_to_63", "output_bit_registers_64_to_127",
          "output_int_registers_0", "output_float_registers_0",
          "output_double_registers_0" },
        1, 6);
    // 接口调用: 订阅
    cli->subscribe(topic6, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        out_bit_lower_ = parser.popInt64();
        out_bit_upper_ = parser.popInt64();
        out_int_0_ = parser.popInt32();
        out_float_0_ = parser.popDouble();
        out_double_0_ = parser.popDouble();
    });

    // 接口调用: 设置话题
    int topic7 =
        cli->setTopic(false,
                      { "R1_robot_mode", "R1_safety_mode", "runtime_state",
                        "R1_actual_main_voltage", "R1_actual_robot_voltage" },
                      50, 7);
    // 接口调用: 订阅
    cli->subscribe(topic7, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        robot_mode_ = parser.popRobotModeType();
        safety_mode_ = parser.popSafetyModeType();
        runtime_state_ = parser.popRuntimeState();
        actual_main_voltage_ = parser.popDouble();
        actual_robot_voltage_ = parser.popDouble();
    });

    // 接口调用: 设置话题
    int topic8 = cli->setTopic(false,
                               { "R1_actual_q", "R1_actual_qd",
                                 "R1_actual_TCP_pose", "R1_actual_TCP_speed" },
                               50, 8);
    // 接口调用: 订阅
    cli->subscribe(topic8, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        actual_q_ = parser.popVectorDouble();
        actual_qd_ = parser.popVectorDouble();
        actual_TCP_pose_ = parser.popVectorDouble();
        actual_TCP_speed_ = parser.popVectorDouble();
    });

    // 接口调用: 设置话题
    int topic9 = cli->setTopic(false,
                               {

                                   "R1_joint_mode",
                                   "R1_joint_temperatures",
                                   "R1_actual_joint_voltage",
                                   "R1_actual_current",
                               },
                               1, 9);
    // 接口调用: 订阅
    cli->subscribe(topic9, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        joint_mode_ = parser.popVectorJointStateType();
        joint_temperatures_ = parser.popVectorDouble();
        actual_joint_voltage_ = parser.popVectorDouble();
        actual_current_ = parser.popVectorDouble();
    });

    // 接口调用: 设置话题
    int topic10 = cli->setTopic(
        false,
        { "R1_standard_digital_input_bits", "R1_tool_digital_input_bits",
          "R1_configurable_digital_input_bits", "R1_link_digital_input_bits",
          "R1_standard_digital_output_bits", "R1_tool_digital_output_bits",
          "R1_configurable_digital_output_bits", "R1_link_digital_output_bits",
          "R1_standard_analog_input_values", "R1_tool_analog_input_values",
          "R1_standard_analog_output_values", "R1_tool_analog_output_values" },
        1, 10);
    // 接口调用: 订阅
    cli->subscribe(topic10, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        standard_digital_input_bits_ = parser.popInt64();
        tool_digital_input_bits_ = parser.popInt64();
        configurable_digital_input_bits_ = parser.popInt64();
        link_digital_input_bits_ = parser.popInt64();
        standard_digital_output_bits_ = parser.popInt64();
        tool_digital_output_bits_ = parser.popInt64();
        configurable_digital_output_bits_ = parser.popInt64();
        link_digital_output_bits_ = parser.popInt64();
        standard_analog_input_values_ = parser.popVectorDouble();
        tool_analog_input_values_ = parser.popVectorDouble();
        standard_analog_output_values_ = parser.popVectorDouble();
        tool_analog_output_values_ = parser.popVectorDouble();
    });
}

// 打印更新信息
void update()
{
    std::unique_lock<std::mutex> lck(rtde_mtx_);
    printBit(in_bit_lower_, "input_bit_registers_0_to_63", 64);
    printBit(in_bit_upper_, "input_bit_registers_64_to_127", 64);
    printBit(out_bit_lower_, "output_bit_registers_0_to_63", 64);
    printBit(out_bit_upper_, "output_bit_registers_64_to_127", 64);

    std::cout << std::endl << "input_int_registers_0" << std::endl;
    std::cout << in_int_0_;

    std::cout << std::endl << "input_float_registers_0" << std::endl;
    std::cout << in_float_0_;

    std::cout << std::endl << "input_double_registers_0" << std::endl;
    std::cout << in_double_0_ << std::endl;

    std::cout << std::endl << "output_int_registers_0" << std::endl;
    std::cout << out_int_0_;

    std::cout << std::endl << "output_float_registers_0" << std::endl;
    std::cout << out_float_0_;

    std::cout << std::endl << "output_double_registers_0" << std::endl;
    std::cout << out_double_0_;

    printSingle<RobotModeType>(robot_mode_, "机器人状态");
    printSingle<SafetyModeType>(safety_mode_, "安全状态");
    printSingle<RuntimeState>(runtime_state_, "规划器运行状态");
    printSingle<double>(actual_main_voltage_, "主电压");
    printSingle(actual_robot_voltage_, "机器人电压");

    printVec<double>(actual_q_, "当前关节角(单位:弧度)");
    printVec<double>(actual_qd_, "当前关节速度(单位:rad/s)");
    printVec<double>(actual_TCP_pose_, "当前TCP位置(单位:m)和姿态(单位:rad)");
    printVec<double>(actual_TCP_speed_,
                     "当前TCP位置速度(单位:m/s)和姿态速度(单位:rad/s)");

    for (size_t i = 0; i < joint_mode_.size(); i++) {
        if (0 == i) {
            std::cout << "@:关节状态" << std::endl;
        }

        std::cout << joint_mode_.at(i) << ",";

        if ((joint_mode_.size() - 1) == i) {
            std::cout << std::endl;
        }
    }
    printVec<double>(joint_temperatures_, "关节温度");
    printVec<double>(actual_joint_voltage_, "关节电压");
    printVec<double>(actual_current_, "关节电流");

    printBit(standard_digital_input_bits_, "数字输入值",
             standard_digital_input_num_);
    printBit(standard_digital_output_bits_, "数字输出值",
             standard_digital_output_num_);
    printBit(tool_digital_input_bits_, "工具数字值(包括输入和输出)",
             tool_digital_input_num_);
    printBit(tool_digital_output_bits_, "工具数字值(包括输入和输出)",
             tool_digital_output_num_);
    printBit(configurable_digital_input_bits_, "安全输入值",
             configurable_digital_input_num_);
    printBit(configurable_digital_output_bits_, "安全输出值",
             configurable_digital_output_num_);
    printBit(link_digital_input_bits_, "联动输入值", link_digital_input_num_);
    printBit(link_digital_output_bits_, "联动输出值", link_digital_output_num_);
    printVec<double>(standard_analog_input_values_, "模拟输入值");
    printVec<double>(tool_analog_input_values_, "工具模拟输入值");
    printVec<double>(standard_analog_output_values_, "模拟输出值");
    printVec<double>(tool_analog_output_values_, "工具模拟输出值");
}

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif
    auto rtde_cli = std::make_shared<RtdeClient>();
    // 接口调用: 连接到 RTDE 服务
    rtde_cli->connect(LOCAL_IP, 30010);
    // 接口调用: 登录
    rtde_cli->login("aubo", "123456");

    auto rpc_cli = std::make_shared<RpcClient>();
    // 接口调用: 设置 RPC 超时
    rpc_cli->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc_cli->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    // RTDE发布
    examplePublish(rtde_cli);

    // RPC获取IO数量
    exampleIoNum(rpc_cli);

    // RTDE订阅
    exampleSubscribe(rtde_cli);

    while (1) {
        // 打印更新信息
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
