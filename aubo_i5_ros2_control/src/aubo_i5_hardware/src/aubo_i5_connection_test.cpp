#include <iostream>
#include <string>

#include "aubo_i5_hardware/sdk_test_support.hpp"

/*
  读取关节状态 机器人状态  
*/

namespace
{

void print_usage()
{
  std::cerr << "Usage: aubo_i5_connection_test [--config <robot-test-yaml>]\n";
}

}  // namespace

int main(int argc, char ** argv)
{
  std::string config_path = aubo_i5_hardware::test_support::default_config_path();
  if (argc == 3 && std::string(argv[1]) == "--config")
  {
    config_path = argv[2];// 输入yaml配置文件路径
  }
  else if (argc != 1)
  {
    print_usage();
    return 2;
  }

  aubo_i5_hardware::test_support::TestConfig config;
  std::string config_error;
  if (!aubo_i5_hardware::test_support::load_test_config(config_path, config, config_error))// 读取yaml文件中的数据倒导入到config中
  {
    std::cerr << config_error << '\n';
    return 2;
  }

  std::shared_ptr<arcs::aubo_sdk::RpcClient> rpc;
  std::shared_ptr<arcs::aubo_sdk::RtdeClient> rtde;
  aubo_i5_hardware::test_support::Feedback feedback;
  std::string robot_name;
  if (!aubo_i5_hardware::test_support::connect_and_read_feedback(
      config, rpc, rtde, feedback, robot_name))
  {
    return 1;
  }

  {
    std::lock_guard<std::mutex> lock(feedback.mutex);
    std::cout << "Connected to robot: " << robot_name << '\n';
    std::cout << "Joint position (rad):";
    for (const double value : feedback.position)
    {
      std::cout << ' ' << value;
    }
    std::cout << "\nJoint velocity (rad/s):";
    for (const double value : feedback.velocity)
    {
      std::cout << ' ' << value;
    }
    std::cout << "\nRobot mode: " << static_cast<int>(feedback.robot_mode)
              << ", safety mode: " << static_cast<int>(feedback.safety_mode) << '\n';
  }

  aubo_i5_hardware::test_support::disconnect(rtde, rpc);
  std::cout << "Read-only connection test passed; no motion command was sent.\n";
  return 0;
}
