#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include <vector>

#include "aubo_i5_hardware/sdk_test_support.hpp"

/*
  Aubo i5 的单关节伺服运动测试程序，通过 RPC 建立控制连接，通过 RTDE 获取实时状态，然后进入 Servo 模式，
  以125Hz周期发送单个关节的目标位置，最后通过反馈位置验证运动是否成功。
*/
namespace
{

constexpr std::array<const char *, 6> kJointNames{
  "shoulder_joint", "upperArm_joint", "foreArm_joint",
  "wrist1_joint", "wrist2_joint", "wrist3_joint"};
constexpr double kMaximumTestDelta = 0.05;
constexpr double kTestSpeedFraction = 0.05;

const char * argument_value(int argc, char ** argv, const std::string & option)
{
  for (int index = 1; index + 1 < argc; ++index)
  {
    if (option == argv[index])
    {
      return argv[index + 1];
    }
  }
  return nullptr;
}

bool has_flag(int argc, char ** argv, const std::string & flag)
{
  for (int index = 1; index < argc; ++index)
  {
    if (flag == argv[index])
    {
      return true;
    }
  }
  return false;
}

void print_usage()
{
  std::cerr << "Usage: aubo_i5_single_joint_test [--config <robot-test-yaml>] "
            << "--enable-motion --acknowledge-safety\n"
            << "This program sends Servo commands only after both explicit safety flags are present.\n";
}

int joint_index(const std::string & joint_name)
{
  for (std::size_t index = 0; index < kJointNames.size(); ++index)
  {
    if (joint_name == kJointNames[index])
    {
      return static_cast<int>(index);
    }
  }
  return -1;
}

}  // namespace

