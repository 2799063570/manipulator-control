// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from robot_control_ros2:msg/JointVelocityCmd.idl
// generated code does not contain a copyright notice
#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "robot_control_ros2/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "robot_control_ros2/msg/detail/joint_velocity_cmd__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robot_control_ros2
bool cdr_serialize_robot_control_ros2__msg__JointVelocityCmd(
  const robot_control_ros2__msg__JointVelocityCmd * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robot_control_ros2
bool cdr_deserialize_robot_control_ros2__msg__JointVelocityCmd(
  eprosima::fastcdr::Cdr &,
  robot_control_ros2__msg__JointVelocityCmd * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robot_control_ros2
size_t get_serialized_size_robot_control_ros2__msg__JointVelocityCmd(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robot_control_ros2
size_t max_serialized_size_robot_control_ros2__msg__JointVelocityCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robot_control_ros2
bool cdr_serialize_key_robot_control_ros2__msg__JointVelocityCmd(
  const robot_control_ros2__msg__JointVelocityCmd * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robot_control_ros2
size_t get_serialized_size_key_robot_control_ros2__msg__JointVelocityCmd(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robot_control_ros2
size_t max_serialized_size_key_robot_control_ros2__msg__JointVelocityCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robot_control_ros2
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, robot_control_ros2, msg, JointVelocityCmd)();

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
