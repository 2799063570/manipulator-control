#include "aubo_sdk/rpc.h"
#include "math.h"
#ifdef WIN32
#include <Windows.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

// 模板函数：打印单个变量的值和名称
template <typename T> void printSingle(T param, std::string name) {
  std::cout << name << ": " << param << std::endl;
}

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

void examplePoseAdd(RpcClientPtr impl) {
  std::vector<double> p1 = {0.2, 0.5, 0.1, 1.57, 0, 0};
  std::vector<double> p2 = {0.2, 0.5, 0.6, 1.57, 0, 0};

  // 接口调用: 位姿加法
  auto result = impl->getMath()->poseAdd(p1, p2);

  printVec<double>(result, "poseAdd");
}

void examplePoseSub(RpcClientPtr impl) {
  std::vector<double> p1 = {0.2, 0.5, 0.1, 1.57, 0, 0};
  std::vector<double> p2 = {0.2, 0.5, 0.6, 1.57, 0, 0};

  // 接口调用: 位姿减法
  auto result = impl->getMath()->poseSub(p1, p2);

  printVec<double>(result, "poseSub");
}

void exampleInterpolatePose(RpcClientPtr impl) {
  std::vector<double> p1 = {0.2, 0.2, 0.4, 0, 0, 0};
  std::vector<double> p2 = {0.2, 0.2, 0.6, 0, 0, 0};
  double alpha = 0.5;

  // 接口调用: 计算线性差值
  auto result = impl->getMath()->interpolatePose(p1, p2, alpha);

  printVec<double>(result, "interpolatePose");
}

void examplePoseTrans(RpcClientPtr impl) {
  // B相对于A的位姿
  std::vector<double> F_B_A = {0.2, 0.5, 0.1, 1.57, 0, 0};
  // C相对于B的位姿
  std::vector<double> F_C_B = {0.2, 0.5, 0.6, 1.57, 0, 0};

  // 接口调用: 位姿变换获得C相对于A的位姿
  auto F_C_A = impl->getMath()->poseTrans(F_B_A, F_C_B);

  printVec<double>(F_C_A, "poseTrans");
}

void examplePoseTransInv(RpcClientPtr impl) {
  // C相对于A的位姿
  std::vector<double> F_C_A = {0.4, -0.0996016, 0.600478, 3.14, 0, 0};
  // C相对于B的位姿
  std::vector<double> F_C_B = {0.2, 0.5, 0.6, 1.57, 0, 0};
  // 接口调用: 位姿逆变换获得B相对于A的位姿
  auto F_B_A = impl->getMath()->poseTransInv(F_C_A, F_C_B);

  printVec<double>(F_B_A, "poseTransInv");
}

void examplePoseInverse(RpcClientPtr impl) {
  std::vector<double> p = {0.2, 0.5, 0.1, 1.57, 0, 3.14};
  // 接口调用: 获取位姿的逆
  auto result = impl->getMath()->poseInverse(p);

  printVec<double>(result, "poseInverse");
}

void examplePoseDistance(RpcClientPtr impl) {
  std::vector<double> p1 = {0.1, 0.3, 0.1, 0.3142, 0.0, 1.571};
  std::vector<double> p2 = {0.2, 0.5, 0.6, 0, -0.172, 0.0};

  // 接口调用: 获得两个位姿的位置距离
  auto result = impl->getMath()->poseDistance(p1, p2);

  printSingle(result, "poseDistance");
}

void examplePoseEqual(RpcClientPtr impl) {
  std::vector<double> p1 = {0.1, 0.3, 0.1, 0.3142, 0.0, 1.571};
  std::vector<double> p2 = {0.1, 0.3, 0.1, 0.3142, 0.0, 1.5711};
  // 接口调用: 判断两个位姿是否相等
  auto result = impl->getMath()->poseEqual(p1, p2);
  printSingle(result, "poseEqual");
}

