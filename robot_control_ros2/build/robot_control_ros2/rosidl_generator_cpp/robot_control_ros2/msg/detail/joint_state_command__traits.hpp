// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from robot_control_ros2:msg/JointStateCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_state_command.hpp"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__TRAITS_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "robot_control_ros2/msg/detail/joint_state_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace robot_control_ros2
{

namespace msg
{

inline void to_flow_style_yaml(
  const JointStateCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: position
  {
    if (msg.position.size() == 0) {
      out << "position: []";
    } else {
      out << "position: [";
      size_t pending_items = msg.position.size();
      for (auto item : msg.position) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: velocity
  {
    if (msg.velocity.size() == 0) {
      out << "velocity: []";
    } else {
      out << "velocity: [";
      size_t pending_items = msg.velocity.size();
      for (auto item : msg.velocity) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: is_exc
  {
    out << "is_exc: ";
    rosidl_generator_traits::value_to_yaml(msg.is_exc, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const JointStateCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: position
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.position.size() == 0) {
      out << "position: []\n";
    } else {
      out << "position:\n";
      for (auto item : msg.position) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: velocity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.velocity.size() == 0) {
      out << "velocity: []\n";
    } else {
      out << "velocity:\n";
      for (auto item : msg.velocity) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: is_exc
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_exc: ";
    rosidl_generator_traits::value_to_yaml(msg.is_exc, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const JointStateCommand & msg, bool use_flow_style = false)
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
  const robot_control_ros2::msg::JointStateCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  robot_control_ros2::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use robot_control_ros2::msg::to_yaml() instead")]]
inline std::string to_yaml(const robot_control_ros2::msg::JointStateCommand & msg)
{
  return robot_control_ros2::msg::to_yaml(msg);
}

template<>
inline const char * data_type<robot_control_ros2::msg::JointStateCommand>()
{
  return "robot_control_ros2::msg::JointStateCommand";
}

template<>
inline const char * name<robot_control_ros2::msg::JointStateCommand>()
{
  return "robot_control_ros2/msg/JointStateCommand";
}

template<>
struct has_fixed_size<robot_control_ros2::msg::JointStateCommand>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<robot_control_ros2::msg::JointStateCommand>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<robot_control_ros2::msg::JointStateCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__TRAITS_HPP_
