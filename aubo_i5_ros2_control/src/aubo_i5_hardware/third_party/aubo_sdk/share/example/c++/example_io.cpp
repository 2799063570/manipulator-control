#include "aubo_sdk/rpc.h"
#include <bitset>
#ifdef WIN32
#include <Windows.h>
#endif

using namespace arcs::aubo_sdk;

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

// 数字输入——无触发动作
void exampleStandardDigitalInput1(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取数字输入数量
    int di_num = robot->getIoControl()->getStandardDigitalInputNum();
    std::cout << "数字输入数量: " << di_num << std::endl;

    // 接口调用: 设置所有的数字输入的动作为无触发
    robot->getIoControl()->setDigitalInputActionDefault();

    // 打印所有的数字输入值
    for (int i = 0; i < di_num; i++) {
        // 接口调用: 获取数字输入值
        auto input_value = robot->getIoControl()->getStandardDigitalInput(i);
        std::cout << "管脚" << i << "数字输入值:" << input_value << std::endl;
    }
}

// 数字输入——触发动作为拖动示教
void exampleStandardDigitalInput2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 设置DI00的动作为拖动示教。
    StandardInputAction input_action = StandardInputAction::Handguide;
    robot->getIoControl()->setStandardDigitalInputAction(0, input_action);

    std::cout << "说明:当DI00为高电平(DI00和0V短接)时,机器人进入拖动示教。"
              << std::endl;
    std::cout << "当DI00为低电平(DI00和0V断开)时,机器人退出拖动示教。"
              << std::endl;
}

// 数字输出——无触发动作，用户可设置输出值
void exampleStandardDigitalOutput1(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取数字输出数量
    int do_num = robot->getIoControl()->getStandardDigitalOutputNum();
    std::cout << "数字输出数量: " << do_num << std::endl;

    // 接口调用: 设置所有的数字输出的动作为无触发。
    // 当数字输出的动作为无触发时，用户可设置数字输出值。
    robot->getIoControl()->setDigitalOutputRunstateDefault();

    // 接口调用: 设置DO00为高电平
    robot->getIoControl()->setStandardDigitalOutput(0, true);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取DO00的值
    bool value = robot->getIoControl()->getStandardDigitalOutput(0);
    std::cout << "DO00的值:" << (value ? "高电平" : "低电平") << std::endl;
}

