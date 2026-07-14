#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <Windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

// 模板函数: 打印类型为std::vector<T>的变量
template <typename T> void printVec(std::vector<T> param, std::string name) {
  std::cout << name << ": ";

  for (int i = 0; i < param.size(); i++) {
    std::cout << param.at(i);
    if (i != param.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << std::endl;
}

// 模板函数: 打印类型为 std::vector<std::vector<T>> 的变量
template <typename T> void print2Vec(std::vector<T> param, std::string name) {
  std::cout << name << ": " << std::endl;
  for (size_t i = 0; i < param.size(); i++) {
    std::cout << "  第" << i + 1 << "组: ";
    for (size_t j = 0; j < param.at(i).size(); j++) {
      std::cout << param.at(i).at(j);

      if (j != param.at(i).size() - 1) {
        std::cout << ", ";
      } else {
        std::cout << std::endl;
      }
    }

    if ((param.size() - 1) == i) {
      std::cout << std::endl;
    }
  }
}

void exampleForwardK(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();

  // 接口调用: 设置 tcp 偏移
  std::vector<double> offset = {0.1, 0.2, 0.3, 0.0, 0.0, 0.0};
  impl->getRobotInterface(robot_name)->getRobotConfig()->setTcpOffset(offset);

  // 接口调用: 获取当前的关节角
  std::vector<double> q =
      impl->getRobotInterface(robot_name)->getRobotState()->getJointPositions();

  // 接口调用: 正解得到当前的TCP的位姿
  auto result = impl->getRobotInterface(robot_name)
                    ->getRobotAlgorithm()
                    ->forwardKinematics(q);
  printVec(std::get<0>(result), "正解得到当前的TCP的位姿");
}

void exampleForwardToolK(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();

  // 接口调用: 设置 tcp 偏移
  std::vector<double> offset = {0.1, 0.2, 0.3, 0.0, 0.0, 0.0};
  impl->getRobotInterface(robot_name)->getRobotConfig()->setTcpOffset(offset);

  // 接口调用: 获取当前的关节角
  std::vector<double> q =
      impl->getRobotInterface(robot_name)->getRobotState()->getJointPositions();

  // 接口调用: 正解得到法兰盘末端中心的位姿
  auto result = impl->getRobotInterface(robot_name)
                    ->getRobotAlgorithm()
                    ->forwardToolKinematics(q);
  printVec(std::get<0>(result), "正解得到法兰盘末端中心的位姿");
}

void exampleInverseK(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();

  // 接口调用: 设置 tcp 偏移
  std::vector<double> offset = {0.1, 0.2, 0.3, 0.0, 0.0, 0.0};
  impl->getRobotInterface(robot_name)->getRobotConfig()->setTcpOffset(offset);

  // 接口调用: 获取当前的关节角
  std::vector<double> qnear =
      impl->getRobotInterface(robot_name)->getRobotState()->getJointPositions();

  // 接口调用: 获取当前的tcp值作为目标位姿
  std::vector<double> p =
      impl->getRobotInterface(robot_name)->getRobotState()->getTcpPose();
  printVec(p, "当前TCP的位姿");

  // 接口调用: 根据TCP位姿，获得最优逆解
  auto result1 = impl->getRobotInterface(robot_name)
                     ->getRobotAlgorithm()
                     ->inverseKinematics(qnear, p);
  printVec(std::get<0>(result1), "根据TCP位姿，逆解获得最优的关节角");

  // 接口调用: 根据TCP位姿，获得所有的逆解
  auto result2 = impl->getRobotInterface(robot_name)
                     ->getRobotAlgorithm()
                     ->inverseKinematicsAll(p);
  print2Vec(std::get<0>(result2), "根据TCP位姿，逆解获得所有的关节角");
}

void exampleInverseToolK(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();

  // 接口调用: 设置 tcp 偏移
  std::vector<double> offset = {0.1, 0.2, 0.3, 0.0, 0.0, 0.0};
  impl->getRobotInterface(robot_name)->getRobotConfig()->setTcpOffset(offset);

  // 接口调用: 获取当前的关节角
  std::vector<double> qnear =
      impl->getRobotInterface(robot_name)->getRobotState()->getJointPositions();

  // 接口调用: 获取当前的法兰盘末端中心的值作为目标位姿
  std::vector<double> p =
      impl->getRobotInterface(robot_name)->getRobotState()->getToolPose();
  printVec(p, "当前法兰盘末端中心的位姿");

  // 接口调用: 根据法兰盘末端中心的位姿，获得最优逆解
  auto result1 = impl->getRobotInterface(robot_name)
                     ->getRobotAlgorithm()
                     ->inverseToolKinematics(qnear, p);
  printVec(std::get<0>(result1),
           "根据法兰盘末端中心的位姿，逆解获得最优的关节角");

  // 接口调用: 根据法兰盘末端中心的位姿，获得所有的逆解
  auto result2 = impl->getRobotInterface(robot_name)
                     ->getRobotAlgorithm()
                     ->inverseToolKinematicsAll(p);
  print2Vec(std::get<0>(result2),
            "根据法兰盘末端中心的位姿，逆解获得所有的关节角");
}

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv) {
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

  // 正解得到TCP的位姿
  exampleForwardK(rpc_cli);

  // 正解得到法兰盘末端中心的位姿
  exampleForwardToolK(rpc_cli);

  // 根据TCP位姿，获得逆解
  exampleInverseK(rpc_cli);

  // 根据法兰盘末端中心的位姿，获得逆解
  exampleInverseToolK(rpc_cli);

  // 接口调用: 退出登录
  rpc_cli->logout();
  // 接口调用: 断开连接
  rpc_cli->disconnect();

  return 0;
}
