#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include <cerrno>
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>

#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_msgs/msg/float32.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"

namespace
{
constexpr uint8_t kFrameHeader = 0x7b;
constexpr uint8_t kFrameTail = 0x7d;
constexpr std::size_t kCommandFrameSize = 11;
constexpr std::size_t kStateFrameSize = 24;
constexpr float kAccelerometerScale = 1671.84F;  // Raw values for +/- 2 g.
constexpr float kGyroscopeScale = 0.00026644F;  // Raw values for +/- 500 deg/s.

speed_t baud_to_termios(const int baud_rate)
{
  switch (baud_rate) {
    case 9600: return B9600;
    case 19200: return B19200;
    case 38400: return B38400;
    case 57600: return B57600;
    case 115200: return B115200;
    default:
      throw std::invalid_argument("Unsupported baud_rate. Supported values: 9600, 19200, 38400, 57600, 115200.");
  }
}

int16_t read_be_i16(const uint8_t high, const uint8_t low)
{
  const auto value = static_cast<uint16_t>((static_cast<uint16_t>(high) << 8U) | low);
  return static_cast<int16_t>(value);
}

uint16_t read_be_u16(const uint8_t high, const uint8_t low)
{
  return static_cast<uint16_t>((static_cast<uint16_t>(high) << 8U) | low);
}

uint8_t xor_checksum(const uint8_t * data, const std::size_t length)
{
  uint8_t result = 0;
  for (std::size_t index = 0; index < length; ++index) {
    result ^= data[index];
  }
  return result;
}

double clamp(const double value, const double low, const double high)
{
  return std::max(low, std::min(value, high));
}
}  // namespace

class WheeltecBaseDriver final : public rclcpp::Node
{
public:
  WheeltecBaseDriver()
  : Node("wheeltec_base_driver")
  {
    serial_port_ = declare_parameter<std::string>("serial_port", "/dev/wheeltec_controller");
    baud_rate_ = declare_parameter<int>("baud_rate", 115200);
    odom_frame_ = declare_parameter<std::string>("odom_frame", "odom");
    base_frame_ = declare_parameter<std::string>("base_frame", "base_footprint");
    imu_frame_ = declare_parameter<std::string>("imu_frame", "gyro_link");
    publish_tf_ = declare_parameter<bool>("publish_tf", true);
    command_timeout_ = declare_parameter<double>("command_timeout_sec", 0.5);
    max_linear_x_ = declare_parameter<double>("max_linear_x", 0.35);
    max_linear_y_ = declare_parameter<double>("max_linear_y", 0.35);
    max_angular_z_ = declare_parameter<double>("max_angular_z", 1.0);
    accept_lateral_velocity_ = declare_parameter<bool>("accept_lateral_velocity", true);
    linear_command_scale_ = declare_parameter<double>("linear_command_scale", 1000.0);
    angular_command_scale_ = declare_parameter<double>("angular_command_scale", 1000.0);
    odom_x_scale_ = declare_parameter<double>("odom_x_scale", 1.0);
    odom_y_scale_ = declare_parameter<double>("odom_y_scale", 1.0);
    odom_z_scale_positive_ = declare_parameter<double>("odom_z_scale_positive", 1.0);
    odom_z_scale_negative_ = declare_parameter<double>("odom_z_scale_negative", 1.0);

    if (command_timeout_ <= 0.0 || linear_command_scale_ <= 0.0 || angular_command_scale_ <= 0.0) {
      throw std::invalid_argument("command_timeout_sec and command scales must be greater than zero.");
    }

    odom_publisher_ = create_publisher<nav_msgs::msg::Odometry>("odom", rclcpp::SensorDataQoS());
    imu_publisher_ = create_publisher<sensor_msgs::msg::Imu>("imu", rclcpp::SensorDataQoS());
    voltage_publisher_ = create_publisher<std_msgs::msg::Float32>("PowerVoltage", 10);
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    command_subscription_ = create_subscription<geometry_msgs::msg::Twist>(
      "cmd_vel", rclcpp::QoS(10),
      std::bind(&WheeltecBaseDriver::on_cmd_vel, this, std::placeholders::_1));

    last_command_time_ = std::chrono::steady_clock::now();
    open_serial();
    running_.store(true);
    reader_thread_ = std::thread(&WheeltecBaseDriver::reader_loop, this);
    send_velocity(0.0, 0.0, 0.0);

    watchdog_timer_ = create_wall_timer(
      std::chrono::milliseconds(50), std::bind(&WheeltecBaseDriver::watchdog, this));

    RCLCPP_INFO(
      get_logger(), "Wheeltec base driver ready on %s at %d baud. Waiting for /cmd_vel.",
      serial_port_.c_str(), baud_rate_);
  }