int main(int argc, char ** argv)
{
  // 安全启动
  if (!has_flag(argc, argv, "--enable-motion") || !has_flag(argc, argv, "--acknowledge-safety"))
  {
    print_usage();
    return 2;
  }
  // 还是读取参数文件
  std::string config_path = aubo_i5_hardware::test_support::default_config_path();
  if (const char * requested_config = argument_value(argc, argv, "--config"))
  {
    config_path = requested_config;
  }
  aubo_i5_hardware::test_support::TestConfig config;
  std::string config_error;
  if (!aubo_i5_hardware::test_support::load_test_config(config_path, config, config_error))
  {
    std::cerr << config_error << '\n';
    return 2;
  }

  const std::string joint_name = config.joint_name;
  const int axis = joint_index(joint_name);// 获取关节对应的id
  if (axis < 0)
  {
    std::cerr << "Unknown joint name: " << joint_name << '\n';
    return 2;
  }

  const double delta = config.delta_rad;
  const double duration_seconds = config.duration_s;
  // 检查条件
  // 角度变化 时间限制 
  if (!std::isfinite(delta) || !std::isfinite(duration_seconds) || delta == 0.0 ||
    std::abs(delta) > kMaximumTestDelta || duration_seconds < 2.0 || duration_seconds > 10.0 ||
    config.speed_fraction <= 0.0 || config.speed_fraction > kTestSpeedFraction)
  {
    std::cerr << "YAML must specify a non-zero movement of at most 0.05 rad, a duration from 2 to 10 seconds, and a speed fraction in (0, 0.05].\n";
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
  if (!aubo_i5_hardware::test_support::feedback_is_fresh(
      feedback, std::chrono::milliseconds(100)) ||
    !aubo_i5_hardware::test_support::motion_is_permitted(feedback))
  {
    std::cerr << "Robot is not in a fresh, motion-permitted Running/Normal-or-Reduced state.\n";
    aubo_i5_hardware::test_support::disconnect(rtde, rpc);
    return 1;
  }

  std::array<double, 6> initial_position{};
  {
    std::lock_guard<std::mutex> lock(feedback.mutex);
    initial_position = feedback.position;
  }
  const double final_position = initial_position[axis] + delta;// 在当前关节角度加上对应的角度变化
  if (final_position < config.lower_limit_rad || final_position > config.upper_limit_rad)
  {
    std::cerr << "Target exceeds the limits specified in the YAML file.\n";
    aubo_i5_hardware::test_support::disconnect(rtde, rpc);
    return 1;
  }

  int exit_code = 0;
  bool servo_enabled = false;
  bool servo_stop_required = false;
  bool speed_fraction_changed = false;
  double previous_speed_fraction = 0.0;
  arcs::common_interface::MotionControlPtr motion;
  try
  {
    const auto robot = rpc->getRobotInterface(robot_name);
    if (!robot)
    {
      throw std::runtime_error("The controller returned an invalid robot interface.");
    }
    motion = robot->getMotionControl();
    if (!motion)
    {
      throw std::runtime_error("The controller returned an invalid motion interface.");
    }
    previous_speed_fraction = motion->getSpeedFraction();
    if (motion->setSpeedFraction(config.speed_fraction) != 0)
    {
      std::cerr << "Could not set the configured speed fraction.\n";
      exit_code = 1;
    }
    else
    {
      speed_fraction_changed = true;
      if (motion->setServoMode(true) != 0)//实时伺服模式
      {
        std::cerr << "Could not enable Servo mode.\n";
        exit_code = 1;
      }
      else
      {
        servo_stop_required = true;
        for (int attempt = 0; attempt < 5 && !motion->isServoModeEnabled(); ++attempt)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        servo_enabled = motion->isServoModeEnabled();
        if (!servo_enabled)
        {
          std::cerr << "Servo mode was not confirmed.\n";
          exit_code = 1;
        }
        else
        {
          const auto steps = static_cast<int>(std::ceil(duration_seconds * 125.0));// 执行步数
          const auto period = std::chrono::microseconds(8000);// 8ms
          std::vector<double> target(initial_position.begin(), initial_position.end());// 目标关节角度数组
          auto next_cycle = std::chrono::steady_clock::now();
          std::cout << "Moving " << joint_name << " by " << delta << " rad over "
                    << duration_seconds << " s at a " << config.speed_fraction * 100.0
                    << "% speed fraction.\n";
          for (int step = 1; step <= steps; ++step)
          {
            if (!aubo_i5_hardware::test_support::feedback_is_fresh(
                feedback, std::chrono::milliseconds(100)) ||
              !aubo_i5_hardware::test_support::motion_is_permitted(feedback))
            {
              std::cerr << "Feedback became stale or the controller left a permitted safety state; aborting.\n";
              exit_code = 1;
              break;
            }
            target[axis] = initial_position[axis] + delta * static_cast<double>(step) / steps;
            if (motion->servoJoint(target, 0.1, 0.1, 0.008, 0.03, 100) < 0)// 发送目标关节角度
            {
              std::cerr << "servoJoint rejected the command at step " << step << ".\n";
              exit_code = 1;
              break;
            }
            next_cycle += period;
            std::this_thread::sleep_until(next_cycle);
          }
        }
      }
    }

    if (servo_stop_required && motion->setServoMode(false) != 0)
    {
      std::cerr << "Warning: the SDK did not confirm Servo mode exit. Use the controller immediately.\n";
      exit_code = 1;
    }
    servo_stop_required = false;
    if (speed_fraction_changed && motion->setSpeedFraction(previous_speed_fraction) != 0)
    {
      std::cerr << "Warning: could not restore the previous speed fraction.\n";
      exit_code = 1;
    }
  }
  catch (const std::exception & exception)
  {
    std::cerr << "AUBO SDK exception during the motion test: " << exception.what() << '\n';
    if (servo_stop_required && motion)
    {
      try
      {
        motion->setServoMode(false);
      }
      catch (...) {}
    }
    if (speed_fraction_changed && motion)
    {
      try
      {
        motion->setSpeedFraction(previous_speed_fraction);
      }
      catch (...) {}
    }
    exit_code = 1;
  }

  if (exit_code == 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    std::lock_guard<std::mutex> lock(feedback.mutex);
    const double observed_delta = feedback.position[axis] - initial_position[axis];
    if (std::abs(observed_delta - delta) > 0.03)
    {
      std::cerr << "Observed joint displacement (" << observed_delta
                << " rad) differs too much from the requested displacement.\n";
      exit_code = 1;
    }
    else
    {
      std::cout << "Single-joint test passed; observed displacement: " << observed_delta << " rad.\n";
    }
  }

  aubo_i5_hardware::test_support::disconnect(rtde, rpc);
  return exit_code;
}
