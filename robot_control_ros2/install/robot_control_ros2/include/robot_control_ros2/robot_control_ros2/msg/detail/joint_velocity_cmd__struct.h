// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_control_ros2:msg/JointVelocityCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_velocity_cmd.h"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__STRUCT_H_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/JointVelocityCmd in the package robot_control_ros2.
typedef struct robot_control_ros2__msg__JointVelocityCmd
{
  float velocity_cmd[6];
} robot_control_ros2__msg__JointVelocityCmd;

// Struct for a sequence of robot_control_ros2__msg__JointVelocityCmd.
typedef struct robot_control_ros2__msg__JointVelocityCmd__Sequence
{
  robot_control_ros2__msg__JointVelocityCmd * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_ros2__msg__JointVelocityCmd__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__STRUCT_H_
