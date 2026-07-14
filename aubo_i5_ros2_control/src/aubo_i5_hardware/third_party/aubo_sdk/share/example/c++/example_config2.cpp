#include "aubo_sdk/rpc.h"
#include <unordered_map>
#ifdef WIN32
#include <windows.h>
#endif

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

void exampleConfig(RpcClientPtr cli) {
  // 接口调用: 获取机器人的名字
  auto robot_name = cli->getRobotNames().front();

  // 接口调用: 获取机器人的名字
  std::string name;
  name = cli->getRobotInterface(robot_name)->getRobotConfig()->getName();
  std::cout << "获取机器人的名字：" << name << std::endl;

  // 接口调用: 获取机器人的自由度
  int dof;
  dof = cli->getRobotInterface(robot_name)->getRobotConfig()->getDof();
  std::cout << "获取机器人的自由度：" << dof << std::endl;

  // 接口调用: 获取机器人的伺服控制周期(从硬件抽象层读取)
  double cycle_time;
  cycle_time =
      cli->getRobotInterface(robot_name)->getRobotConfig()->getCycletime();
  std::cout << "获取伺服控制周期：" << cycle_time << std::endl;

  // 接口调用: 获取机器人类型代码
  std::string robot_type;
  robot_type =
      cli->getRobotInterface(robot_name)->getRobotConfig()->getRobotType();
  std::cout << "获取机器人类型代码：" << robot_type << std::endl;

  // 接口调用: 获取机器人子类型代码
  std::string sub_type;
  sub_type =
      cli->getRobotInterface(robot_name)->getRobotConfig()->getRobotSubType();
  std::cout << "获取机器人子类型代码：" << sub_type << std::endl;

  // 接口调用: 获取控制柜类型代码
  std::string control_box_type;
  control_box_type =
      cli->getRobotInterface(robot_name)->getRobotConfig()->getControlBoxType();
  std::cout << "获取控制柜类型代码：" << control_box_type << std::endl;

  // 接口调用: 设置碰撞灵敏度等级
  int level = 10;
  cli->getRobotInterface(robot_name)
      ->getRobotConfig()
      ->setCollisionLevel(level);
  std::cout << "设置碰撞灵敏度等级：" << level << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(1));

  // 接口调用: 获取碰撞灵敏度等级
  int collision_level;
  collision_level =
      cli->getRobotInterface(robot_name)->getRobotConfig()->getCollisionLevel();
  std::cout << "获取碰撞灵敏度等级：" << collision_level << std::endl;

  // 接口调用: 设置碰撞停止类型
  int type = 1;
  cli->getRobotInterface(robot_name)
      ->getRobotConfig()
      ->setCollisionStopType(type);
  std::cout << "设置碰撞停止类型：" << type << std::endl;

  // 接口调用: 获取碰撞停止类型
  int collision_stop_type;
  collision_stop_type = cli->getRobotInterface(robot_name)
                            ->getRobotConfig()
                            ->getCollisionStopType();
  std::cout << "获取碰撞停止类型：" << collision_stop_type << std::endl;

  // 接口调用: 获取机器人DH参数的理论值
  std::unordered_map<std::string, std::vector<double>> kin_param1;
  kin_param1 = cli->getRobotInterface(robot_name)
                   ->getRobotConfig()
                   ->getKinematicsParam(true);
  std::cout << "获取机器人DH参数的理论值：" << std::endl;
  for (auto it = kin_param1.begin(); it != kin_param1.end(); it++) {
    std::cout << it->first << ": ";
    int size = it->second.size();
    for (int i = 0; i < size; i++) {
      std::cout << it->second.at(i) << " ";
    }
    std::cout << std::endl;
  }

  // 接口调用: 获取机器人DH参数的实际值
  std::unordered_map<std::string, std::vector<double>> kin_param2;
  kin_param2 = cli->getRobotInterface(robot_name)
                   ->getRobotConfig()
                   ->getKinematicsParam(false);
  std::cout << "获取机器人DH参数的实际值：" << std::endl;
  for (auto it = kin_param2.begin(); it != kin_param2.end(); it++) {
    std::cout << it->first << ": ";
    int size = it->second.size();
    for (int i = 0; i < size; i++) {
      std::cout << it->second.at(i) << " ";
    }
    std::cout << std::endl;
  }

  // 接口调用: 获取指定温度下的DH参数补偿值
  std::unordered_map<std::string, std::vector<double>> kin_compensate;
  double temperature = 20;
  kin_compensate = cli->getRobotInterface(robot_name)
                       ->getRobotConfig()
                       ->getKinematicsCompensate(temperature);
  std::cout << "获取机器人DH参数补偿值：" << std::endl;
  for (auto it = kin_compensate.begin(); it != kin_compensate.end(); it++) {
    std::cout << it->first << ": ";
    int size = it->second.size();
    for (int i = 0; i < size; i++) {
      std::cout << it->second.at(i) << " ";
    }
    std::cout << std::endl;
  }

  // 接口调用: 获取可用的末端力矩传感器的名字
  std::vector<std::string> tcp_force_sensor_names;
  tcp_force_sensor_names = cli->getRobotInterface(robot_name)
                               ->getRobotConfig()
                               ->getTcpForceSensorNames();
  printVec<std::string>(tcp_force_sensor_names, "获取末端力矩传感器的名字");

  // 接口调用: 获取安全参数校验码 CRC32
  uint32_t check_sum;
  check_sum = cli->getRobotInterface(robot_name)
                  ->getRobotConfig()
                  ->getSafetyParametersCheckSum();
  std::cout << "获取安全参数校验码 CRC32：" << check_sum << std::endl;

  // 接口调用: 获取关节最大位置（物理极限）
  std::vector<double> joint_max_positions;
  joint_max_positions = cli->getRobotInterface(robot_name)
                            ->getRobotConfig()
                            ->getJointMaxPositions();
  printVec<double>(joint_max_positions, "获取关节最大位置（物理极限）");

  // 接口调用: 获取关节最小位置（物理极限）
  std::vector<double> joint_min_positions;
  joint_min_positions = cli->getRobotInterface(robot_name)
                            ->getRobotConfig()
                            ->getJointMinPositions();
  printVec<double>(joint_min_positions, "获取关节最小位置（物理极限）");

  // 接口调用: 获取关节最大速度（物理极限）
  std::vector<double> joint_max_speeds;
  joint_max_speeds =
      cli->getRobotInterface(robot_name)->getRobotConfig()->getJointMaxSpeeds();
  printVec<double>(joint_max_speeds, "获取关节最大速度（物理极限）");

  // 接口调用: 获取关节最大加速度（物理极限）
  std::vector<double> joint_max_acc;
  joint_max_acc = cli->getRobotInterface(robot_name)
                      ->getRobotConfig()
                      ->getJointMaxAccelerations();
  printVec<double>(joint_max_acc, "获取关节最大加速度（物理极限）");

  // 接口调用:获取机器人的重力加速度
  std::vector<double> gravity;
  gravity = cli->getRobotInterface(robot_name)->getRobotConfig()->getGravity();
  printVec<double>(gravity, "获取机器人的重力加速度");

  // 接口调用: 设置TCP偏移
  std::vector<double> tcp_offset1 = {-0.1, 0.2, 0.3, 3.14, 0.0, 1.57};
  cli->getRobotInterface(robot_name)
      ->getRobotConfig()
      ->setTcpOffset(tcp_offset1);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  printVec<double>(tcp_offset1, "设置TCP偏移为");

  // 接口调用: 获取TCP偏移
  std::vector<double> tcp_offset2;
  tcp_offset2 =
      cli->getRobotInterface(robot_name)->getRobotConfig()->getTcpOffset();
  printVec<double>(tcp_offset2, "获取TCP偏移");

  // 接口调用: 设置负载
  double mass1 = 2.5;
  std::vector<double> cog1(3, 0.0);
  std::vector<double> aom1(3, 0.0);
  std::vector<double> inertia1(6, 0.0);
  cli->getRobotInterface(robot_name)
      ->getRobotConfig()
      ->setPayload(mass1, cog1, aom1, inertia1);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "设置末端负载为：" << std::endl;
  std::cout << "mass：" << mass1 << std::endl;
  printVec<double>(cog1, "cog");
  printVec<double>(aom1, "aom");
  printVec<double>(inertia1, "intertia");

  // 接口调用: 获取末端负载
  auto payload =
      cli->getRobotInterface(robot_name)->getRobotConfig()->getPayload();
  std::cout << "获取末端负载：" << std::endl;
  std::cout << "mass：" << std::get<0>(payload) << std::endl;
  std::vector<double> cog2 = std::get<1>(payload);
  printVec<double>(cog2, "cog");
  std::vector<double> aom2 = std::get<2>(payload);
  printVec<double>(aom2, "aom");
  std::vector<double> inertia2 = std::get<3>(payload);
  printVec<double>(inertia2, "intertia");

  // 接口调用: 获取关节摩擦力参数
  auto joint_friction_param =
      cli->getRobotInterface(robot_name)
          ->getRobotConfig()
          ->getHardwareCustomParameters("joint_friction_params");
  std::cout << "关节摩擦力参数: " << joint_friction_param << std::endl;

}

#define ip_local "127.0.0.1"

/**
 * 功能: 获取机械臂配置信息
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、登录
 * 第二步: 获取机械臂相关的配置信息
 * 第三步: 退出登录、断开 RPC 连接
 */
int main(int argc, char **argv) {
#ifdef WIN32
  // 将Windows控制台输出代码页设置为 UTF-8
  SetConsoleOutputCP(CP_UTF8);
#endif

  auto rpc_cli = std::make_shared<RpcClient>();
  // 接口调用: 设置 RPC 超时
  rpc_cli->setRequestTimeout(1000);
  // 接口调用: 连接到 RPC 服务
  rpc_cli->connect(ip_local, 30004);
  // 接口调用: 登录
  rpc_cli->login("aubo", "123456");

  // 获取机械臂相关的配置信息
  exampleConfig(rpc_cli);

  // 接口调用: 退出登录
  rpc_cli->logout();
  // 接口调用: 断开连接
  rpc_cli->disconnect();

  return 0;
}
