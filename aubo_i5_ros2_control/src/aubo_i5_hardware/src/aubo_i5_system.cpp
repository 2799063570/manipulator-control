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
// 顺序必须与 URDF/ros2_control 配置以及 AUBO SDK 返回的六轴向量完全一致。
constexpr std::array<const char *, 6> kJointNames{
  "shoulder_joint", "upperArm_joint", "foreArm_joint",
  "wrist1_joint", "wrist2_joint", "wrist3_joint"};
}

// 生命周期阶段 1：只解析静态配置，不访问网络，便于尽早发现模型配置错误。
hardware_interface::CallbackReturn AuboI5System::on_init(
  const hardware_interface::HardwareInfo & system_info)
{
  // 先让基类保存 HardwareInfo；后续通过 info_ 读取关节和硬件参数。
  if (hardware_interface::SystemInterface::on_init(system_info) !=
      hardware_interface::CallbackReturn::SUCCESS)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  // 插件仅支持 AUBO i5 的固定六轴布局。严格检查名称、顺序和接口类型，
  // 防止 SDK 数组与 ROS 关节错误对应后产生危险运动。
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

  // 未在 Xacro/URDF 中填写的参数使用保守默认值；robot_ip 是唯一无默认值的参数。
  const auto parameter = [this](const std::string & name, const std::string & fallback) {
      const auto it = info_.hardware_parameters.find(name);
      return it == info_.hardware_parameters.end() ? fallback : it->second;
    };
  try
  {
    robot_ip_ = parameter("robot_ip", "");
    rpc_port_ = std::stoi(parameter("rpc_port", "30004"));
    rtde_port_ = std::stoi(parameter("rtde_port", "30010"));
    connect_timeout_ms_ = std::stoi(parameter("connect_timeout_ms", "1000"));
    command_timeout_ms_ = std::stoi(parameter("command_timeout_ms", "100"));
    max_command_velocity_ = std::stod(parameter("max_command_velocity", "0.20"));
    enable_motion_ = parameter("enable_motion", "false") == "true";
  }
  catch (const std::exception & exception)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"),
      "Invalid hardware parameter: %s", exception.what());
    return hardware_interface::CallbackReturn::ERROR;
  }
  if (robot_ip_.empty())
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "robot_ip must be explicitly provided.");
    return hardware_interface::CallbackReturn::ERROR;
  }
  if (rpc_port_ <= 0 || rtde_port_ <= 0 || connect_timeout_ms_ <= 0 ||
    command_timeout_ms_ <= 0 || !std::isfinite(max_command_velocity_) ||
    max_command_velocity_ <= 0.0)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"),
      "Ports, timeouts and max_command_velocity must be positive finite values.");
    return hardware_interface::CallbackReturn::ERROR;
  }
  return hardware_interface::CallbackReturn::SUCCESS;
}

// 生命周期阶段 2：建立真机通信，并在控制器运行前准备一致的初始命令。
hardware_interface::CallbackReturn AuboI5System::on_activate(
  const rclcpp_lifecycle::State &)
{
  if (!connect_and_subscribe())
  {
    stop_servo();
    return hardware_interface::CallbackReturn::ERROR;
  }
  // RTDE 是异步回调，这里最多等待 connect_timeout_ms_，直到收到首帧有效数据。
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
  {
    // 在锁内取得同一帧的位置和速度，避免复制过程中 RTDE 回调改写快照。
    std::lock_guard<std::mutex> lock(state_mutex_);
    position_state_ = feedback_position_;
    velocity_state_ = feedback_velocity_;
  }
  // 命令从真机当前位置起步，避免控制器激活时默认零值引发位置跳变。
  position_command_ = position_state_;
  previous_command_ = position_state_;
  std::copy(position_state_.begin(), position_state_.end(), servo_target_.begin());
  RCLCPP_INFO(rclcpp::get_logger("AuboI5System"),
    "AUBO i5 hardware activated in %s mode.", enable_motion_ ? "motion-enabled" : "read-only");
  return hardware_interface::CallbackReturn::SUCCESS;
}

// 生命周期阶段 3：先撤销运动能力，再释放网络资源。
hardware_interface::CallbackReturn AuboI5System::on_deactivate(
  const rclcpp_lifecycle::State &)
{
  stop_servo();
  connected_ = false;
  feedback_valid_ = false;
  // 停用过程应尽量完成清理，因此 SDK 注销/断开异常在这里被吞掉。
  if (rtde_client_)
  {
    try
    {
      rtde_client_->logout();
      rtde_client_->disconnect();
    }
    catch (...) {}
  }
  if (rpc_client_)
  {
    try
    {
      rpc_client_->logout();
      rpc_client_->disconnect();
    }
    catch (...) {}
  }
  rtde_client_.reset();
  rpc_client_.reset();
  return hardware_interface::CallbackReturn::SUCCESS;
}

