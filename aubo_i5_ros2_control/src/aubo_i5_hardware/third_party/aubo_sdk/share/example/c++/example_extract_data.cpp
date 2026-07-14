#include <cstring>
#include <fstream>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#endif

#include "aubo_sdk/rpc.h"

using namespace arcs::aubo_sdk;
using namespace arcs::common_interface;

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

// 读取txt文件，查找关键词并提取数据
std::vector<double> findKeyword(std::vector<double> data,
                                std::string waypoint) {
  // 读取轨迹文件
  std::ifstream in("../trajs/path1.txt");
  // 目标关键词
  std::string target_keyword;
  target_keyword = waypoint;
  // 存储读取的一行文本
  std::string line;
  // 清空传入的数据向量
  data.clear();
  // 逐行读取文件内容
  while (std::getline(in, line)) {
    // 检查当前行是否包含目标关键词
    if (line.find(target_keyword) != std::string::npos) {
      // 定位方括号内的内容
      int start = line.find("[") + 1;
      int end = line.find("]");
      std::string first_group = line.substr(start, end - start);
      // 定义分隔符
      std::string delimiter = ",";
      // 初始化字符串查找位置
      size_t pos = 0;
      // 用于存储分隔后的数据的字符串
      std::string token;
      // 循环分隔字符串并将其转换为浮点数存储在数据向量中
      while ((pos = first_group.find(delimiter)) != std::string::npos) {
        token = first_group.substr(0, pos);
        // 对vector的容量进行限制，防止其因为关键词的定位导致输入数据超过6个
        if (data.size() < 6) {
          data.push_back(std::stod(token));
        }
        // 删除已处理的部分
        first_group.erase(0, pos + delimiter.length());
      }
      // 处理剩余的数据
      if (data.size() < 6) {
        data.push_back(std::stod(first_group));
      }
    }
  }
  return data;
}

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv) {
  auto rpc_cli = std::make_shared<RpcClient>();
  // 接口调用: 设置 RPC 超时
  rpc_cli->setRequestTimeout(1000);
  // 接口调用: 连接到 RPC 服务
  rpc_cli->connect(LOCAL_IP, 30004);
  // 接口调用: 登录
  rpc_cli->login("aubo", "123456");
  // 接口调用: 获取机器人的名字
  auto robot_name = rpc_cli->getRobotNames().front();
  auto robot_interface = rpc_cli->getRobotInterface(robot_name);

  // 接口调用: 设置机械臂的速度比率
  robot_interface->getMotionControl()->setSpeedFraction(0.3);

  std::vector<double> initial, pre_pick, pick, sleep, post_pick, home_1,
      pre_place, slide, place, slide_back, post_place, home_2;
  initial = findKeyword(initial, "initial");
  pre_pick = findKeyword(pre_pick, "pre_pick (MoveJ)");

  pick = findKeyword(pick, "pick (MoveL)");
  // 接口调用: 正解获取位姿
  std::tuple<std::vector<double>, int> pick_pose_data =
      robot_interface->getRobotAlgorithm()->forwardKinematics(pick);

  post_pick = findKeyword(post_pick, "post_pick (MoveL)");
  // 接口调用: 正解获取位姿
  std::tuple<std::vector<double>, int> post_pick_pose_data =
      robot_interface->getRobotAlgorithm()->forwardKinematics(post_pick);

  home_1 = findKeyword(home_1, "home_1 (MoveJ)");
  pre_place = findKeyword(pre_place, "pre_place (MoveJ)");

  slide = findKeyword(slide, "slide (MoveL)");
  // 接口调用: 正解获取位姿
  std::tuple<std::vector<double>, int> slide_pose_data =
      robot_interface->getRobotAlgorithm()->forwardKinematics(slide);

  place = findKeyword(place, "place (MoveL)");
  // 接口调用: 正解获取位姿
  std::tuple<std::vector<double>, int> place_pose_data =
      robot_interface->getRobotAlgorithm()->forwardKinematics(place);

  slide_back = findKeyword(slide_back, "slide_back (MoveL)");
  // 接口调用: 正解获取位姿
  std::tuple<std::vector<double>, int> slide_back_pose_data =
      robot_interface->getRobotAlgorithm()->forwardKinematics(slide_back);

  post_place = findKeyword(post_place, "post_place (MoveL)");
  // 接口调用: 正解获取位姿
  std::tuple<std::vector<double>, int> post_place_pose_data =
      robot_interface->getRobotAlgorithm()->forwardKinematics(post_place);

  home_2 = findKeyword(home_2, "home_2 (MoveJ)");

  std::vector<double> in_point = initial;
  std::vector<double> p1 = pre_pick;
  std::vector<double> p2 = std::get<0>(pick_pose_data);
  p2[2] = p2[2] + 0.112;
  std::vector<double> p3 = std::get<0>(post_pick_pose_data);
  p3[2] = p2[2] + 0.112;
  std::vector<double> p4 = home_1;
  std::vector<double> p5 = pre_place;
  std::vector<double> p6 = std::get<0>(slide_pose_data);
  p6[2] = p6[2] + 0.112;
  std::vector<double> p7 = std::get<0>(place_pose_data);
  p7[2] = p7[2] + 0.112;
  std::vector<double> p8 = std::get<0>(slide_back_pose_data);
  p8[2] = p8[2] + 0.112;
  std::vector<double> p9 = std::get<0>(post_place_pose_data);
  p9[2] = p9[2] + 0.112;
  std::vector<double> p10 = home_2;

  // 接口调用: 关节运动
  robot_interface->getMotionControl()->moveJoint(in_point, 80 * (M_PI / 180),

                                                 60 * (M_PI / 180), 0., 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "Init finish " << std::endl;

  printf("goto the first point\n");
  // 接口调用: 关节运动
  robot_interface->getMotionControl()->moveJoint(p1, 80 * (M_PI / 180),
                                                 60 * (M_PI / 180), 0., 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the first point " << std::endl;

  printf("goto the second point\n");
  // 接口调用: 直线运动
  robot_interface->getMotionControl()->moveLine(p2, 1.2, 0.25, 0.025, 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the second point " << std::endl;

  // sleep for 0.5s
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "SlEEP FOR 0.5S" << std::endl;

  printf("goto the third point\n");
  // 接口调用: 直线运动
  robot_interface->getMotionControl()->moveLine(p3, 1.2, 0.25, 0.025, 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the third point " << std::endl;

  printf("goto the fourth point\n");
  // 接口调用: 关节运动
  robot_interface->getMotionControl()->moveJoint(p4, 80 * (M_PI / 180),
                                                 60 * (M_PI / 180), 0., 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the fourth point " << std::endl;

  printf("goto the fifth point\n");
  // 接口调用: 关节运动
  robot_interface->getMotionControl()->moveJoint(p5, 1.2, 0.25, 0.025, 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the fifth point " << std::endl;

  printf("goto the sixth point\n");
  // 接口调用: 直线运动
  robot_interface->getMotionControl()->moveLine(p6, 1.2, 0.25, 0.025, 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the sixth point " << std::endl;

  printf("goto the seventh point\n");
  // 接口调用: 直线运动
  robot_interface->getMotionControl()->moveLine(p7, 1.2, 0.25, 0.025, 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the seventh point " << std::endl;

  // sleep for 0.5s
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "SlEEP FOR 0.5S" << std::endl;

  printf("goto the eighth point\n");
  // 接口调用: 直线运动
  robot_interface->getMotionControl()->moveLine(p8, 1.2, 0.25, 0.025, 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the eighth point " << std::endl;

  printf("goto the ninth point\n");
  // 接口调用: 直线运动
  robot_interface->getMotionControl()->moveLine(p9, 1.2, 0.25, 0.025, 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the ninth point " << std::endl;

  printf("goto the tenth point\n");
  // 接口调用: 关节运动
  robot_interface->getMotionControl()->moveJoint(in_point, 80 * (M_PI / 180),
                                                 60 * (M_PI / 180), 0., 0.);
  // 阻塞
  waitArrival(robot_interface);
  std::cout << "reach the tenth point " << std::endl;
  std::cout << "Finish the test " << std::endl;

  // 接口调用: 退出登录
  rpc_cli->logout();
  // 接口调用: 断开连接
  rpc_cli->disconnect();

  return 0;
}
