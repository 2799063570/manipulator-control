#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"
#include <aubo/error_stack/error_stack.h>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrival(RobotInterfacePtr impl) {
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

// 直线运动
void exampleMovel(RpcClientPtr cli) {
  // 关节角，单位: 弧度
  std::vector<double> joint_angle = {0.0 * (M_PI / 180),   -15.0 * (M_PI / 180),
                                     100.0 * (M_PI / 180), 25.0 * (M_PI / 180),
                                     90.0 * (M_PI / 180),  0.0 * (M_PI / 180)};

  // 奇异点的位姿
  std::vector<double> pose_singularity = {1000,    1000,      1000,
                                          3.05165, 0.0324355, 1.80417};

  // 接口调用: 获取机器人的名字
  auto robot_name = cli->getRobotNames().front();

  auto robot_interface = cli->getRobotInterface(robot_name);

  // 接口调用: 设置机械臂的速度比率
  robot_interface->getMotionControl()->setSpeedFraction(0.8);

  // 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
  std::vector<double> tcp_offset(6, 0.0);
  robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

  // 接口调用: 关节运动到起始位置
  robot_interface->getMotionControl()->moveJoint(joint_angle, 80 * (M_PI / 180),
                                                 60 * (M_PI / 180), 0, 0);
  // 阻塞
  int ret = waitArrival(robot_interface);
  if (ret == 0) {
    std::cout << "关节运动到起始位置成功！" << std::endl;
  } else {
    std::cout << "关节运动到起始位置失败！" << std::endl;
  }

  // 接口调用: 直线运动到奇异点
  robot_interface->getMotionControl()->moveLine(pose_singularity, 1.2, 0.25,
                                                0.025, 0);
  ret = waitArrival(robot_interface);
  if (ret == 0) {
    std::cout << "直线运动成功！" << std::endl;
  } else {
    std::cout << "直线运动失败！" << std::endl;
  }
}

void printlog(int level, const char *source, int code, std::string content) {
  static const char *level_names[] = {"Critical", "Error", "Warning",
                                      "Info",     "Debug", "BackTrace"};
  fprintf(stderr, "[%s] %s - %d %s\n", level_names[level], source, code,
          content.c_str());
}

/**
 * 功能: 机械臂直线运动到奇异点
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
 * 第二步: 连接 RTDE 服务、登录
 * 第三步: RTDE 设置话题、订阅话题来打印error_stack中的日志信息
 * 第四步: 设置运动速度比率和工具中心点
 * 第五步: 先关节运动到起始位置，然后再以直线运动的方式到奇异点
 * 第六步: RTDE 退出登录、断开连接
 * 第七步: RPC 退出登录、断开连接
 */

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

  auto rtde_cli = std::make_shared<RtdeClient>();
  // 接口调用: 连接到 RTDE 服务
  rtde_cli->connect(LOCAL_IP, 30010);
  // 接口调用: 登录
  rtde_cli->login("aubo", "123456");

  // 接口调用: 设置 RTDE 话题
  int topic = rtde_cli->setTopic(false, {"R1_message"}, 200, 0);
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
      std::string error_content = arcs::error_stack::errorCode2Str(msg.code);
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

  // 直线运动到奇异点
  exampleMovel(rpc_cli);

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
