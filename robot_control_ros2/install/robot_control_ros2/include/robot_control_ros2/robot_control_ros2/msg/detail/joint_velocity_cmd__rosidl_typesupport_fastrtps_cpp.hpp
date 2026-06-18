// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from robot_control_ros2:msg/JointVelocityCmd.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include <cstddef>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "robot_control_ros2/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "robot_control_ros2/msg/detail/joint_velocity_cmd__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace robot_control_ros2
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
cdr_serialize(
  const robot_control_ros2::msg::JointVelocityCmd & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  robot_control_ros2::msg::JointVelocityCmd & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
get_serialized_size(
  const robot_control_ros2::msg::JointVelocityCmd & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
max_serialized_size_JointVelocityCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
cdr_serialize_key(
  const robot_control_ros2::msg::JointVelocityCmd & ros_message,
  eprosima::fastcdr::Cdr &);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
get_serialized_size_key(
  const robot_control_ros2::msg::JointVelocityCmd & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
max_serialized_size_key_JointVelocityCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace robot_control_ros2

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, robot_control_ros2, msg, JointVelocityCmd)();

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
