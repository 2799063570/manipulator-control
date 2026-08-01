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

/**
 * @brief AUBO i5 真机的 ros2_control 系统级硬件插件。
 *
 * controller_manager 通过 pluginlib 创建本类，并按照生命周期依次调用
 * on_init()、on_activate()、周期性的 read()/write()，最后调用 on_deactivate()。
 * 本插件使用两条相互独立的 AUBO SDK 通道：
 *
 * - RTDE：以回调方式接收六个关节的位置、速度以及机器人/安全模式；
 * - RPC：在明确启用运动且所有安全检查通过后，下发 ServoJ 位置目标。
 *
 * RTDE 回调运行在 SDK 线程中，只写入 feedback_* 快照；ros2_control 控制线程
 * 再由 read() 将快照复制到 state interface，以免两个线程直接操作同一状态数组。
 */
class AuboI5System : public hardware_interface::SystemInterface
{
public:
  /// 校验六轴接口定义，读取并检查 URDF 中的硬件参数；此阶段不连接真机。
  hardware_interface::CallbackReturn on_init(
    const hardware_interface::HardwareInfo & system_info) override;
  /// 连接 RPC/RTDE、等待首帧有效反馈，并用实测位置初始化位置命令。
  hardware_interface::CallbackReturn on_activate(
    const rclcpp_lifecycle::State & previous_state) override;
  /// 退出 Servo 模式，注销并断开两条 SDK 连接。
  hardware_interface::CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State & previous_state) override;

  /// 向 ros2_control 导出每轴的位置和速度状态接口。
  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  /// 向 ros2_control 导出每轴的位置命令接口。
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  /// 将 RTDE 反馈快照发布到 ros2_control 状态接口。
  hardware_interface::return_type read(
    const rclcpp::Time & time, const rclcpp::Duration & period) override;
  /// 通过安全门控后，以 ServoJ 命令向真机写入六轴位置目标。
  hardware_interface::return_type write(
    const rclcpp::Time & time, const rclcpp::Duration & period) override;

private:
  /// 创建并登录 RPC/RTDE 客户端，同时注册实时反馈回调。
  bool connect_and_subscribe();
  /// 检查连接、反馈有效标志以及最后一帧反馈是否超时。
  bool feedback_is_fresh() const;
  /// 检查命令数值、单周期位置增量、机器人模式和安全模式。
  bool command_is_safe(const rclcpp::Duration & period) const;
  /// 按需进入并确认控制柜的 Servo 模式；可重复调用。
  bool start_servo();
  /// 尽力退出 Servo 模式；用于停用和所有错误路径，不向外抛异常。
  void stop_servo() noexcept;

  // 来自 <ros2_control><hardware> 的连接与安全参数。
  std::string robot_ip_;
  std::string robot_name_;
  int rpc_port_{30004};
  int rtde_port_{30010};
  int connect_timeout_ms_{1000};
  int command_timeout_ms_{100};
  double max_command_velocity_{0.20};
  bool enable_motion_{false};

  // RPC 负责发送控制命令，RTDE 负责接收实时状态。
  std::shared_ptr<arcs::aubo_sdk::RpcClient> rpc_client_;
  std::shared_ptr<arcs::aubo_sdk::RtdeClient> rtde_client_;

  // 保护 RTDE 回调线程与 ros2_control 控制线程共享的数据。
  mutable std::mutex state_mutex_;
  // ros2_control 对外可见的状态数组，只由 read() 更新。
  std::array<double, 6> position_state_{};
  std::array<double, 6> velocity_state_{};
  // RTDE 回调写入的最新快照；与上面的状态接口分离，以隔离 SDK 回调线程。
  std::array<double, 6> feedback_position_{};
  std::array<double, 6> feedback_velocity_{};
  // 控制器写入 position_command_；previous_command_ 用于限制单周期位置跳变。
  std::array<double, 6> position_command_{};
  std::array<double, 6> previous_command_{};
  // AUBO SDK 的 servoJoint() 接收 vector，因此发送前将定长命令复制到这里。
  std::vector<double> servo_target_{6, 0.0};
  // steady_clock 不受系统时间校准影响，适合判断实时反馈是否超时。
  std::chrono::steady_clock::time_point last_feedback_time_{};
  // 这些标志会跨 SDK 回调线程与控制线程访问，使用原子类型避免数据竞争。
  std::atomic<bool> connected_{false};
  std::atomic<bool> feedback_valid_{false};
  std::atomic<bool> servo_started_{false};
  // 最近一次 RTDE 上报的控制柜状态，write() 只接受允许运动的状态组合。
  arcs::common_interface::RobotModeType robot_mode_{
    arcs::common_interface::RobotModeType::NoController};
  arcs::common_interface::SafetyModeType safety_mode_{
    arcs::common_interface::SafetyModeType::Normal};
};

}  // namespace aubo_i5_hardware
