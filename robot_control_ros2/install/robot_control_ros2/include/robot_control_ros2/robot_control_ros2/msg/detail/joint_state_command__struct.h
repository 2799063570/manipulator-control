// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_control_ros2:msg/JointStateCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_state_command.h"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__STRUCT_H_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'position'
// Member 'velocity'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/JointStateCommand in the package robot_control_ros2.
typedef struct robot_control_ros2__msg__JointStateCommand
{
  rosidl_runtime_c__double__Sequence position;
  rosidl_runtime_c__double__Sequence velocity;
  bool is_exc;
} robot_control_ros2__msg__JointStateCommand;

// Struct for a sequence of robot_control_ros2__msg__JointStateCommand.
typedef struct robot_control_ros2__msg__JointStateCommand__Sequence
{
  robot_control_ros2__msg__JointStateCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_ros2__msg__JointStateCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__STRUCT_H_
