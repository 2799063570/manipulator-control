#include <math.h>
#include <thread>
#include "aubo_sdk/rpc.h"

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

/**
 * 功能: 设置运动学模型补偿参数和获取DH参数
 * 步骤:
 * 第一步: 连接 RPC 服务、机械臂登录
 * 第二步: 设置运动学模型补偿参数
 * 第三步: 获取机器人DH参数
 */
#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
    auto rpc_cli = std::make_shared<RpcClient>();
    // 接口调用: 连接到 RPC 服务
    rpc_cli->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    // 接口调用: 获取机器人的名字
    auto name = rpc_cli->getRobotNames().front();
    auto impl = rpc_cli->getRobotInterface(name);

    // 运动学模型补偿数据
    std::string value =
        "[dh_comp]\r\n"
        "alpha = [0.0, 0.000886627, -0.00305258, 0.00168948, 0.000534071, "
        "0.00557807]\r\n"
        "a = [0.0, -8.72e-05, 0.000741, -0.0001693, -0.0001688, 1.82e-05]\r\n"
        "d = [-0.0235, 0.0001258, -0.0001258, 0.0001258, 0.000586, "
        "-0.0005141]\r\n"
        "theta = [0.0, 0.000439823, -0.0181933, -0.0217206, -0.0178146, "
        "0.0]\r\n"
        "beta  = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]\r\n";

    // 接口调用: 设置运动学模型补偿参数
    impl->getRobotConfig()->setPersistentParameters(value);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 接口调用: 获取机器人DH参数
    auto dh = impl->getRobotConfig()->getKinematicsParam(true);
    for (auto iter : dh) {
        std::cout << iter.first << ":";
        for (int i = 0; i < (int)iter.second.size(); i++) {
            std::cout << iter.second.at(i) << ",";
        }
        std::cout << std::endl;
    }
}
