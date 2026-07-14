#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

using namespace arcs::aubo_sdk;

// 模板:打印类型为std::vector<T>的变量
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

// 获取机械臂状态
void exampleState(RpcClientPtr cli) {
  // 接口调用:获取机器人的名字
  auto robot_name = cli->getRobotNames().front();

  // 接口调用:获取机器人的模式状态
  RobotModeType robot_mode_type;
  robot_mode_type =
      cli->getRobotInterface(robot_name)->getRobotState()->getRobotModeType();
  std::cout << "机器人的模式状态:" << robot_mode_type << std::endl;

  // 接口调用:获取安全模式
  SafetyModeType safety_mode_type;
  safety_mode_type =
      cli->getRobotInterface(robot_name)->getRobotState()->getSafetyModeType();
  std::cout << "安全模式:" << safety_mode_type << std::endl;

  // 接口调用:机器人是否处于静止状态
  bool is_steady;
  is_steady = cli->getRobotInterface(robot_name)->getRobotState()->isSteady();
  std::cout << "机器人是否处于静止状态:" << (is_steady ? "是" : "否")
            << std::endl;

  // 接口调用:机器人是否在安全限制之内
  bool is_within_safety_limits;
  is_within_safety_limits = cli->getRobotInterface(robot_name)
                                ->getRobotState()
                                ->isWithinSafetyLimits();
  std::cout << "机器人是否在安全限制之内:"
            << (is_within_safety_limits ? "是" : "否") << std::endl;

  // 接口调用:获取TCP在基坐标系下的实际位姿
  std::vector<double> actual_tcp_pose;
  actual_tcp_pose =
      cli->getRobotInterface(robot_name)->getRobotState()->getTcpPose();
  printVec<double>(actual_tcp_pose, "TCP在基坐标系下的实际位姿");

  // 接口调用:获取法兰盘中心在基坐标系下的实际位姿
  std::vector<double> actual_flange_pose;
  actual_flange_pose =
      cli->getRobotInterface(robot_name)->getRobotState()->getToolPose();
  printVec<double>(actual_flange_pose, "法兰盘中心在基坐标系下的实际位姿");

  // 接口调用:获取机械臂关节状态
  std::vector<JointStateType> joint_state;
  joint_state =
      cli->getRobotInterface(robot_name)->getRobotState()->getJointState();
  printVec<JointStateType>(joint_state, "机械臂关节状态");

  // 接口调用:获取关节的伺服状态
  std::vector<JointServoModeType> joint_servo_mode;
  joint_servo_mode =
      cli->getRobotInterface(robot_name)->getRobotState()->getJointServoMode();
  printVec<JointServoModeType>(joint_servo_mode, "关节的伺服状态");

  // 接口调用:获取机械臂的实际位置关节角度
  std::vector<double> joint_positions;
  joint_positions =
      cli->getRobotInterface(robot_name)->getRobotState()->getJointPositions();
  printVec<double>(joint_positions, "机械臂的实际位置关节角度");

  // 接口调用:获取机械臂的关节电流
  std::vector<double> joint_currents;
  joint_currents =
      cli->getRobotInterface(robot_name)->getRobotState()->getJointCurrents();
  printVec<double>(joint_currents, "机械臂的关节电流");

  // 接口调用:获取机械臂的关节电压
  std::vector<double> joint_voltages;
  joint_voltages =
      cli->getRobotInterface(robot_name)->getRobotState()->getJointVoltages();
  printVec<double>(joint_voltages, "机械臂的关节电压");

  // 接口调用:获取机械臂的关节温度
  std::vector<double> joint_temperatures;
  joint_temperatures = cli->getRobotInterface(robot_name)
                           ->getRobotState()
                           ->getJointTemperatures();
  printVec<double>(joint_temperatures, "机械臂的关节温度");

  // 接口调用:获取关节的UUID
  std::vector<std::string> joint_unique_ids;
  joint_unique_ids =
      cli->getRobotInterface(robot_name)->getRobotState()->getJointUniqueIds();
  printVec<std::string>(joint_unique_ids, "关节的UUID");

  // 接口调用:获取关节的固件版本
  std::vector<int> joint_firmware_versions;
  joint_firmware_versions = cli->getRobotInterface(robot_name)
                                ->getRobotState()
                                ->getJointFirmwareVersions();
  printVec<int>(joint_firmware_versions, "关节的固件版本");

  // 接口调用:获取关节的硬件版本
  std::vector<int> joint_hardward_versions;
  joint_hardward_versions = cli->getRobotInterface(robot_name)
                                ->getRobotState()
                                ->getJointHardwareVersions();
  printVec<int>(joint_hardward_versions, "关节的硬件版本");

  // 接口调用:获取主板固件的UUID
  std::string master_board_unique_id;
  master_board_unique_id = cli->getRobotInterface(robot_name)
                               ->getRobotState()
                               ->getMasterBoardUniqueId();
  std::cout << "主板固件的UUID：" << master_board_unique_id << std::endl;

  // 接口调用:获取主板固件的固件版本
  int master_board_firmware_version;
  master_board_firmware_version = cli->getRobotInterface(robot_name)
                                      ->getRobotState()
                                      ->getMasterBoardFirmwareVersion();
  std::cout << "主板固件的固件版本：" << master_board_firmware_version
            << std::endl;

  // 接口调用:获取主板固件的硬件版本
  int master_board_hardware_version;
  master_board_hardware_version = cli->getRobotInterface(robot_name)
                                      ->getRobotState()
                                      ->getMasterBoardHardwareVersion();
  std::cout << "主板固件的硬件版本：" << master_board_hardware_version
            << std::endl;

  // 接口调用:获取从板固件的UUID
  std::string slave_board_unique_id;
  slave_board_unique_id = cli->getRobotInterface(robot_name)
                              ->getRobotState()
                              ->getSlaveBoardUniqueId();
  std::cout << "从板固件的UUID：" << slave_board_unique_id << std::endl;

  // 接口调用:获取从板固件的固件版本
  int slave_board_firmware_version;
  slave_board_firmware_version = cli->getRobotInterface(robot_name)
                                     ->getRobotState()
                                     ->getSlaveBoardFirmwareVersion();
  std::cout << "从板固件的固件版本：" << slave_board_firmware_version
            << std::endl;

  // 接口调用:获取从板固件的硬件版本
  int slave_board_hardware_version;
  slave_board_hardware_version = cli->getRobotInterface(robot_name)
                                     ->getRobotState()
                                     ->getSlaveBoardHardwareVersion();
  std::cout << "从板固件的硬件版本：" << slave_board_hardware_version
            << std::endl;

  // 接口调用:获取工具的UUID
  std::string tool_unique_id;
  tool_unique_id =
      cli->getRobotInterface(robot_name)->getRobotState()->getToolUniqueId();
  std::cout << "工具的UUID：" << tool_unique_id << std::endl;

  // 接口调用:获取工具的固件版本
  int tool_firmware_version;
  tool_firmware_version = cli->getRobotInterface(robot_name)
                              ->getRobotState()
                              ->getToolFirmwareVersion();
  std::cout << "工具的固件版本：" << tool_firmware_version << std::endl;

  // 接口调用:获取工具的硬件版本
  int tool_hardware_version;
  tool_hardware_version = cli->getRobotInterface(robot_name)
                              ->getRobotState()
                              ->getToolHardwareVersion();
  std::cout << "工具的固件版本：" << tool_hardware_version << std::endl;

  // 接口调用:获取基座的UUID
  std::string pedestal_unique_id;
  pedestal_unique_id = cli->getRobotInterface(robot_name)
                           ->getRobotState()
                           ->getPedestalUniqueId();
  std::cout << "基座的UUID：" << pedestal_unique_id << std::endl;

  // 接口调用:获取基座的固件版本
  int pedestal_firmware_version;
  pedestal_firmware_version = cli->getRobotInterface(robot_name)
                                  ->getRobotState()
                                  ->getPedestalFirmwareVersion();
  std::cout << "基座的固件版本：" << pedestal_firmware_version << std::endl;

  // 接口调用:获取基座的硬件版本
  int pedestal_hardware_version;
  pedestal_hardware_version = cli->getRobotInterface(robot_name)
                                  ->getRobotState()
                                  ->getPedestalHardwareVersion();
  std::cout << "基座的硬件版本：" << pedestal_hardware_version << std::endl;

  // 接口调用:获取机械臂关节目标位置角度
  std::vector<double> joint_target_positions;
  joint_target_positions = cli->getRobotInterface(robot_name)
                               ->getRobotState()
                               ->getJointTargetPositions();
  printVec<double>(joint_target_positions, "关节目标位置角度");

  // 接口调用:获取控制柜温度
  double control_box_temperature;
  control_box_temperature = cli->getRobotInterface(robot_name)
                                ->getRobotState()
                                ->getControlBoxTemperature();
  std::cout << "控制柜温度：" << control_box_temperature << std::endl;

  // 接口调用:获取母线电压
  double main_voltage;
  main_voltage =
      cli->getRobotInterface(robot_name)->getRobotState()->getMainVoltage();
  std::cout << "母线电压：" << main_voltage << std::endl;

  // 接口调用:获取母线电流
  double main_current;
  main_current =
      cli->getRobotInterface(robot_name)->getRobotState()->getMainCurrent();
  std::cout << "母线电流：" << main_current << std::endl;

  // 接口调用:获取机器人电压
  double robot_voltage;
  robot_voltage =
      cli->getRobotInterface(robot_name)->getRobotState()->getRobotVoltage();
  std::cout << "机器人电压：" << robot_voltage << std::endl;

  // 接口调用:获取机器人电流
  double robot_current;
  robot_current =
      cli->getRobotInterface(robot_name)->getRobotState()->getRobotCurrent();
  std::cout << "机器人电流：" << robot_current << std::endl;
}

#define ip_local "127.0.0.1"

/**
 * 功能:获取机械臂状态
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、登录
 * 第二步: 获取机械臂状态
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

  // 获取机械臂状态
  exampleState(rpc_cli);

  // 接口调用: 退出登录
  rpc_cli->logout();
  // 接口调用: 断开连接
  rpc_cli->disconnect();

  return 0;
}
