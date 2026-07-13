#include "aubo_i5_hardware/aubo_i5_system.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <thread>

#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "pluginlib/class_list_macros.hpp"
#include "rclcpp/rclcpp.hpp"

namespace aubo_i5_hardware
{
namespace
{
constexpr std::array<const char *, 6> kJointNames{
  "shoulder_joint", "upperArm_joint", "foreArm_joint",
  "wrist1_joint", "wrist2_joint", "wrist3_joint"};
}

hardware_interface::CallbackReturn AuboI5System::on_init(
  const hardware_interface::HardwareInfo & system_info)
{
  if (hardware_interface::SystemInterface::on_init(system_info) !=
      hardware_interface::CallbackReturn::SUCCESS)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  if (info_.joints.size() != kJointNames.size())
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Expected exactly six arm joints.");
    return hardware_interface::CallbackReturn::ERROR;
  }
  for (std::size_t i = 0; i < kJointNames.size(); ++i)
  {
    const auto & joint = info_.joints[i];
    if (joint.name != kJointNames[i] || joint.command_interfaces.size() != 1 ||
        joint.command_interfaces[0].name != hardware_interface::HW_IF_POSITION ||
        joint.state_interfaces.size() != 2 ||
        joint.state_interfaces[0].name != hardware_interface::HW_IF_POSITION ||
        joint.state_interfaces[1].name != hardware_interface::HW_IF_VELOCITY)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"),
        "Joint interfaces must be position command plus position/velocity state, in AUBO i5 order.");
      return hardware_interface::CallbackReturn::ERROR;
    }
  }

  const auto parameter = [this](const std::string & name, const std::string & fallback) {
      const auto it = info_.hardware_parameters.find(name);
      return it == info_.hardware_parameters.end() ? fallback : it->second;
    };
  robot_ip_ = parameter("robot_ip", "");
  rpc_port_ = std::stoi(parameter("rpc_port", "30004"));
  rtde_port_ = std::stoi(parameter("rtde_port", "30010"));
  connect_timeout_ms_ = std::stoi(parameter("connect_timeout_ms", "1000"));
  command_timeout_ms_ = std::stoi(parameter("command_timeout_ms", "100"));
  max_command_velocity_ = std::stod(parameter("max_command_velocity", "0.20"));
  enable_motion_ = parameter("enable_motion", "false") == "true";
  if (robot_ip_.empty())
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "robot_ip must be explicitly provided.");
    return hardware_interface::CallbackReturn::ERROR;
  }
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn AuboI5System::on_activate(
  const rclcpp_lifecycle::State &)
{
  if (!connect_and_subscribe())
  {
    stop_servo();
    return hardware_interface::CallbackReturn::ERROR;
  }
  const auto deadline = std::chrono::steady_clock::now() +
    std::chrono::milliseconds(connect_timeout_ms_);
  while (!feedback_valid_ && std::chrono::steady_clock::now() < deadline)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  if (!feedback_is_fresh())
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "No valid RTDE joint feedback received.");
    stop_servo();
    return hardware_interface::CallbackReturn::ERROR;
  }
  std::lock_guard<std::mutex> lock(state_mutex_);
  position_command_ = position_state_;
  previous_command_ = position_state_;
  RCLCPP_INFO(rclcpp::get_logger("AuboI5System"),
    "AUBO i5 hardware activated in %s mode.", enable_motion_ ? "motion-enabled" : "read-only");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn AuboI5System::on_deactivate(
  const rclcpp_lifecycle::State &)
{
  stop_servo();
  connected_ = false;
  feedback_valid_ = false;
  rtde_client_.reset();
  rpc_client_.reset();
  return hardware_interface::CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface> AuboI5System::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> interfaces;
  for (std::size_t i = 0; i < kJointNames.size(); ++i)
  {
    interfaces.emplace_back(info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_state_[i]);
    interfaces.emplace_back(info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &velocity_state_[i]);
  }
  return interfaces;
}

std::vector<hardware_interface::CommandInterface> AuboI5System::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> interfaces;
  for (std::size_t i = 0; i < kJointNames.size(); ++i)
  {
    interfaces.emplace_back(info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_command_[i]);
  }
  return interfaces;
}

hardware_interface::return_type AuboI5System::read(const rclcpp::Time &, const rclcpp::Duration &)
{
  return feedback_is_fresh() ? hardware_interface::return_type::OK :
    hardware_interface::return_type::ERROR;
}