  ~WheeltecBaseDriver() override
  {
    stop_serial();
  }

private:
  void open_serial()
  {
    serial_fd_ = ::open(serial_port_.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (serial_fd_ < 0) {
      throw std::runtime_error("Cannot open serial port " + serial_port_ + ": " + std::strerror(errno));
    }

    termios options{};
    if (tcgetattr(serial_fd_, &options) != 0) {
      const auto message = std::string("Cannot read serial settings: ") + std::strerror(errno);
      ::close(serial_fd_);
      serial_fd_ = -1;
      throw std::runtime_error(message);
    }

    cfmakeraw(&options);
    const auto speed = baud_to_termios(baud_rate_);
    cfsetispeed(&options, speed);
    cfsetospeed(&options, speed);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CRTSCTS;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;
    if (tcsetattr(serial_fd_, TCSANOW, &options) != 0) {
      const auto message = std::string("Cannot configure serial port: ") + std::strerror(errno);
      ::close(serial_fd_);
      serial_fd_ = -1;
      throw std::runtime_error(message);
    }
    tcflush(serial_fd_, TCIOFLUSH);
  }

  void stop_serial()
  {
    if (serial_fd_ < 0) {
      return;
    }
    send_velocity(0.0, 0.0, 0.0);
    running_.store(false);
    if (reader_thread_.joinable()) {
      reader_thread_.join();
    }
    ::close(serial_fd_);
    serial_fd_ = -1;
  }

  void on_cmd_vel(const geometry_msgs::msg::Twist::SharedPtr command)
  {
    const double x = clamp(command->linear.x, -max_linear_x_, max_linear_x_);
    const double y = accept_lateral_velocity_ ? clamp(command->linear.y, -max_linear_y_, max_linear_y_) : 0.0;
    const double z = clamp(command->angular.z, -max_angular_z_, max_angular_z_);
    send_velocity(x, y, z);
    last_command_time_ = std::chrono::steady_clock::now();
    timeout_stop_sent_.store(false);
  }

  void watchdog()
  {
    const auto elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - last_command_time_).count();
    if (elapsed > command_timeout_ && !timeout_stop_sent_.exchange(true)) {
      RCLCPP_WARN(get_logger(), "No /cmd_vel for %.2f s; sending a zero velocity command.", elapsed);
      send_velocity(0.0, 0.0, 0.0);
    }
  }

  void send_velocity(const double x, const double y, const double z)
  {
    if (serial_fd_ < 0) {
      return;
    }

    const auto encode = [](const double value, const double scale) {
        const auto scaled = std::lround(value * scale);
        return static_cast<int16_t>(std::clamp<long>(
          scaled, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()));
      };
    const auto x_encoded = encode(x, linear_command_scale_);
    const auto y_encoded = encode(y, linear_command_scale_);
    const auto z_encoded = encode(z, angular_command_scale_);

    std::array<uint8_t, kCommandFrameSize> frame{};
    frame[0] = kFrameHeader;
    frame[1] = 0;
    frame[2] = 0;
    frame[3] = static_cast<uint8_t>((static_cast<uint16_t>(x_encoded) >> 8U) & 0xffU);
    frame[4] = static_cast<uint8_t>(static_cast<uint16_t>(x_encoded) & 0xffU);
    frame[5] = static_cast<uint8_t>((static_cast<uint16_t>(y_encoded) >> 8U) & 0xffU);
    frame[6] = static_cast<uint8_t>(static_cast<uint16_t>(y_encoded) & 0xffU);
    frame[7] = static_cast<uint8_t>((static_cast<uint16_t>(z_encoded) >> 8U) & 0xffU);
    frame[8] = static_cast<uint8_t>(static_cast<uint16_t>(z_encoded) & 0xffU);
    frame[9] = xor_checksum(frame.data(), 9);
    frame[10] = kFrameTail;

    std::lock_guard<std::mutex> lock(write_mutex_);
    const auto written = ::write(serial_fd_, frame.data(), frame.size());
    if (written != static_cast<ssize_t>(frame.size())) {
      RCLCPP_ERROR(get_logger(), "Failed to write complete command frame: %s", std::strerror(errno));
    }
  }

  void reader_loop()
  {
    std::array<uint8_t, kStateFrameSize> frame{};
    std::size_t index = 0;
    while (running_.load()) {
      pollfd descriptor{serial_fd_, POLLIN, 0};
      const auto result = ::poll(&descriptor, 1, 100);
      if (result <= 0) {
        continue;
      }

      uint8_t byte = 0;
      const auto bytes_read = ::read(serial_fd_, &byte, 1);
      if (bytes_read != 1) {
        continue;
      }
      if (index == 0) {
        if (byte == kFrameHeader) {
          frame[index++] = byte;
        }
        continue;
      }

      frame[index++] = byte;
      if (index != kStateFrameSize) {
        continue;
      }
      index = 0;
      if (frame.back() != kFrameTail || xor_checksum(frame.data(), 22) != frame[22]) {
        RCLCPP_WARN(get_logger(), "Discarded an invalid Wheeltec state frame.");
        continue;
      }
      publish_state(frame);
    }
  }

