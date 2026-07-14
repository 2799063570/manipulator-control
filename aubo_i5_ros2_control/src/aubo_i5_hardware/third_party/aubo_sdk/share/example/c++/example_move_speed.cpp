#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"
#include <aubo/error_stack/error_stack.h>
#include <map>
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 关节示教
void exampleSpeedJoint(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();
  auto robot_interface = impl->getRobotInterface(robot_name);

  // 接口调用: 设置机械臂的速度比率
  robot_interface->getMotionControl()->setSpeedFraction(0.3);

  // 输入字符串
  std::string input_angle;
  // 定义输入字符串和键值映射
  std::map<std::string, int> keymap;
  keymap["j1+"] = 1;
  keymap["j1-"] = 2;
  keymap["j2+"] = 3;
  keymap["j2-"] = 4;
  keymap["j3+"] = 5;
  keymap["j3-"] = 6;
  keymap["j4+"] = 7;
  keymap["j4-"] = 8;
  keymap["j5+"] = 9;
  keymap["j5-"] = 10;
  keymap["j6+"] = 11;
  keymap["j6-"] = 12;
  keymap["s"] = 13;
  keymap["exit"] = 14;

  // 初始化循环控制变量
  bool continue_loop = true;

  // speedJoint速度
  double speed = 0.2;

  int cnt = 0;
  while (continue_loop) {
    std::cout << "请输入机械臂要运动的关节角: " << std::endl;
    // 显示有效输入提示
    if (cnt++ == 0) {
      std::cout << "有效输入值如下: "
                   "j1+、j1-、j2+、j2-、j3+、j3-、j4+、j4-、j5+、j5-、j6+"
                   "、j6-、s、exit"
                << std::endl;
      std::cout << "j1+表示关节1正方向，j1-表示关节1负方向，j2+"
                   "表示关节2正方向，j2-"
                   "表示关节2负方向，j3+表示关节3正方向，j3-"
                   "，表示关节3负方向，j4+"
                   "表示关节4正方向，j4-表示关节4负方向，j5+"
                   "表示关节5正方向，j5-"
                   "表示关节5负方向，j6+表示关节6正方向，j6-"
                   "表示关节6负方向，s表示停止运动，exit表示退出循环"
                << std::endl;
    }

    std::cin >> input_angle;
    char value = keymap[input_angle];
    switch (value) {
    case 1: {
      std::vector<double> qd = {speed, 0.0, 0.0, 0.0, 0.0, 0.0};

      // 接口调用: 机械臂的关节1沿着正方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 2: {
      std::vector<double> qd = {speed * (-1), 0.0, 0.0, 0.0, 0.0, 0.0};

      // 接口调用: 机械臂的关节1沿着负方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 3: {
      std::vector<double> qd = {0.0, speed, 0.0, 0.0, 0.0, 0.0};

      // 接口调用: 机械臂的关节2沿着正方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 4: {
      std::vector<double> qd = {0.0, speed * (-1), 0.0, 0.0, 0.0, 0.0};

      // 接口调用: 机械臂的关节2沿着负方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 5: {
      std::vector<double> qd = {0.0, 0.0, speed, 0.0, 0.0, 0.0};

      // 接口调用: 机械臂的关节3沿着正方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }

    case 6: {
      std::vector<double> qd = {0.0, 0.0, speed * (-1), 0.0, 0.0, 0.0};

      // 接口调用: 机械臂的关节3沿着负方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 7: {
      std::vector<double> qd = {0.0, 0.0, 0.0, speed, 0.0, 0.0};

      // 接口调用: 机械臂的关节4沿着正方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 8: {
      std::vector<double> qd = {0.0, 0.0, 0.0, speed * (-1), 0.0, 0.0};
      // 接口调用: 机械臂的关节4沿着负方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 9: {
      std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, speed, 0.0};

      // 接口调用: 机械臂的关节5沿着正方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 10: {
      std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, speed * (-1), 0.0};

      // 接口调用: 机械臂的关节5沿着负方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 11: {
      std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, 0.0, speed};

      // 接口调用: 机械臂的关节6沿着正方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 12: {
      std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, 0.0, speed * (-1)};

      // 接口调用: 机械臂的关节6沿着负方向运动
      robot_interface->getMotionControl()->speedJoint(qd, 1.5, 100);
      break;
    }
    case 13: {
      // 接口调用: 停止关节速度跟随运动
      robot_interface->getMotionControl()->stopJoint(31);

      break;
    }
    case 14: {
      // 退出循环
      continue_loop = false;
      break;
    }
    default: {
      std::cerr << "输入值无效" << std::endl;
      std::cerr << "以下为有效输入值: "
                   "j1+、j1-、j2+、j2-、j3+、j3-、j4+、j4-、j5+、j5-、j6+"
                   "、j6-、s、exit"
                << std::endl;
    }
    }
  }
}