hardware_interface::return_type AuboI5System::write(
  const rclcpp::Time &, const rclcpp::Duration & period)
{
  if (!enable_motion_)
  {
    return hardware_interface::return_type::OK;
  }
  if (!feedback_is_fresh() || !command_is_safe(period) || !start_servo())
  {
    return hardware_interface::return_type::ERROR;
  }
  std::vector<double> target(position_command_.begin(), position_command_.end());
  try
  {
    const int result = rpc_client_->getRobotInterface(robot_name_)->getMotionControl()->servoJoint(
      target, 0.2, 0.2, 0.01, 0.1, 200);
    if (result < 0)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "servoJoint failed with code %d", result);
      return hardware_interface::return_type::ERROR;
    }
  }
  catch (const std::exception & exception)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "servoJoint exception: %s", exception.what());
    return hardware_interface::return_type::ERROR;
  }
  previous_command_ = position_command_;
  return hardware_interface::return_type::OK;
}

bool AuboI5System::connect_and_subscribe()
{
  const char * username = std::getenv("AUBO_ROBOT_USERNAME");
  const char * password = std::getenv("AUBO_ROBOT_PASSWORD");
  if (username == nullptr || password == nullptr)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"),
      "AUBO_ROBOT_USERNAME and AUBO_ROBOT_PASSWORD must be set in the process environment.");
    return false;
  }
  try
  {
    rpc_client_ = std::make_shared<arcs::aubo_sdk::RpcClient>();
    rpc_client_->setRequestTimeout(connect_timeout_ms_);
    rpc_client_->connect(robot_ip_, rpc_port_);
    rpc_client_->login(username, password);
    robot_name_ = rpc_client_->getRobotNames().front();

    rtde_client_ = std::make_shared<arcs::aubo_sdk::RtdeClient>();
    rtde_client_->connect(robot_ip_, rtde_port_);
    rtde_client_->login(username, password);
    const int topic = rtde_client_->setTopic(false,
      {"R1_actual_q", "R1_actual_qd", "R1_robot_mode", "R1_safety_mode"}, 125, 0);
    if (topic < 0)
    {
      return false;
    }
    rtde_client_->subscribe(topic, [this](arcs::common_interface::InputParser & parser) {
      const auto q = parser.popVectorDouble();
      const auto qd = parser.popVectorDouble();
      const auto robot_mode = parser.popRobotModeType();
      const auto safety_mode = parser.popSafetyModeType();
      if (q.size() != 6 || qd.size() != 6)
      {
        return;
      }
      std::lock_guard<std::mutex> lock(state_mutex_);
      std::copy_n(q.begin(), 6, position_state_.begin());
      std::copy_n(qd.begin(), 6, velocity_state_.begin());
      robot_mode_ = robot_mode;
      safety_mode_ = safety_mode;
      last_feedback_time_ = std::chrono::steady_clock::now();
      feedback_valid_ = true;
    });
    connected_ = true;
    return true;
  }
  catch (const std::exception & exception)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "AUBO connection failed: %s", exception.what());
    connected_ = false;
    return false;
  }
}

bool AuboI5System::feedback_is_fresh() const
{
  if (!connected_ || !feedback_valid_)
  {
    return false;
  }
  std::lock_guard<std::mutex> lock(state_mutex_);
  return std::chrono::steady_clock::now() - last_feedback_time_ <
    std::chrono::milliseconds(command_timeout_ms_);
}

bool AuboI5System::command_is_safe(const rclcpp::Duration & period) const
{
  std::lock_guard<std::mutex> lock(state_mutex_);
  const double maximum_delta = max_command_velocity_ * period.seconds();
  for (std::size_t i = 0; i < kJointNames.size(); ++i)
  {
    if (!std::isfinite(position_command_[i]) ||
        std::abs(position_command_[i] - previous_command_[i]) > maximum_delta)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Unsafe command for %s", kJointNames[i]);
      return false;
    }
  }
  return robot_mode_ == arcs::common_interface::RobotModeType::Running &&
    (safety_mode_ == arcs::common_interface::SafetyModeType::Normal ||
     safety_mode_ == arcs::common_interface::SafetyModeType::ReducedMode);
}

bool AuboI5System::start_servo()
{
  if (servo_started_)
  {
    return true;
  }
  try
  {
    rpc_client_->getRobotInterface(robot_name_)->getMotionControl()->setServoMode(true);
    servo_started_ = true;
    return true;
  }
  catch (const std::exception & exception)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Could not enter Servo mode: %s", exception.what());
    return false;
  }
}

void AuboI5System::stop_servo() noexcept
{
  if (!servo_started_ || !rpc_client_)
  {
    return;
  }
  try
  {
    rpc_client_->getRobotInterface(robot_name_)->getMotionControl()->setServoMode(false);
  }
  catch (...) {}
  servo_started_ = false;
}

}  // namespace aubo_i5_hardware

PLUGINLIB_EXPORT_CLASS(aubo_i5_hardware::AuboI5System, hardware_interface::SystemInterface)
