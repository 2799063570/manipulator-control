// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from robot_control_ros2:msg/JointStateCommand.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "robot_control_ros2/msg/detail/joint_state_command__rosidl_typesupport_introspection_c.h"
#include "robot_control_ros2/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "robot_control_ros2/msg/detail/joint_state_command__functions.h"
#include "robot_control_ros2/msg/detail/joint_state_command__struct.h"


// Include directives for member types
// Member `position`
// Member `velocity`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  robot_control_ros2__msg__JointStateCommand__init(message_memory);
}

void robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_fini_function(void * message_memory)
{
  robot_control_ros2__msg__JointStateCommand__fini(message_memory);
}

size_t robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__size_function__JointStateCommand__position(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_const_function__JointStateCommand__position(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_function__JointStateCommand__position(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__fetch_function__JointStateCommand__position(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_const_function__JointStateCommand__position(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__assign_function__JointStateCommand__position(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_function__JointStateCommand__position(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__resize_function__JointStateCommand__position(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

size_t robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__size_function__JointStateCommand__velocity(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_const_function__JointStateCommand__velocity(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_function__JointStateCommand__velocity(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__fetch_function__JointStateCommand__velocity(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_const_function__JointStateCommand__velocity(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__assign_function__JointStateCommand__velocity(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_function__JointStateCommand__velocity(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__resize_function__JointStateCommand__velocity(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_message_member_array[3] = {
  {
    "position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robot_control_ros2__msg__JointStateCommand, position),  // bytes offset in struct
    NULL,  // default value
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__size_function__JointStateCommand__position,  // size() function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_const_function__JointStateCommand__position,  // get_const(index) function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_function__JointStateCommand__position,  // get(index) function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__fetch_function__JointStateCommand__position,  // fetch(index, &value) function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__assign_function__JointStateCommand__position,  // assign(index, value) function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__resize_function__JointStateCommand__position  // resize(index) function pointer
  },
  {
    "velocity",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robot_control_ros2__msg__JointStateCommand, velocity),  // bytes offset in struct
    NULL,  // default value
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__size_function__JointStateCommand__velocity,  // size() function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_const_function__JointStateCommand__velocity,  // get_const(index) function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__get_function__JointStateCommand__velocity,  // get(index) function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__fetch_function__JointStateCommand__velocity,  // fetch(index, &value) function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__assign_function__JointStateCommand__velocity,  // assign(index, value) function pointer
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__resize_function__JointStateCommand__velocity  // resize(index) function pointer
  },
  {
    "is_exc",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robot_control_ros2__msg__JointStateCommand, is_exc),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_message_members = {
  "robot_control_ros2__msg",  // message namespace
  "JointStateCommand",  // message name
  3,  // number of fields
  sizeof(robot_control_ros2__msg__JointStateCommand),
  false,  // has_any_key_member_
  robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_message_member_array,  // message members
  robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_init_function,  // function to initialize message memory (memory has to be allocated)
  robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_message_type_support_handle = {
  0,
  &robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_message_members,
  get_message_typesupport_handle_function,
  &robot_control_ros2__msg__JointStateCommand__get_type_hash,
  &robot_control_ros2__msg__JointStateCommand__get_type_description,
  &robot_control_ros2__msg__JointStateCommand__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_robot_control_ros2
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, robot_control_ros2, msg, JointStateCommand)() {
  if (!robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_message_type_support_handle.typesupport_identifier) {
    robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &robot_control_ros2__msg__JointStateCommand__rosidl_typesupport_introspection_c__JointStateCommand_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
