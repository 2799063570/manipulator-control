// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from robot_control_ros2:msg/JointVelocityCmd.idl
// generated code does not contain a copyright notice

#include "robot_control_ros2/msg/detail/joint_velocity_cmd__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_robot_control_ros2
const rosidl_type_hash_t *
robot_control_ros2__msg__JointVelocityCmd__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xfc, 0x1a, 0x19, 0xa2, 0x6d, 0x2c, 0xfa, 0xfb,
      0x91, 0xf3, 0x3a, 0x7a, 0xe6, 0xcf, 0xae, 0x11,
      0x87, 0x3a, 0xe0, 0xaa, 0xcd, 0x76, 0x47, 0xed,
      0x5a, 0x5a, 0x19, 0x7b, 0x78, 0x11, 0x92, 0x44,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char robot_control_ros2__msg__JointVelocityCmd__TYPE_NAME[] = "robot_control_ros2/msg/JointVelocityCmd";

// Define type names, field names, and default values
static char robot_control_ros2__msg__JointVelocityCmd__FIELD_NAME__velocity_cmd[] = "velocity_cmd";

static rosidl_runtime_c__type_description__Field robot_control_ros2__msg__JointVelocityCmd__FIELDS[] = {
  {
    {robot_control_ros2__msg__JointVelocityCmd__FIELD_NAME__velocity_cmd, 12, 12},
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
robot_control_ros2__msg__JointVelocityCmd__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {robot_control_ros2__msg__JointVelocityCmd__TYPE_NAME, 39, 39},
      {robot_control_ros2__msg__JointVelocityCmd__FIELDS, 1, 1},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "float32[6] velocity_cmd";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
robot_control_ros2__msg__JointVelocityCmd__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {robot_control_ros2__msg__JointVelocityCmd__TYPE_NAME, 39, 39},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 23, 23},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
robot_control_ros2__msg__JointVelocityCmd__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *robot_control_ros2__msg__JointVelocityCmd__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
