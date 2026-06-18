// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from robot_control_ros2:msg/JointVelocityCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_velocity_cmd.hpp"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__TRAITS_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "robot_control_ros2/msg/detail/joint_velocity_cmd__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace robot_control_ros2
{

namespace msg
{

inline void to_flow_style_yaml(
  const JointVelocityCmd & msg,
  std::ostream & out)
{
  out << "{";
  // member: velocity_cmd
  {
    if (msg.velocity_cmd.size() == 0) {
      out << "velocity_cmd: []";
    } else {
      out << "velocity_cmd: [";
      size_t pending_items = msg.velocity_cmd.size();
      for (auto item : msg.velocity_cmd) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const JointVelocityCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: velocity_cmd
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.velocity_cmd.size() == 0) {
      out << "velocity_cmd: []\n";
    } else {
      out << "velocity_cmd:\n";
      for (auto item : msg.velocity_cmd) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const JointVelocityCmd & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace robot_control_ros2

namespace rosidl_generator_traits
{

[[deprecated("use robot_control_ros2::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const robot_control_ros2::msg::JointVelocityCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  robot_control_ros2::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use robot_control_ros2::msg::to_yaml() instead")]]
inline std::string to_yaml(const robot_control_ros2::msg::JointVelocityCmd & msg)
{
  return robot_control_ros2::msg::to_yaml(msg);
}

template<>
inline const char * data_type<robot_control_ros2::msg::JointVelocityCmd>()
{
  return "robot_control_ros2::msg::JointVelocityCmd";
}

template<>
inline const char * name<robot_control_ros2::msg::JointVelocityCmd>()
{
  return "robot_control_ros2/msg/JointVelocityCmd";
}

template<>
struct has_fixed_size<robot_control_ros2::msg::JointVelocityCmd>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<robot_control_ros2::msg::JointVelocityCmd>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<robot_control_ros2::msg::JointVelocityCmd>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__TRAITS_HPP_
