#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrival(RobotInterfacePtr impl, int max_retry_count) {
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
    std::cout << "exec_id: " << exec_id << std::endl;
  }

  // 等待机械臂动作完成
  while (exec_id != -1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    exec_id = impl->getMotionControl()->getExecId();
    std::cout << "exec_id: " << exec_id << std::endl;
  }

  return 0;
}

// 等待样条运动完成
void waitMoveSplineFinished(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();

  auto robot_interface = impl->getRobotInterface(robot_name);

  // 等待样条运动开始
  while (robot_interface->getMotionControl()->getExecId() == -1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  std::cout << "样条运动开始" << std::endl;

  while (1) {
    auto id = robot_interface->getMotionControl()->getExecId();
    if (id == -1) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

// 样条运动
void exampleMoveSpline(RpcClientPtr rpc_cli)

{
  // 路点，用关节角来表示，单位: 弧度
  std::vector<double> joint_angle1 = {0.0,
                                      -15.0 * (M_PI / 180),
                                      100.0 * (M_PI / 180),
                                      25.0 * (M_PI / 180),
                                      90.0 * (M_PI / 180),
                                      0.0};

  std::vector<double> joint_angle2 = {
      19.25 * (M_PI / 180), -6.08 * (M_PI / 180), 78.87 * (M_PI / 180),
      -5.05 * (M_PI / 180), 90.0 * (M_PI / 180),  19.25 * (M_PI / 180)};

  std::vector<double> joint_angle3 = {
      28.20 * (M_PI / 180),  -12.87 * (M_PI / 180), 52.22 * (M_PI / 180),
      -24.91 * (M_PI / 180), 90.0 * (M_PI / 180),   28.2 * (M_PI / 180)};

  std::vector<double> joint_angle4 = {
      38.20 * (M_PI / 180),  -15.58 * (M_PI / 180), 58.16 * (M_PI / 180),
      -16.25 * (M_PI / 180), 90.0 * (M_PI / 180),   38.20 * (M_PI / 180)};

  std::vector<double> joint_angle5 = {
      38.20 * (M_PI / 180), -15.73 * (M_PI / 180), 91.05 * (M_PI / 180),
      16.79 * (M_PI / 180), 90.0 * (M_PI / 180),   38.20 * (M_PI / 180)};

  // 接口调用: 获取机器人的名字
  auto robot_name = rpc_cli->getRobotNames().front();

  auto robot_interface = rpc_cli->getRobotInterface(robot_name);

  // 接口调用: 设置机械臂的速度比率
  robot_interface->getMotionControl()->setSpeedFraction(0.8);

  std::cout << "添加第一个路点" << std::endl;
  // 接口调用: 添加样条运动的第一个路点
  rpc_cli->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveSpline(joint_angle1, 80 * (M_PI / 180), 60 * (M_PI / 180), 0);

  std::cout << "添加第二个路点" << std::endl;
  // 接口调用: 添加样条运动的第二个路点
  rpc_cli->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveSpline(joint_angle2, 80 * (M_PI / 180), 60 * (M_PI / 180), 0);

  std::cout << "添加第三个路点" << std::endl;
  // 接口调用: 添加样条运动的第三个路点
  rpc_cli->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveSpline(joint_angle3, 80 * (M_PI / 180), 60 * (M_PI / 180), 0);

  std::cout << "添加第四个路点" << std::endl;
  // 接口调用: 添加样条运动的第四个路点
  rpc_cli->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveSpline(joint_angle4, 80 * (M_PI / 180), 180 * (M_PI / 180), 0);

  std::cout << "添加第五个路点" << std::endl;
  // 接口调用: 添加样条运动的第五个路点
  rpc_cli->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveSpline(joint_angle5, 80 * (M_PI / 180), 60 * (M_PI / 180), 0);

  // 接口调用: 当关节角参数传入为空时，结束路点添加。
  // 等待算法计算完成后，机械臂开始执行样条运动。
  // 所以，当添加的路点数量越多时，由于算法计算的耗时时间长，
  // 机械臂可能不会立刻运动。
  rpc_cli->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveSpline({}, 80 * (M_PI / 180), 60 * (M_PI / 180), 0.005);

  // 等待样条运动完成
  waitMoveSplineFinished(rpc_cli);

  std::cout << "样条运动结束" << std::endl;
}

/**
 * 功能: 机械臂做关节空间的样条运动
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
 * 第二步: 设置运动速度比率、以样条运动的方式依次经过5个离散路点
 * 第三步: RPC 退出登录、断开连接
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

  // 样条运动
  exampleMoveSpline(rpc_cli);

  // 接口调用: 退出登录
  rpc_cli->logout();
  // 接口调用: 断开连接
  rpc_cli->disconnect();

  return 0;
}