// 在基坐标系下的位置姿态示教
void exampleSpeedLine(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();
  auto robot_interface = impl->getRobotInterface(robot_name);

  // 接口调用: 设置机械臂的速度比率
  robot_interface->getMotionControl()->setSpeedFraction(0.3);

  // 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
  std::vector<double> tcp_offset(6, 0.0);
  robot_interface->getRobotConfig()->setTcpOffset(tcp_offset);

  // 输入字符串
  std::string input_axis;
  // 定义输入字符串和键值映射
  std::map<std::string, int> keymap;
  keymap["x+"] = 1;
  keymap["x-"] = 2;
  keymap["y+"] = 3;
  keymap["y-"] = 4;
  keymap["z+"] = 5;
  keymap["z-"] = 6;
  keymap["rx+"] = 7;
  keymap["rx-"] = 8;
  keymap["ry+"] = 9;
  keymap["ry-"] = 10;
  keymap["rz+"] = 11;
  keymap["rz-"] = 12;
  keymap["s"] = 13;
  keymap["exit"] = 14;

  // 初始化循环控制变量
  bool continue_loop = true;

  // speedLine速度
  double speed = 0.25;

  int cnt = 0;
  while (continue_loop) {
    std::cout << "请输入机械臂要运动的轴: " << std::endl;
    // 显示有效输入提示
    if (cnt++ == 0) {
      std::cout << "有效输入值如下: "
                   "x+、x-、y+、y-、z+、z-、rx+、rx-、ry+、ry-、rz+"
                   "、rz-、s、exit"
                << std::endl;
      std::cout << "x+表示x轴正方向做直线运动，x-表示x轴负方向做直线运动，y+"
                   "表示y轴正方向做直线运动，y-"
                   "表示y轴负方向做直线运动，z+表示z轴正方向做直线运动，z-"
                   "表示z轴负方向做直线运动，rx+"
                   "表示x轴正方向做旋转运动，rx-表示x轴负方向做旋转运动，ry+"
                   "表示y轴正方向做旋转运动，ry-"
                   "表示y轴负方向做旋转运动，rz+表示z轴正方向做旋转运动，rz-"
                   "表示z轴负方向做旋转运动，s表示停止运动，exit表示退出循环"
                << std::endl;
    }

    std::cin >> input_axis;
    char value = keymap[input_axis];
    switch (value) {
    case 1: {
      std::vector<double> qd = {speed, 0.0, 0.0, 0.0, 0.0, 0.0};

      // 接口调用: TCP沿着基坐标系x轴正方向做直线运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 2: {
      std::vector<double> qd = {speed * (-1), 0.0, 0.0, 0.0, 0.0, 0.0};

      // 接口调用: TCP沿着基坐标系x轴负方向做直线运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 3: {
      std::vector<double> qd = {0.0, speed, 0.0, 0.0, 0.0, 0.0};

      // 接口调用: TCP沿着基坐标系y轴正方向做直线运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 4: {
      std::vector<double> qd = {0.0, speed * (-1), 0.0, 0.0, 0.0, 0.0};

      // 接口调用: TCP沿着基坐标系y轴负方向做直线运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 5: {
      std::vector<double> qd = {0.0, 0.0, speed, 0.0, 0.0, 0.0};

      // 接口调用: TCP沿着基坐标系z轴正方向做直线运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }

    case 6: {
      std::vector<double> qd = {0.0, 0.0, speed * (-1), 0.0, 0.0, 0.0};

      // 接口调用: TCP沿着基坐标系z轴负方向做直线运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 7: {
      std::vector<double> qd = {0.0, 0.0, 0.0, speed, 0.0, 0.0};

      // 接口调用: TCP沿着基坐标系x轴正方向做旋转运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 8: {
      std::vector<double> qd = {0.0, 0.0, 0.0, speed * (-1), 0.0, 0.0};

      // 接口调用: TCP沿着基坐标系x轴负方向做旋转运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 9: {
      std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, speed, 0.0};

      // 接口调用: TCP沿着基坐标系y轴正方向做旋转运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 10: {
      std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, speed * (-1), 0.0};

      // 接口调用: TCP沿着基坐标系y轴负方向做旋转运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 11: {
      std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, 0.0, speed};

      // 接口调用: TCP沿着基坐标系z轴正方向做旋转运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 12: {
      std::vector<double> qd = {0.0, 0.0, 0.0, 0.0, 0.0, speed * (-1)};

      // 接口调用: TCP沿着基坐标系z轴负方向做旋转运动
      robot_interface->getMotionControl()->speedLine(qd, 1.2, 100);
      break;
    }
    case 13: {
      // 接口调用: 停止直线速度跟随运动
      robot_interface->getMotionControl()->stopLine(10, 10);

      break;
    }
    case 14: {
      // 退出循环
      continue_loop = false;
      break;
    }
    default: {
      std::cerr << "输入值无效" << std::endl;
      std::cerr << "以下为有效输入值: "
                   "x+、x-、y+、y-、z+、z-、rx+、rx-、ry+、ry-、rz+"
                   "、rz-、s、exit"
                << std::endl;
    }
    }
  }
}

// 打印日志信息
void printlog(int level, const char *source, int code, std::string content) {
  static const char *level_names[] = {"Critical", "Error", "Warning",
                                      "Info",     "Debug", "BackTrace"};
  fprintf(stderr, "[%s] %s - %d %s\n", level_names[level], source, code,
          content.c_str());
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

  // 关节示教
  exampleSpeedJoint(rpc_cli);

  // 在基坐标系下的位置姿态示教
  //  exampleSpeedLine(rpc_cli);

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