// controller_manager 使用这些指针直接读取 position_state_/velocity_state_。
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

// 轨迹控制器通过这些指针写入 position_command_，真正发送发生在 write() 中。
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
  // 状态超时意味着控制线程已无法确认真机实际位置，立即退出 Servo 并报告错误。
  if (!feedback_is_fresh())
  {
    feedback_valid_ = false;
    stop_servo();
    return hardware_interface::return_type::ERROR;
  }

  std::lock_guard<std::mutex> lock(state_mutex_);
  position_state_ = feedback_position_;
  velocity_state_ = feedback_velocity_;
  return hardware_interface::return_type::OK;
}

hardware_interface::return_type AuboI5System::write(
  const rclcpp::Time &, const rclcpp::Duration & period)
{
  // 默认只读：即使上层控制器写入命令，本插件也不会进入 Servo 或调用运动接口。
  if (!enable_motion_)
  {
    return hardware_interface::return_type::OK;
  }
  // 发送前必须同时满足反馈新鲜、命令增量安全、机器人运行且安全模式允许。
  if (!feedback_is_fresh() || !command_is_safe(period))
  {
    stop_servo();
    return hardware_interface::return_type::ERROR;
  }
  // 首次写入时才进入 Servo 模式，之后 start_servo() 直接返回成功。
  if (!start_servo())
  {
    stop_servo();
    connected_ = false;
    return hardware_interface::return_type::ERROR;
  }
  std::copy(position_command_.begin(), position_command_.end(), servo_target_.begin());
  try
  {
    // 六轴目标单位为 rad；period.seconds() 使 SDK 插补周期与 controller_manager 一致。
    // 其余常量为 SDK 的 ServoJ 参数，修改前需对照现场 SDK 版本和厂家手册验证。
    const int result = rpc_client_->getRobotInterface(robot_name_)->getMotionControl()->servoJoint(
      servo_target_, 0.2, 0.2, period.seconds(), 0.1, 200);
    if (result < 0)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "servoJoint failed with code %d", result);
      stop_servo();
      connected_ = false;
      return hardware_interface::return_type::ERROR;
    }
  }
  catch (const std::exception & exception)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "servoJoint exception: %s", exception.what());
    stop_servo();
    connected_ = false;
    return hardware_interface::return_type::ERROR;
  }
  // 仅成功发送后更新基准；下一周期的限速检查以本次已接受命令为起点。
  previous_command_ = position_command_;
  return hardware_interface::return_type::OK;
}