// 欧拉角转四元数
void exampleRpyToQuat(RpcClientPtr impl) {
  std::vector<double> rpy = {0.611, 0.785, 0.960};
  // 接口调用: 欧拉角转四元数
  auto quat = impl->getMath()->rpyToQuaternion(rpy);

  printVec(quat, "欧拉角 -> 四元数");
}

// 四元数转欧拉角
void exampleQuatToRpy(RpcClientPtr impl) {
  std::vector<double> quat = {0.834722, 0.0780426, 0.451893, 0.304864};
  // 接口调用: 四元数转欧拉角
  auto rpy = impl->getMath()->quaternionToRpy(quat);

  printVec(rpy, "四元数 -> 欧拉角");
}

// 坐标系标定
void exampleCalibrateCoordinate(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();

  std::vector<double> tcp_offset = {0.17734, 0.00233, 0.14682, 0.0, 0.0, 0.0};

  // 接口调用: 设置 TCP 偏移值
  impl->getRobotInterface(robot_name)
      ->getRobotConfig()
      ->setTcpOffset(tcp_offset);

  // TCP 在基坐标系下的位姿
  std::vector<double> coord_p0(6), coord_p1(6), coord_p2(6);
  coord_p0[0] = 0.55462;
  coord_p0[1] = 0.06219;
  coord_p0[2] = 0.37175;
  coord_p0[3] = -3.142;
  coord_p0[4] = 0.0;
  coord_p0[5] = 1.580;

  coord_p1[0] = 0.63746;
  coord_p1[1] = 0.11805;
  coord_p1[2] = 0.37175;
  coord_p1[3] = -3.142;
  coord_p1[4] = 0.0;
  coord_p1[5] = 1.580;

  coord_p2[0] = 0.40441;
  coord_p2[1] = 0.28489;
  coord_p2[2] = 0.37174;
  coord_p2[3] = -3.142;
  coord_p2[4] = 0.0;
  coord_p2[5] = 1.580;

  // 接口调用: 用户坐标系标定
  // 接口调用: 获取用户坐标系相对于基坐标系的位姿
  auto [user_on_base, ret] =
      impl->getMath()->calibrateCoordinate({coord_p0, coord_p1, coord_p2}, 0);

  // 接口调用: 获取 TCP 在基坐标系下的当前位姿
  auto tcp_on_base =
      impl->getRobotInterface(robot_name)->getRobotState()->getTcpPose();

  // 接口调用: 获取用户坐标系相对于基坐标系的位姿的逆
  auto tcp_on_base_inv = impl->getMath()->poseInverse(user_on_base);

  // 接口调用: 获取 TCP 在用户坐标系下的当前位姿
  auto tcp_on_user = impl->getMath()->poseTrans(tcp_on_base_inv, tcp_on_base);

  printVec(tcp_on_base, "TCP在基坐标系下的位姿");
  printVec(tcp_offset, "TCP偏移");
  printVec(user_on_base, "用户坐标系相对于基坐标系的位姿");
  printVec(tcp_on_user, "TCP在用户坐标系下的位姿");
}

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

