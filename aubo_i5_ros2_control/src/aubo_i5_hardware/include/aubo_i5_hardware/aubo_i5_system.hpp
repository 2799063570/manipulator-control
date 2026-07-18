#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp/duration.hpp"
#include "rclcpp_lifecycle/state.hpp"

#include <aubo/robot/robot_state.h>
#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"

namespace aubo_i5_hardware
{

class AuboI5System : public hardware_interface::SystemInterface
{
public:
  hardware_interface::CallbackReturn on_init(
    const hardware_interface::HardwareInfo & system_info) override;
  hardware_interface::CallbackReturn on_activate(
    const rclcpp_lifecycle::State & previous_state) override;
  hardware_interface::CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State & previous_state) override;

  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  hardware_interface::return_type read(
    const rclcpp::Time & time, const rclcpp::Duration & period) override;
  hardware_interface::return_type write(
    const rclcpp::Time & time, const rclcpp::Duration & period) override;

private:
  bool connect_and_subscribe();
  bool feedback_is_fresh() const;
  bool command_is_safe(const rclcpp::Duration & period) const;
  bool start_servo();
  void stop_servo() noexcept;

  std::string robot_ip_;
  std::string robot_name_;
  int rpc_port_{30004};
  int rtde_port_{30010};
  int connect_timeout_ms_{1000};
  int command_timeout_ms_{100};
  double max_command_velocity_{0.20};
  bool enable_motion_{false};

  std::shared_ptr<arcs::aubo_sdk::RpcClient> rpc_client_;
  std::shared_ptr<arcs::aubo_sdk::RtdeClient> rtde_client_;

  mutable std::mutex state_mutex_;
  std::array<double, 6> position_state_{};
  std::array<double, 6> velocity_state_{};
  // RTDE owns these snapshots.  State interfaces are only updated by read(),
  // which keeps the controller thread separate from the SDK callback thread.
  std::array<double, 6> feedback_position_{};
  std::array<double, 6> feedback_velocity_{};
  std::array<double, 6> position_command_{};
  std::array<double, 6> previous_command_{};
  std::vector<double> servo_target_{6, 0.0};
  std::chrono::steady_clock::time_point last_feedback_time_{};
  std::atomic<bool> connected_{false};
  std::atomic<bool> feedback_valid_{false};
  std::atomic<bool> servo_started_{false};
  arcs::common_interface::RobotModeType robot_mode_{
    arcs::common_interface::RobotModeType::NoController};
  arcs::common_interface::SafetyModeType safety_mode_{
    arcs::common_interface::SafetyModeType::Normal};
};

}  // namespace aubo_i5_hardware