// 数字输出——设置触发动作为拖动示教，控制器自动设置输出值
void exampleStandardDigitalOutput2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取数字输出数量
    int do_num = robot->getIoControl()->getStandardDigitalOutputNum();
    std::cout << "数字输出数量: " << do_num << std::endl;

    // 接口调用: 设置DO00的动作为拖动示教。
    // 当机器人进入拖动示教时,DO00的值为高电平。
    // 当机器人退出拖动示教时,DO00的值为低电平。
    // 此时，用户不可设置DO00的值。
    StandardOutputRunState runstate = StandardOutputRunState::Handguiding;
    robot->getIoControl()->setStandardDigitalOutputRunstate(0, runstate);

    // 接口调用: 进入拖动示教
    robot->getRobotManage()->freedrive(true);

    // 等待进入拖动示教模式
    int i = 0;
    while (robot->getRobotManage()->isFreedriveEnabled()) {
        if (i++ > 5) {
            std::cerr << "拖动示教模式使能失败" << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "进入拖动示教成功" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取DO00的值
    bool value = robot->getIoControl()->getStandardDigitalOutput(0);
    std::cout << "DO00的值:" << (value ? "高电平" : "低电平") << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    // 接口调用: 退出拖动示教
    robot->getRobotManage()->freedrive(false);

    // 等待退出拖动示教模式
    i = 0;
    while (robot->getRobotManage()->isFreedriveEnabled()) {
        if (i++ > 5) {
            std::cerr << "拖动示教模式失能失败" << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "退出拖动示教成功" << std::endl;

    // 接口调用: 获取DO00的值
    value = robot->getIoControl()->getStandardDigitalOutput(0);
    std::cout << "DO00的值:" << (value ? "高电平" : "低电平") << std::endl;
}

// 模拟输入
void exampleStandardAnalogInput(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取模拟输入数量
    int ai_num = robot->getIoControl()->getStandardAnalogInputNum();
    std::cout << "模拟输入数量: " << ai_num << std::endl;

    // 打印所有的模拟输入值
    for (int i = 0; i < ai_num; i++) {
        // 接口调用: 获取模拟输入值
        auto input_value = robot->getIoControl()->getStandardAnalogInput(i);
        std::cout << "管脚" << i << "模拟输入值:" << input_value << std::endl;
    }
}

// 模拟输出——无触发动作，用户设置输出值
void exampleStandardAnalogOutput1(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取标准模拟输出数量
    int ao_num = robot->getIoControl()->getStandardAnalogOutputNum();
    std::cout << "模拟输出数量: " << ao_num << std::endl;

    // 接口调用：设置所有的模拟输出动作为无触发。
    // 当模拟输出的动作为无触发时，用户可设置模拟输出值。
    StandardOutputRunState output_runstate = StandardOutputRunState::None;
    for (int i = 0; i < ao_num; i++) {
        robot->getIoControl()->setStandardAnalogOutputRunstate(i,
                                                               output_runstate);
    }

    // 接口调用: 设置AO0的输出电压为5V
    double value = 5.0;
    robot->getIoControl()->setStandardAnalogOutput(0, value);
    std::cout << "设置AO0的输出电压:" << value << " V" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取AO0的输出电压
    value = robot->getIoControl()->getStandardAnalogOutput(0);
    std::cout << "获取AO0的输出电压:" << value << " V" << std::endl;
}

// 模拟输出——设置触发动作为拖动示教器时最大，控制器自动设置输出值
void exampleStandardAnalogOutput2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取标准模拟输出数量
    int ao_num = robot->getIoControl()->getStandardAnalogOutputNum();
    std::cout << "模拟输出数量: " << ao_num << std::endl;

    // 接口调用：设置AO0的动作为拖动示教时最大
    // 当机器人进入拖动示教时，AO0的值最大。
    // 当机器人退出拖动示教时，AO0的值为0。
    // 此时，用户不可设置AO0的值。
    StandardOutputRunState output_runstate =
        StandardOutputRunState::Handguiding;
    robot->getIoControl()->setStandardAnalogOutputRunstate(0, output_runstate);

    // 接口调用: 进入拖动示教
    robot->getRobotManage()->freedrive(true);

    // 等待进入拖动示教模式
    int i = 0;
    while (robot->getRobotManage()->isFreedriveEnabled()) {
        if (i++ > 5) {
            std::cerr << "拖动示教模式使能失败" << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "进入拖动示教成功" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取AO0的输出电压
    double value = robot->getIoControl()->getStandardAnalogOutput(0);
    std::cout << "获取AO0的输出电压:" << value << " V" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    // 接口调用: 退出拖动示教
    robot->getRobotManage()->freedrive(false);

    // 等待退出拖动示教模式
    i = 0;
    while (robot->getRobotManage()->isFreedriveEnabled()) {
        if (i++ > 5) {
            std::cerr << "拖动示教模式失能失败" << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "退出拖动示教成功" << std::endl;

    // 接口调用: 获取AO0的输出电压
    value = robot->getIoControl()->getStandardAnalogOutput(0);
    std::cout << "获取AO0的输出电压:" << value << " V" << std::endl;
}

// 工具数字输入——无触发动作
void exampleToolDigitalInput1(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取工具数字IO的数量，包含输入和输出
    int tool_io_num = robot->getIoControl()->getToolDigitalInputNum();
    std::cout << "获取工具数字IO的数量(包含输入和输出)：" << tool_io_num
              << std::endl;

    // 接口调用: 设置TOOL_IO[0]为输入
    bool isInput = true;
    robot->getIoControl()->setToolIoInput(0, isInput);
    std::this_thread::sleep_for(std::chrono::seconds(0));
    std::cout << "设置TOOL_IO[0]的类型为:" << (isInput ? "输入" : "输出")
              << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取TOOL_IO[0]的类型
    isInput = robot->getIoControl()->isToolIoInput(0);
    std::cout << "获取TOOL_IO[0]的类型为:" << (isInput ? "输入" : "输出")
              << std::endl;

    // 接口调用: 设置TOOL_IO[0]的动作为无触发
    StandardInputAction input_action = StandardInputAction::Default;
    robot->getIoControl()->setToolDigitalInputAction(0, input_action);

    // 接口调用: 获取TOOL_IO[0]的值
    bool value = robot->getIoControl()->getToolDigitalInput(0);
    std::cout << "获取TOOL_IO[0]数字输入值:" << (value ? "高电平" : "低电平")
              << std::endl;
}

// 工具数字输入——触发动作为拖动示教
void exampleToolDigitalInput2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取工具数字IO的数量，包含输入和输出
    int tool_io_num = robot->getIoControl()->getToolDigitalInputNum();
    std::cout << "获取工具数字IO的数量(包含输入和输出)：" << tool_io_num
              << std::endl;

    // 接口调用: 设置TOOL_IO[0]为输入
    bool isInput = true;
    robot->getIoControl()->setToolIoInput(0, isInput);
    std::this_thread::sleep_for(std::chrono::seconds(0));
    std::cout << "设置TOOL_IO[0]的类型为:" << (isInput ? "输入" : "输出")
              << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取TOOL_IO[0]的类型
    isInput = robot->getIoControl()->isToolIoInput(0);
    std::cout << "获取TOOL_IO[0]的类型为:" << (isInput ? "输入" : "输出")
              << std::endl;

    // 接口调用: 设置TOOL_IO[0]的动作为拖动示教
    StandardInputAction input_action = StandardInputAction::Handguide;
    robot->getIoControl()->setToolDigitalInputAction(0, input_action);

    std::cout << "说明:当TOOL_IO[0]为高电平(TOOL_IO[0]和GND短接)时,"
                 "机器人进入拖动示教。"
              << std::endl;
    std::cout
        << "当TOOL_IO[0]为低电平(TOOL_IO[0]和GND断开)时,机器人退出拖动示教。"
        << std::endl;
}

// 工具数字输出——无触发动作，用户设置输出值
void exampleToolDigitalOutput1(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取工具数字IO的数量，包含输入和输出
    int tool_io_num = robot->getIoControl()->getToolDigitalInputNum();
    std::cout << "获取工具数字IO的数量(包含输入和输出)：" << tool_io_num
              << std::endl;

    // 接口调用: 设置TOOL_IO[0]为输出
    bool isInput = false;
    robot->getIoControl()->setToolIoInput(0, isInput);
    std::this_thread::sleep_for(std::chrono::seconds(0));
    std::cout << "设置TOOL_IO[0]的类型为:" << (isInput ? "输入" : "输出")
              << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取TOOL_IO[0]的类型
    isInput = robot->getIoControl()->isToolIoInput(0);
    std::cout << "获取TOOL_IO[0]的类型为:" << (isInput ? "输入" : "输出")
              << std::endl;

    // 接口调用: 设置TOOL_IO[0]的动作为无触发
    // 当工具数字输出的动作为无触发时，用户可设置工具数字输出值。
    StandardOutputRunState output_runstate = StandardOutputRunState::None;
    robot->getIoControl()->setToolDigitalOutputRunstate(0, output_runstate);

    // 接口调用: 设置TOOL_IO[0]为高电平
    bool value = true;
    robot->getIoControl()->setToolDigitalOutput(0, value);
    std::cout << "设置TOOL_IO[0]的值:" << (value ? "高电平" : "低电平")
              << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取TOOL_IO[0]的值
    value = robot->getIoControl()->getToolDigitalOutput(0);
    std::cout << "获取TOOL_IO[0]的值:" << (value ? "高电平" : "低电平")
              << std::endl;
}

// 工具数字输出——设置触发动作为拖动示教，控制器自动设置输出值
void exampleToolDigitalOutput2(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取工具数字IO的数量，包含输入和输出
    int tool_io_num = robot->getIoControl()->getToolDigitalInputNum();
    std::cout << "获取工具数字IO的数量(包含输入和输出)：" << tool_io_num
              << std::endl;

    // 接口调用: 设置TOOL_IO[0]为输出
    bool isInput = false;
    robot->getIoControl()->setToolIoInput(0, isInput);
    std::this_thread::sleep_for(std::chrono::seconds(0));
    std::cout << "设置TOOL_IO[0]的类型为:" << (isInput ? "输入" : "输出")
              << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取TOOL_IO[0]的类型
    isInput = robot->getIoControl()->isToolIoInput(0);
    std::cout << "获取TOOL_IO[0]的类型为:" << (isInput ? "输入" : "输出")
              << std::endl;

    // 接口调用: 设置TOOL_IO[0]的动作为拖动示教。
    // 当机器人进入拖动示教时，TOOL_IO[0]的值为高电平。
    // 当机器人退出拖动示教时，TOOL_IO[0]的值为低电平。
    // 此时，用户不可设置TOOL_IO[0]的值。
    StandardOutputRunState output_runstate =
        StandardOutputRunState::Handguiding;
    robot->getIoControl()->setToolDigitalOutputRunstate(0, output_runstate);

    // 接口调用: 进入拖动示教
    robot->getRobotManage()->freedrive(true);

    // 等待进入拖动示教模式
    int i = 0;
    while (robot->getRobotManage()->isFreedriveEnabled()) {
        if (i++ > 5) {
            std::cerr << "拖动示教模式使能失败" << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "进入拖动示教成功" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取TOOL_IO[0]的值
    bool value = robot->getIoControl()->getToolDigitalOutput(0);
    std::cout << "获取TOOL_IO[0]的值:" << (value ? "高电平" : "低电平")
              << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    // 接口调用: 退出拖动示教
    robot->getRobotManage()->freedrive(false);

    // 等待退出拖动示教模式
    i = 0;
    while (robot->getRobotManage()->isFreedriveEnabled()) {
        if (i++ > 5) {
            std::cerr << "拖动示教模式失能失败" << std::endl;
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "退出拖动示教成功" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 接口调用: 获取TOOL_IO[0]的值
    value = robot->getIoControl()->getToolDigitalOutput(0);
    std::cout << "获取TOOL_IO[0]的值:" << (value ? "高电平" : "低电平")
              << std::endl;
}

// 工具模拟输入
void exampleToolAnalogInput(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取工具端模拟输入数量
    int tool_ai_num = robot->getIoControl()->getToolAnalogInputNum();
    std::cout << "工具端模拟输入数量: " << tool_ai_num << std::endl;

    // 接口调用: 获取所有工具模拟输入值
    for (int i = 0; i < tool_ai_num; i++) {
        double value = robot->getIoControl()->getToolAnalogInput(i);
        std::cout << "TOOL_AI[" << i << "]的值:" << value << std::endl;
    }
}

// 安全IO
void exampleSafetyIO(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取安全IO输入和输出数量
    int safety_input_num =
        robot->getIoControl()->getConfigurableDigitalInputNum();
    int safety_output_num =
        robot->getIoControl()->getConfigurableDigitalOutputNum();

    std::cout << "安全IO的输入数量: " << safety_input_num << std::endl;
    std::cout << "安全IO的输出数量: " << safety_output_num << std::endl;

    // 获取安全IO输入的值
    for (int i = 0; i < safety_input_num; i++) {
        // 接口调用: 获取安全IO输入的值
        auto value = robot->getIoControl()->getConfigurableDigitalInput(i);
        std::cout << "管脚" << i << "安全IO输入的值:" << value << std::endl;
    }

    // 获取安全IO输出的值
    for (int i = 0; i < safety_output_num; i++) {
        // 接口调用: 获取安全IO输出的值
        auto value = robot->getIoControl()->getConfigurableDigitalOutput(i);
        std::cout << "管脚" << i << "安全IO输出的值:" << value << std::endl;
    }
}

// 联动IO
void exampleStaticLinkIO(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
    auto robot = cli->getRobotInterface(robot_name);

    // 接口调用: 获取联动IO输入和输出数量
    int link_input_num = robot->getIoControl()->getStaticLinkInputNum();
    int link_output_num = robot->getIoControl()->getStaticLinkOutputNum();
    std::cout << "联动IO的输入数量: " << link_input_num << std::endl;
    std::cout << "联动IO的输出数量: " << link_output_num << std::endl;

    // 获取联动IO输入的值
    int input_value_decimal = robot->getIoControl()->getStaticLinkInputs();
    printBit(input_value_decimal, "所有联动IO输入值", link_input_num);

    // 获取联动IO输出的值
    int output_value_decimal = robot->getIoControl()->getStaticLinkOutputs();
    printBit(output_value_decimal, "所有联动IO输出值", link_output_num);
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

    // 数字输入——无触发动作
    exampleStandardDigitalInput1(rpc_cli);

    //    // 数字输入——触发动作为拖动示教
    //    exampleStandardDigitalInput2(rpc_cli);

    // 数字输出——无触发动作，用户设置输出值
    exampleStandardDigitalOutput1(rpc_cli);

    //    // 数字输出——设置触发动作为拖动示教，控制器自动设置输出值
    //    exampleStandardDigitalOutput2(rpc_cli);

    // 模拟输入
    exampleStandardAnalogInput(rpc_cli);

    // 模拟输出——无触发动作，用户设置输出值
    exampleStandardAnalogOutput1(rpc_cli);

    //    // 模拟输出——设置触发动作为拖动示教器时最大，控制器自动设置输出值
    //    exampleStandardAnalogOutput2(rpc_cli);

    // 工具数字输入——无触发动作
    exampleToolDigitalInput1(rpc_cli);

    //    // 工具数字输入——触发动作为拖动示教
    //    exampleToolDigitalInput2(rpc_cli);

    // 工具数字输出——无触发动作，用户设置输出值
    exampleToolDigitalOutput1(rpc_cli);

    //    // 工具数字输出——设置触发动作为拖动示教，控制器自动设置输出值
    //    exampleToolDigitalOutput2(rpc_cli);

    // 工具模拟输入
    exampleToolAnalogInput(rpc_cli);

    // 安全IO
    exampleSafetyIO(rpc_cli);

    // 联动IO
    exampleStaticLinkIO(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