// 计算另一半圆弧的中间点
void exampleCalculateCircleFourthPoint(RpcClientPtr impl) {
  // 接口调用: 获取机器人的名字
  auto robot_name = impl->getRobotNames().front();

  std::vector<double> tcp_offset = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  // 接口调用: 设置 TCP 偏移值
  impl->getRobotInterface(robot_name)
      ->getRobotConfig()
      ->setTcpOffset(tcp_offset);

  // 接口调用: 设置机械臂运动的速度比率
  impl->getRobotInterface(robot_name)
      ->getMotionControl()
      ->setSpeedFraction(0.3);

  std::vector<double> q = {0.00,
                           -10.43 / 180 * M_PI,
                           87.39 / 180 * M_PI,
                           7.82 / 180 * M_PI,
                           90.0 / 180 * M_PI,
                           0.0 / 180 * M_PI};

  // 接口调用: 关节运动到起始位置
  impl->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveJoint(q, 1.2, 1.0, 0, 0);
  // 阻塞
  int ret = waitArrival(impl->getRobotInterface(robot_name));
  if (ret == 0) {
    std::cout << "关节运动到初始位置成功" << std::endl;
  } else {
    std::cout << "关节运动到初始位置失败" << std::endl;
  }

  std::vector<double> p1 = {0.5488696249770836,     -0.1214996547187204,
                            0.2631931199112321,     -3.14159198038469,
                            -3.673205103150083e-06, 1.570796326792424};

  std::vector<double> p2 = {0.5488696249770835,   -0.1214996547187207,
                            0.3599720701808493,   -3.14159198038469,
                            -3.6732051029273e-06, 1.570796326792423};

  std::vector<double> p3 = {0.5488696249770836,     -0.0389996547187214,
                            0.3599720701808496,     -3.141591980384691,
                            -3.673205102557476e-06, 1.570796326792422};

  // 接口调用: 直线运动到圆的第一个点
  impl->getRobotInterface(robot_name)
      ->getMotionControl()
      ->moveLine(p1, 1.2, 0.25, 0, 0);
  // 阻塞
  ret = waitArrival(impl->getRobotInterface(robot_name));
  if (ret == 0) {
    std::cout << "直线运动到圆的起始点成功" << std::endl;
  } else {
    std::cout << "直线运动到圆的起始点失败" << std::endl;
  }

  // 接口调用: 计算另一半圆弧的中间点位置
  auto [p4, retval] =
      impl->getMath()->calculateCircleFourthPoint(p1, p2, p3, 1);

  if (retval == 0) {
    std::cerr << "计算另一半圆弧的中间点失败，无法完成圆运动" << std::endl;
  } else {
    // 接口调用: 设置为固定模式
    impl->getRobotInterface(robot_name)
        ->getMotionControl()
        ->setCirclePathMode(0);

    // 接口调用: 做圆弧运动
    impl->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveCircle(p2, p3, 1.2, 0.25, 0, 0);
    // 阻塞
    ret = waitArrival(impl->getRobotInterface(robot_name));
    if (ret == -1) {
      std::cout << "圆运动失败" << std::endl;
    }

    // 接口调用: 做组成圆运动的另一半圆弧运动
    impl->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveCircle(p4, p1, 1.2, 0.25, 0, 0);
    // 阻塞
    ret = waitArrival(impl->getRobotInterface(robot_name));
    if (ret == 0) {
      std::cout << "圆运动成功" << std::endl;
    } else {
      std::cout << "圆运动失败" << std::endl;
    }
  }
}

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv) {
#ifdef WIN32
  // 将Windows控制台输出代码页设置为 UTF-8
  SetConsoleOutputCP(CP_UTF8);
#endif
  auto rpc_cli = std::make_shared<RpcClient>();
  // 接口调用: 设置 RPC 超时, 单位: ms
  rpc_cli->setRequestTimeout(1000);
  // 接口调用: 连接到 RPC 服务
  rpc_cli->connect(LOCAL_IP, 30004);
  // 接口调用: 登录
  rpc_cli->login("aubo", "123456");

  // 位姿相加
  examplePoseAdd(rpc_cli);

  // 位姿相减
  examplePoseSub(rpc_cli);

  // 位姿线性插值
  exampleInterpolatePose(rpc_cli);

  // 位姿变换
  examplePoseTrans(rpc_cli);

  // 位姿逆变换
  examplePoseTransInv(rpc_cli);

  // 姿态的逆
  examplePoseInverse(rpc_cli);

  // 位姿距离（不包括姿态）
  examplePoseDistance(rpc_cli);

  // 判断位姿是否相等
  examplePoseEqual(rpc_cli);

  // 欧拉角转四元数
  exampleRpyToQuat(rpc_cli);

  // 四元数转欧拉角
  exampleQuatToRpy(rpc_cli);

  // 坐标系标定
  exampleCalibrateCoordinate(rpc_cli);

  // 计算另一半圆弧的中间点
  //  exampleCalculateCircleFourthPoint(rpc_cli);

  // 接口调用: 退出登录
  rpc_cli->logout();
  // 接口调用: 断开连接
  rpc_cli->disconnect();

  return 0;
}
