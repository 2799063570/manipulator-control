// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_control_ros2:msg/JointPositionCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_position_cmd.h"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__STRUCT_H_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/JointPositionCmd in the package robot_control_ros2.
typedef struct robot_control_ros2__msg__JointPositionCmd
{
  float position_cmd[6];
} robot_control_ros2__msg__JointPositionCmd;

// Struct for a sequence of robot_control_ros2__msg__JointPositionCmd.
typedef struct robot_control_ros2__msg__JointPositionCmd__Sequence
{
  robot_control_ros2__msg__JointPositionCmd * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_ros2__msg__JointPositionCmd__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__STRUCT_H_
