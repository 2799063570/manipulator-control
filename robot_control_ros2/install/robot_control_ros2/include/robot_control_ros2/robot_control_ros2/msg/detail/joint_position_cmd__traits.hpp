// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from robot_control_ros2:msg/JointPositionCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_position_cmd.hpp"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__TRAITS_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "robot_control_ros2/msg/detail/joint_position_cmd__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace robot_control_ros2
{

namespace msg
{

inline void to_flow_style_yaml(
  const JointPositionCmd & msg,
  std::ostream & out)
{
  out << "{";
  // member: position_cmd
  {
    if (msg.position_cmd.size() == 0) {
      out << "position_cmd: []";
    } else {
      out << "position_cmd: [";
      size_t pending_items = msg.position_cmd.size();
      for (auto item : msg.position_cmd) {
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
  const JointPositionCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: position_cmd
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.position_cmd.size() == 0) {
      out << "position_cmd: []\n";
    } else {
      out << "position_cmd:\n";
      for (auto item : msg.position_cmd) {
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

inline std::string to_yaml(const JointPositionCmd & msg, bool use_flow_style = false)
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
  const robot_control_ros2::msg::JointPositionCmd & msg,
  std::ostream & out, size_t indentation = 0)
{
  robot_control_ros2::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use robot_control_ros2::msg::to_yaml() instead")]]
inline std::string to_yaml(const robot_control_ros2::msg::JointPositionCmd & msg)
{
  return robot_control_ros2::msg::to_yaml(msg);
}

template<>
inline const char * data_type<robot_control_ros2::msg::JointPositionCmd>()
{
  return "robot_control_ros2::msg::JointPositionCmd";
}

template<>
inline const char * name<robot_control_ros2::msg::JointPositionCmd>()
{
  return "robot_control_ros2/msg/JointPositionCmd";
}

template<>
struct has_fixed_size<robot_control_ros2::msg::JointPositionCmd>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<robot_control_ros2::msg::JointPositionCmd>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<robot_control_ros2::msg::JointPositionCmd>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__TRAITS_HPP_