  void publish_state(const std::array<uint8_t, kStateFrameSize> & frame)
  {
    double vx = static_cast<double>(read_be_i16(frame[2], frame[3])) / 1000.0 * odom_x_scale_;
    double vy = static_cast<double>(read_be_i16(frame[4], frame[5])) / 1000.0 * odom_y_scale_;
    double wz = static_cast<double>(read_be_i16(frame[6], frame[7])) / 1000.0;
    wz *= wz >= 0.0 ? odom_z_scale_positive_ : odom_z_scale_negative_;

    const auto stamp = now();
    if (have_last_state_) {
      const auto dt = (stamp - last_state_stamp_).seconds();
      if (dt > 0.0 && dt < 1.0) {
        pose_x_ += (vx * std::cos(pose_yaw_) - vy * std::sin(pose_yaw_)) * dt;
        pose_y_ += (vx * std::sin(pose_yaw_) + vy * std::cos(pose_yaw_)) * dt;
        pose_yaw_ += wz * dt;
      }
    }
    have_last_state_ = true;
    last_state_stamp_ = stamp;

    tf2::Quaternion orientation;
    orientation.setRPY(0.0, 0.0, pose_yaw_);

    nav_msgs::msg::Odometry odom;
    odom.header.stamp = stamp;
    odom.header.frame_id = odom_frame_;
    odom.child_frame_id = base_frame_;
    odom.pose.pose.position.x = pose_x_;
    odom.pose.pose.position.y = pose_y_;
    odom.pose.pose.orientation.x = orientation.x();
    odom.pose.pose.orientation.y = orientation.y();
    odom.pose.pose.orientation.z = orientation.z();
    odom.pose.pose.orientation.w = orientation.w();
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = wz;
    odom.pose.covariance[0] = 1e-3;
    odom.pose.covariance[7] = 1e-3;
    odom.pose.covariance[14] = 1e6;
    odom.pose.covariance[21] = 1e6;
    odom.pose.covariance[28] = 1e6;
    odom.pose.covariance[35] = 1e3;
    odom.twist.covariance = odom.pose.covariance;
    odom_publisher_->publish(odom);

    if (publish_tf_) {
      geometry_msgs::msg::TransformStamped transform;
      transform.header = odom.header;
      transform.child_frame_id = base_frame_;
      transform.transform.translation.x = pose_x_;
      transform.transform.translation.y = pose_y_;
      transform.transform.rotation = odom.pose.pose.orientation;
      tf_broadcaster_->sendTransform(transform);
    }

    sensor_msgs::msg::Imu imu;
    imu.header.stamp = stamp;
    imu.header.frame_id = imu_frame_;
    imu.orientation_covariance[0] = -1.0;  // The protocol supplies raw IMU only; do not claim an orientation estimate.
    imu.angular_velocity.x = static_cast<double>(read_be_i16(frame[14], frame[15])) * kGyroscopeScale;
    imu.angular_velocity.y = static_cast<double>(read_be_i16(frame[16], frame[17])) * kGyroscopeScale;
    imu.angular_velocity.z = static_cast<double>(read_be_i16(frame[18], frame[19])) * kGyroscopeScale;
    imu.linear_acceleration.x = static_cast<double>(read_be_i16(frame[8], frame[9])) / kAccelerometerScale;
    imu.linear_acceleration.y = static_cast<double>(read_be_i16(frame[10], frame[11])) / kAccelerometerScale;
    imu.linear_acceleration.z = static_cast<double>(read_be_i16(frame[12], frame[13])) / kAccelerometerScale;
    imu_publisher_->publish(imu);

    std_msgs::msg::Float32 voltage;
    voltage.data = static_cast<float>(read_be_u16(frame[20], frame[21])) / 1000.0F;
    voltage_publisher_->publish(voltage);
  }

  std::string serial_port_;
  int baud_rate_{};
  std::string odom_frame_;
  std::string base_frame_;
  std::string imu_frame_;
  bool publish_tf_{};
  double command_timeout_{};
  double max_linear_x_{};
  double max_linear_y_{};
  double max_angular_z_{};
  bool accept_lateral_velocity_{};
  double linear_command_scale_{};
  double angular_command_scale_{};
  double odom_x_scale_{};
  double odom_y_scale_{};
  double odom_z_scale_positive_{};
  double odom_z_scale_negative_{};

  int serial_fd_{-1};
  std::atomic_bool running_{false};
  std::atomic_bool timeout_stop_sent_{false};
  std::thread reader_thread_;
  std::mutex write_mutex_;
  std::chrono::steady_clock::time_point last_command_time_;
  rclcpp::Time last_state_stamp_{0, 0, RCL_ROS_TIME};
  bool have_last_state_{false};
  double pose_x_{0.0};
  double pose_y_{0.0};
  double pose_yaw_{0.0};

  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publisher_;
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr voltage_publisher_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr command_subscription_;
  rclcpp::TimerBase::SharedPtr watchdog_timer_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  try {
    rclcpp::spin(std::make_shared<WheeltecBaseDriver>());
  } catch (const std::exception & error) {
    RCLCPP_FATAL(rclcpp::get_logger("wheeltec_base_driver"), "%s", error.what());
    rclcpp::shutdown();
    return 1;
  }
  rclcpp::shutdown();
  return 0;
}
