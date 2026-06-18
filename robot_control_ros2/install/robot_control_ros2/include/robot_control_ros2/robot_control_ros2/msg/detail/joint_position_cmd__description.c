// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from robot_control_ros2:msg/JointPositionCmd.idl
// generated code does not contain a copyright notice

#include "robot_control_ros2/msg/detail/joint_position_cmd__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_robot_control_ros2
const rosidl_type_hash_t *
robot_control_ros2__msg__JointPositionCmd__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x01, 0x9c, 0xc4, 0xf1, 0xd4, 0x5b, 0xce, 0x97,
      0x56, 0x06, 0x8a, 0x2e, 0x93, 0xf6, 0x99, 0xf9,
      0x51, 0x8a, 0x99, 0xa4, 0x83, 0xab, 0xc4, 0xf9,
      0x59, 0xc1, 0x0e, 0x06, 0x06, 0xb7, 0xf8, 0x36,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char robot_control_ros2__msg__JointPositionCmd__TYPE_NAME[] = "robot_control_ros2/msg/JointPositionCmd";

// Define type names, field names, and default values
static char robot_control_ros2__msg__JointPositionCmd__FIELD_NAME__position_cmd[] = "position_cmd";

static rosidl_runtime_c__type_description__Field robot_control_ros2__msg__JointPositionCmd__FIELDS[] = {
  {
    {robot_control_ros2__msg__JointPositionCmd__FIELD_NAME__position_cmd, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT_ARRAY,
      6,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
robot_control_ros2__msg__JointPositionCmd__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {robot_control_ros2__msg__JointPositionCmd__TYPE_NAME, 39, 39},
      {robot_control_ros2__msg__JointPositionCmd__FIELDS, 1, 1},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "float32[6] position_cmd";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
robot_control_ros2__msg__JointPositionCmd__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {robot_control_ros2__msg__JointPositionCmd__TYPE_NAME, 39, 39},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 23, 23},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
robot_control_ros2__msg__JointPositionCmd__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *robot_control_ros2__msg__JointPositionCmd__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
