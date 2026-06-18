// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from robot_control_ros2:msg/JointStateCommand.idl
// generated code does not contain a copyright notice

#include "robot_control_ros2/msg/detail/joint_state_command__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_robot_control_ros2
const rosidl_type_hash_t *
robot_control_ros2__msg__JointStateCommand__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x78, 0x4a, 0xfb, 0xd1, 0x3c, 0x8c, 0x08, 0xdf,
      0x6c, 0x3c, 0xfb, 0x49, 0xf8, 0x7a, 0x3f, 0x1c,
      0x67, 0x1d, 0x1e, 0xeb, 0x76, 0xe0, 0xba, 0x5e,
      0xcc, 0xd9, 0x5d, 0x67, 0x80, 0xa8, 0x4c, 0x8e,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char robot_control_ros2__msg__JointStateCommand__TYPE_NAME[] = "robot_control_ros2/msg/JointStateCommand";

// Define type names, field names, and default values
static char robot_control_ros2__msg__JointStateCommand__FIELD_NAME__position[] = "position";
static char robot_control_ros2__msg__JointStateCommand__FIELD_NAME__velocity[] = "velocity";
static char robot_control_ros2__msg__JointStateCommand__FIELD_NAME__is_exc[] = "is_exc";

static rosidl_runtime_c__type_description__Field robot_control_ros2__msg__JointStateCommand__FIELDS[] = {
  {
    {robot_control_ros2__msg__JointStateCommand__FIELD_NAME__position, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__msg__JointStateCommand__FIELD_NAME__velocity, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__msg__JointStateCommand__FIELD_NAME__is_exc, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
robot_control_ros2__msg__JointStateCommand__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {robot_control_ros2__msg__JointStateCommand__TYPE_NAME, 40, 40},
      {robot_control_ros2__msg__JointStateCommand__FIELDS, 3, 3},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "float64[] position\n"
  "float64[] velocity\n"
  "bool is_exc";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
robot_control_ros2__msg__JointStateCommand__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {robot_control_ros2__msg__JointStateCommand__TYPE_NAME, 40, 40},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 49, 49},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
robot_control_ros2__msg__JointStateCommand__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *robot_control_ros2__msg__JointStateCommand__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
