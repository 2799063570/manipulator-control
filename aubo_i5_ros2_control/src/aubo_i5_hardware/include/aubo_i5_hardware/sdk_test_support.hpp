#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <ament_index_cpp/get_package_share_directory.hpp>
#include <aubo/robot/robot_state.h>
#include <aubo_sdk/rpc.h>
#include <aubo_sdk/rtde.h>
#include <yaml-cpp/yaml.h>

namespace aubo_i5_hardware::test_support
{

constexpr std::size_t kArmDof = 6;

struct TestConfig
{
  std::string robot_ip;
  int rpc_port{30004};
  int rtde_port{30010};
  int feedback_timeout_ms{2000};// 等待时间
  std::string joint_name;
  double delta_rad{0.0};
  double duration_s{0.0};
  double speed_fraction{0.0};
  double lower_limit_rad{0.0};
  double upper_limit_rad{0.0};
};

inline std::string default_config_path()
{
  return ament_index_cpp::get_package_share_directory("aubo_i5_hardware") +
    "/config/robot_test.example.yaml";
}

inline bool load_test_config(
  const std::string & config_path, TestConfig & config, std::string & error)
{
  try
  {
    const YAML::Node root = YAML::LoadFile(config_path);
    const YAML::Node connection = root["connection"];
    const YAML::Node motion = root["single_joint_test"];
    if (!connection || !motion)
    {
      error = "The YAML file must contain connection and single_joint_test sections.";
      return false;
    }
    config.robot_ip = connection["robot_ip"].as<std::string>();
    config.rpc_port = connection["rpc_port"].as<int>();
    config.rtde_port = connection["rtde_port"].as<int>();
    config.feedback_timeout_ms = connection["feedback_timeout_ms"].as<int>();
    config.joint_name = motion["joint_name"].as<std::string>();
    config.delta_rad = motion["delta_rad"].as<double>();
    config.duration_s = motion["duration_s"].as<double>();
    config.speed_fraction = motion["speed_fraction"].as<double>();
    config.lower_limit_rad = motion["lower_limit_rad"].as<double>();
    config.upper_limit_rad = motion["upper_limit_rad"].as<double>();
  }
  catch (const YAML::Exception & exception)
  {
    error = "Could not read " + config_path + ": " + exception.what();
    return false;
  }

  if (config.robot_ip.empty() || config.rpc_port <= 0 || config.rtde_port <= 0 ||
    config.feedback_timeout_ms <= 0 || config.joint_name.empty() ||
    !std::isfinite(config.delta_rad) || !std::isfinite(config.duration_s) ||
    !std::isfinite(config.speed_fraction) || !std::isfinite(config.lower_limit_rad) ||
    !std::isfinite(config.upper_limit_rad) || config.lower_limit_rad >= config.upper_limit_rad)
  {
    error = "The YAML configuration has an empty robot_ip or invalid numeric values.";
    return false;
  }
  return true;
}

struct Feedback
{
  std::mutex mutex;
  std::condition_variable received;
  std::array<double, kArmDof> position{};
  std::array<double, kArmDof> velocity{};
  arcs::common_interface::RobotModeType robot_mode{
    arcs::common_interface::RobotModeType::NoController};
  arcs::common_interface::SafetyModeType safety_mode{
    arcs::common_interface::SafetyModeType::Undefined};
  std::chrono::steady_clock::time_point timestamp{};
  bool valid{false};
};

inline void disconnect(
  std::shared_ptr<arcs::aubo_sdk::RtdeClient> & rtde,
  std::shared_ptr<arcs::aubo_sdk::RpcClient> & rpc) noexcept
{
  if (rtde)
  {
    try
    {
      rtde->logout();
      rtde->disconnect();
    }
    catch (...) {}
    rtde.reset();
  }
  if (rpc)
  {
    try
    {
      rpc->logout();
      rpc->disconnect();
    }
    catch (...) {}
    rpc.reset();
  }
}

inline bool connect_and_read_feedback(
  const TestConfig & config,
  std::shared_ptr<arcs::aubo_sdk::RpcClient> & rpc,
  std::shared_ptr<arcs::aubo_sdk::RtdeClient> & rtde,
  Feedback & feedback,
  std::string & robot_name)
{
  const char * username = std::getenv("AUBO_ROBOT_USERNAME");// 寻找环境变量 用户名和密码
  const char * password = std::getenv("AUBO_ROBOT_PASSWORD");
  if (username == nullptr || password == nullptr)
  {
    std::cerr << "Set AUBO_ROBOT_USERNAME and AUBO_ROBOT_PASSWORD first.\n";
    return false;
  }

  try
  {
    rpc = std::make_shared<arcs::aubo_sdk::RpcClient>();
    if (rpc->setRequestTimeout(config.feedback_timeout_ms) != 0 ||
      rpc->connect(config.robot_ip, config.rpc_port) != 0 ||
      rpc->login(username, password) != 0)
    {
      std::cerr << "RPC connection or login failed.\n";
      disconnect(rtde, rpc);
      return false;
    }
    const auto robot_names = rpc->getRobotNames();
    if (robot_names.empty())
    {
      std::cerr << "The controller returned no robot names.\n";
      disconnect(rtde, rpc);
      return false;
    }
    robot_name = robot_names.front();

    rtde = std::make_shared<arcs::aubo_sdk::RtdeClient>();
    if (rtde->connect(config.robot_ip, config.rtde_port) < 0 ||
      rtde->login(username, password) != 0)
    {
      std::cerr << "RTDE connection or login failed.\n";
      disconnect(rtde, rpc);
      return false;
    }
    const int topic = rtde->setTopic(
      false, {"R1_actual_q", "R1_actual_qd", "R1_robot_mode", "R1_safety_mode"}, 125, 0);
    if (topic < 0)
    {
      std::cerr << "Could not create the RTDE feedback topic.\n";
      disconnect(rtde, rpc);
      return false;
    }
    if (rtde->subscribe(topic, [&feedback](arcs::aubo_sdk::InputParser & parser) {
      const auto q = parser.popVectorDouble();// 解析位置、速度、模式
      const auto qd = parser.popVectorDouble();
      const auto robot_mode = parser.popRobotModeType();
      const auto safety_mode = parser.popSafetyModeType();
      if (q.size() != kArmDof || qd.size() != kArmDof ||
        !std::all_of(q.begin(), q.end(), [](double value) {return std::isfinite(value);}) ||
        !std::all_of(qd.begin(), qd.end(), [](double value) {return std::isfinite(value);})) // 判断一个范围内的所有元素是否都满足某个条件。
      {
        return;
      }
      {
        std::lock_guard<std::mutex> lock(feedback.mutex);
        std::copy_n(q.begin(), kArmDof, feedback.position.begin());
        std::copy_n(qd.begin(), kArmDof, feedback.velocity.begin());
        feedback.robot_mode = robot_mode;
        feedback.safety_mode = safety_mode;
        feedback.timestamp = std::chrono::steady_clock::now();
        feedback.valid = true;
      }
      feedback.received.notify_all();// 通知所有的线程
    }) != 0)
    {
      std::cerr << "Could not subscribe to RTDE feedback.\n";
      disconnect(rtde, rpc);
      return false;
    }

    std::unique_lock<std::mutex> lock(feedback.mutex);// 上锁
    if (!feedback.received.wait_for(lock, std::chrono::milliseconds(config.feedback_timeout_ms), [&feedback] {
      return feedback.valid;
    }))// 设置等待条件 防止虚假唤醒。
    {
      std::cerr << "Timed out waiting for a valid six-axis RTDE feedback frame.\n";
      lock.unlock();
      disconnect(rtde, rpc);
      return false;
    }
    return true;
  }
  catch (const std::exception & exception)
  {
    std::cerr << "AUBO SDK exception: " << exception.what() << '\n';
    disconnect(rtde, rpc);
    return false;
  }
}

inline bool feedback_is_fresh(Feedback & feedback, std::chrono::milliseconds timeout)
{
  std::lock_guard<std::mutex> lock(feedback.mutex);
  return feedback.valid && std::chrono::steady_clock::now() - feedback.timestamp < timeout;
}

inline bool motion_is_permitted(Feedback & feedback)
{
  std::lock_guard<std::mutex> lock(feedback.mutex);
  return feedback.robot_mode == arcs::common_interface::RobotModeType::Running &&
    (feedback.safety_mode == arcs::common_interface::SafetyModeType::Normal ||
    feedback.safety_mode == arcs::common_interface::SafetyModeType::ReducedMode);
}

}  // namespace aubo_i5_hardware::test_support