bool AuboI5System::connect_and_subscribe()
{
  // 每次激活都从干净状态开始，避免沿用上次连接残留的有效标志或机器人名。
  connected_ = false;
  feedback_valid_ = false;
  robot_name_.clear();
  const char * username = std::getenv("AUBO_ROBOT_USERNAME");
  const char * password = std::getenv("AUBO_ROBOT_PASSWORD");
  // 凭据只从进程环境读取，避免把用户名和密码写进可提交的 URDF/launch 文件。
  if (username == nullptr || password == nullptr)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"),
      "AUBO_ROBOT_USERNAME and AUBO_ROBOT_PASSWORD must be set in the process environment.");
    return false;
  }
  try
  {
    // RPC 通道：用于枚举机器人、切换 Servo 模式和下发位置命令。
    rpc_client_ = std::make_shared<arcs::aubo_sdk::RpcClient>();
    if (rpc_client_->setRequestTimeout(connect_timeout_ms_) != 0 ||
      rpc_client_->connect(robot_ip_, rpc_port_) != 0 ||
      rpc_client_->login(username, password) != 0)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "RPC connection or login failed.");
      return false;
    }
    const auto robot_names = rpc_client_->getRobotNames();
    if (robot_names.empty())
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "The controller returned no robot names.");
      return false;
    }
    robot_name_ = robot_names.front();

    // RTDE 通道：独立接收高频状态，不让 RPC 命令请求阻塞状态采集。
    rtde_client_ = std::make_shared<arcs::aubo_sdk::RtdeClient>();
    if (rtde_client_->connect(robot_ip_, rtde_port_) < 0 ||
      rtde_client_->login(username, password) != 0)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "RTDE connection or login failed.");
      return false;
    }
    // 以 125 Hz 订阅六轴位置/速度和两个安全相关模式字段。
    const int topic = rtde_client_->setTopic(false,
      {"R1_actual_q", "R1_actual_qd", "R1_robot_mode", "R1_safety_mode"}, 125, 0);
    if (topic < 0)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Could not create the RTDE feedback topic.");
      return false;
    }
    if (rtde_client_->subscribe(topic, [this](arcs::aubo_sdk::InputParser & parser) {
      // pop 顺序必须与 setTopic() 的字段顺序一一对应。
      const auto q = parser.popVectorDouble();
      const auto qd = parser.popVectorDouble();
      const auto robot_mode = parser.popRobotModeType();
      const auto safety_mode = parser.popSafetyModeType();
      // 丢弃维度错误或含 NaN/Inf 的整帧数据，不污染最后一帧有效快照。
      if (q.size() != 6 || qd.size() != 6 ||
        !std::all_of(q.begin(), q.end(), [](double value) {return std::isfinite(value);}) ||
        !std::all_of(qd.begin(), qd.end(), [](double value) {return std::isfinite(value);}))
      {
        return;
      }
      // 原子标志负责快速判定，成组状态仍需互斥锁保证快照一致性。
      std::lock_guard<std::mutex> lock(state_mutex_);
      std::copy_n(q.begin(), 6, feedback_position_.begin());
      std::copy_n(qd.begin(), 6, feedback_velocity_.begin());
      robot_mode_ = robot_mode;
      safety_mode_ = safety_mode;
      last_feedback_time_ = std::chrono::steady_clock::now();
      feedback_valid_ = true;
    }) != 0)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Could not subscribe to RTDE feedback.");
      return false;
    }
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
  // last_feedback_time_ 与快照在同一把锁下更新和读取。
  std::lock_guard<std::mutex> lock(state_mutex_);
  return std::chrono::steady_clock::now() - last_feedback_time_ <
    std::chrono::milliseconds(command_timeout_ms_);
}

bool AuboI5System::command_is_safe(const rclcpp::Duration & period) const
{
  // 将最大关节速度(rad/s)换算成当前控制周期允许的位置增量(rad)。
  const double maximum_delta = max_command_velocity_ * period.seconds();
  if (!std::isfinite(maximum_delta) || maximum_delta <= 0.0)
  {
    RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Invalid controller update period.");
    return false;
  }
  for (std::size_t i = 0; i < kJointNames.size(); ++i)
  {
    if (!std::isfinite(position_command_[i]) ||
        std::abs(position_command_[i] - previous_command_[i]) > maximum_delta)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Unsafe command for %s", kJointNames[i]);
      return false;
    }
  }
  // Normal 和 ReducedMode 是此插件唯一接受的安全状态；急停、保护停机等均拒绝写入。
  std::lock_guard<std::mutex> lock(state_mutex_);
  return robot_mode_ == arcs::common_interface::RobotModeType::Running &&
    (safety_mode_ == arcs::common_interface::SafetyModeType::Normal ||
     safety_mode_ == arcs::common_interface::SafetyModeType::ReducedMode);
}

bool AuboI5System::start_servo()
{
  // 已确认进入 Servo 模式时避免每个控制周期重复调用 setServoMode(true)。
  if (servo_started_ && rpc_client_)
  {
    return true;
  }
  try
  {
    if (!rpc_client_ || robot_name_.empty())
    {
      return false;
    }
    const auto motion = rpc_client_->getRobotInterface(robot_name_)->getMotionControl();
    if (motion->setServoMode(true) != 0)
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Could not enter Servo mode.");
      return false;
    }
    // 控制柜切换模式存在短暂延迟，最多轮询 5 次，每次间隔 5 ms。
    for (int attempt = 0; attempt < 5 && !motion->isServoModeEnabled(); ++attempt)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    if (!motion->isServoModeEnabled())
    {
      RCLCPP_ERROR(rclcpp::get_logger("AuboI5System"), "Could not confirm Servo mode.");
      return false;
    }
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
    // 这是错误处理/析构式清理路径，关闭失败也不能继续向外抛异常。
    rpc_client_->getRobotInterface(robot_name_)->getMotionControl()->setServoMode(false);
  }
  catch (...) {}
  servo_started_ = false;
}

}  // namespace aubo_i5_hardware

// 向 pluginlib 注册实现；hardware_interface_plugin.xml 中的类型名必须与此处一致。
PLUGINLIB_EXPORT_CLASS(aubo_i5_hardware::AuboI5System, hardware_interface::SystemInterface)
