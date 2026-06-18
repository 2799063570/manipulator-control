// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from robot_control_ros2:msg/JointPositionCmd.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "robot_control_ros2/msg/detail/joint_position_cmd__rosidl_typesupport_introspection_c.h"
#include "robot_control_ros2/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "robot_control_ros2/msg/detail/joint_position_cmd__functions.h"
#include "robot_control_ros2/msg/detail/joint_position_cmd__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  robot_control_ros2__msg__JointPositionCmd__init(message_memory);
}

void robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_fini_function(void * message_memory)
{
  robot_control_ros2__msg__JointPositionCmd__fini(message_memory);
}

size_t robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__size_function__JointPositionCmd__position_cmd(
  const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__get_const_function__JointPositionCmd__position_cmd(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__get_function__JointPositionCmd__position_cmd(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__fetch_function__JointPositionCmd__position_cmd(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__get_const_function__JointPositionCmd__position_cmd(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__assign_function__JointPositionCmd__position_cmd(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__get_function__JointPositionCmd__position_cmd(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_message_member_array[1] = {
  {
    "position_cmd",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(robot_control_ros2__msg__JointPositionCmd, position_cmd),  // bytes offset in struct
    NULL,  // default value
    robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__size_function__JointPositionCmd__position_cmd,  // size() function pointer
    robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__get_const_function__JointPositionCmd__position_cmd,  // get_const(index) function pointer
    robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__get_function__JointPositionCmd__position_cmd,  // get(index) function pointer
    robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__fetch_function__JointPositionCmd__position_cmd,  // fetch(index, &value) function pointer
    robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__assign_function__JointPositionCmd__position_cmd,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_message_members = {
  "robot_control_ros2__msg",  // message namespace
  "JointPositionCmd",  // message name
  1,  // number of fields
  sizeof(robot_control_ros2__msg__JointPositionCmd),
  false,  // has_any_key_member_
  robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_message_member_array,  // message members
  robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_message_type_support_handle = {
  0,
  &robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_message_members,
  get_message_typesupport_handle_function,
  &robot_control_ros2__msg__JointPositionCmd__get_type_hash,
  &robot_control_ros2__msg__JointPositionCmd__get_type_description,
  &robot_control_ros2__msg__JointPositionCmd__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_robot_control_ros2
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, robot_control_ros2, msg, JointPositionCmd)() {
  if (!robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_message_type_support_handle.typesupport_identifier) {
    robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &robot_control_ros2__msg__JointPositionCmd__rosidl_typesupport_introspection_c__JointPositionCmd_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
