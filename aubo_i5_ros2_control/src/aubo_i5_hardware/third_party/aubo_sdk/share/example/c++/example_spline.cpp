#include <cstring>
#include <fstream>
#include <math.h>
#include <thread>

#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"
#include <aubo/error_stack/error_stack.h>
#ifdef WIN32
#include <windows.h>
#endif
using namespace arcs::aubo_sdk;
using namespace arcs::common_interface;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrival(RobotInterfacePtr impl) {
  // 接口调用: 获取当前的运动指令 ID
  int exec_id = impl->getMotionControl()->getExecId();

  int cnt = 0;
  // 在等待机械臂开始运动时，获取exec_id最大的重试次数
  int max_retry_count = 5;

  // 等待机械臂开始运动
  while (exec_id == -1) {
    if (cnt++ > max_retry_count) {
      return -1;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    exec_id = impl->getMotionControl()->getExecId();
  }

  // 等待机械臂动作完成
  while (exec_id != -1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    exec_id = impl->getMotionControl()->getExecId();
  }

  return 0;
}

// 等待样条运动完成
void waitMoveSplineFinished(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();

  auto robot_interface = impl->getRobotInterface(robot_name);

  std::cout << "等待样条运动开始" << std::endl;
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
  std::cout << "样条运动结束" << std::endl;
}

class TrajectoryIo {
public:
  // 构造函数，接受要打开的文件名作为参数
  TrajectoryIo(const char *filename) {
    input_file_.open(filename, std::ios::in);
  }

  // 检查文件是否成功打开
  bool open() {
    if (!input_file_.is_open()) {
      std::cerr << "无法打开轨迹文件. 请检查输入的文件路径是否正确."
                << std::endl;
      return false;
    }
    return true;
  }
  ~TrajectoryIo() { input_file_.close(); }

  // 解析文件中的轨迹数据，
  // 并将其转换为一个二维的 std::vector。
  // 它逐行读取文件内容，将每行数据解析为一组 double 数值，
  // 并将这些数值存储在一个嵌套的二维向量中。
  std::vector<std::vector<double>> parse() {
    std::vector<std::vector<double>> res;
    std::string tmp;
    int linenum = 1;
    while (std::getline(input_file_, tmp, '\n')) {
      try {
        auto q = split(tmp, ",");
        res.push_back(q);
      } catch (const char *p) {
        std::cerr << "Line: " << linenum << " \"" << p << "\""
                  << " is not a number of double" << std::endl;
        break;
      }
      linenum++;
    }
    return res;
  }

  // 切割字符串并转换为 double 类型
  std::vector<double> split(const std::string &str, const char *delim) {
    std::vector<double> res;
    if ("" == str) {
      return res;
    }
    // 先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1]; // 不要忘了
    std::strcpy(strs, str.c_str());

    char *p = std::strtok(strs, delim);
    char *endp = nullptr;
    while (p) {
      double v = std::strtod(p, &endp);
      if (endp[0] != 0 && endp[0] != '\r') {
        delete[] strs;
        strs = nullptr;
        throw p;
      }
      res.push_back(v); // 存入结果数组
      p = std::strtok(nullptr, delim);
    }

    if (strs) {
      delete[] strs;
      strs = nullptr;
    }

    return res;
  }

private:
  std::ifstream input_file_; // 输入文件流
};

// 样条运动
void exampleSpline(RpcClientPtr cli) {
  // 读取轨迹文件
  auto filename = "../trajs/coffee_spline.txt";
  TrajectoryIo input(filename);

  // 尝试打开轨迹文件，如果无法打开，直接返回
  if (!input.open()) {
    return;
  }

  // 解析轨迹数据
  auto traj = input.parse();

  // 检查轨迹文件中是否有路点，
  // 如果数量为 0，输出错误消息并返回
  auto traj_sz = traj.size();
  if (traj_sz == 0) {
    std::cerr << "轨迹文件中的路点数量为0." << std::endl;
    return;
  }

  // 接口调用: 获取机器人的名字
  auto robot_name = cli->getRobotNames().front();

  auto robot_interface = cli->getRobotInterface(robot_name);

  // 接口调用: 设置机械臂的速度比率
  robot_interface->getMotionControl()->setSpeedFraction(0.3);

  // 接口调用: 关节运动到轨迹文件中的第一个路点
  std::vector<double> joint_angle = traj[0];
  cli->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveJoint(joint_angle, 80 * (M_PI / 180), 60 * (M_PI / 180), 0., 0.);

  // 阻塞
  int ret = waitArrival(robot_interface);
  if (ret == 0) {
    std::cout << "关节运动到轨迹文件中的第一个路点成功" << std::endl;
  } else {
    std::cout << "关节运动到轨迹文件中的第一个路点失败" << std::endl;
  }

  std::cout << "添加轨迹文件中的路点" << std::endl;
  // 接口调用: 添加轨迹文件中的路点
  for (int i = 1; i < (int)traj.size(); i++) {
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveSpline(traj[i], 1, 1, 0);
  }

  // 接口调用: 当关节角参数传入为空时，结束路点添加。
  // 等待算法计算完成后，机械臂开始执行样条运动。
  // 所以，当添加的路点数量越多时，由于算法计算的耗时时间长，
  // 机械臂可能不会立刻运动。
  cli->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveSpline({}, 1, 1, 0.005);
}

void printlog(int level, const char *source, int code, std::string content) {
  static const char *level_names[] = {"Critical", "Error", "Warning",
                                      "Info",     "Debug", "BackTrace"};
  fprintf(stderr, "[%s] %s - %d %s\n", level_names[level], source, code,
          content.c_str());
}

/**
 * 功能: 关节空间的样条运动实现.txt文件中的离散轨迹
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
 * 第二步: 连接 RTDE 服务、登录
 * 第三步: RTDE 设置话题、订阅话题来打印error_stack中的日志信息
 * 第四步: 读取解析.txt轨迹文件
 * 第五步: 添加文件中的路点，做样条运动
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

  // 样条运动
  exampleSpline(rpc_cli);

  // 等待样条运动完成
  waitMoveSplineFinished(rpc_cli);

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
