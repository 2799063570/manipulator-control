// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from robot_control_ros2:msg/JointStateCommand.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "robot_control_ros2/msg/detail/joint_state_command__functions.h"
#include "robot_control_ros2/msg/detail/joint_state_command__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace robot_control_ros2
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void JointStateCommand_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) robot_control_ros2::msg::JointStateCommand(_init);
}

void JointStateCommand_fini_function(void * message_memory)
{
  auto typed_message = static_cast<robot_control_ros2::msg::JointStateCommand *>(message_memory);
  typed_message->~JointStateCommand();
}

size_t size_function__JointStateCommand__position(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__JointStateCommand__position(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__JointStateCommand__position(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void fetch_function__JointStateCommand__position(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__JointStateCommand__position(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__JointStateCommand__position(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__JointStateCommand__position(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

void resize_function__JointStateCommand__position(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

size_t size_function__JointStateCommand__velocity(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__JointStateCommand__velocity(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__JointStateCommand__velocity(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void fetch_function__JointStateCommand__velocity(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const double *>(
    get_const_function__JointStateCommand__velocity(untyped_member, index));
  auto & value = *reinterpret_cast<double *>(untyped_value);
  value = item;
}

void assign_function__JointStateCommand__velocity(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<double *>(
    get_function__JointStateCommand__velocity(untyped_member, index));
  const auto & value = *reinterpret_cast<const double *>(untyped_value);
  item = value;
}

void resize_function__JointStateCommand__velocity(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember JointStateCommand_message_member_array[3] = {
  {
    "position",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robot_control_ros2::msg::JointStateCommand, position),  // bytes offset in struct
    nullptr,  // default value
    size_function__JointStateCommand__position,  // size() function pointer
    get_const_function__JointStateCommand__position,  // get_const(index) function pointer
    get_function__JointStateCommand__position,  // get(index) function pointer
    fetch_function__JointStateCommand__position,  // fetch(index, &value) function pointer
    assign_function__JointStateCommand__position,  // assign(index, value) function pointer
    resize_function__JointStateCommand__position  // resize(index) function pointer
  },
  {
    "velocity",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robot_control_ros2::msg::JointStateCommand, velocity),  // bytes offset in struct
    nullptr,  // default value
    size_function__JointStateCommand__velocity,  // size() function pointer
    get_const_function__JointStateCommand__velocity,  // get_const(index) function pointer
    get_function__JointStateCommand__velocity,  // get(index) function pointer
    fetch_function__JointStateCommand__velocity,  // fetch(index, &value) function pointer
    assign_function__JointStateCommand__velocity,  // assign(index, value) function pointer
    resize_function__JointStateCommand__velocity  // resize(index) function pointer
  },
  {
    "is_exc",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robot_control_ros2::msg::JointStateCommand, is_exc),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers JointStateCommand_message_members = {
  "robot_control_ros2::msg",  // message namespace
  "JointStateCommand",  // message name
  3,  // number of fields
  sizeof(robot_control_ros2::msg::JointStateCommand),
  false,  // has_any_key_member_
  JointStateCommand_message_member_array,  // message members
  JointStateCommand_init_function,  // function to initialize message memory (memory has to be allocated)
  JointStateCommand_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t JointStateCommand_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &JointStateCommand_message_members,
  get_message_typesupport_handle_function,
  &robot_control_ros2__msg__JointStateCommand__get_type_hash,
  &robot_control_ros2__msg__JointStateCommand__get_type_description,
  &robot_control_ros2__msg__JointStateCommand__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace robot_control_ros2


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<robot_control_ros2::msg::JointStateCommand>()
{
  return &::robot_control_ros2::msg::rosidl_typesupport_introspection_cpp::JointStateCommand_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, robot_control_ros2, msg, JointStateCommand)() {
  return &::robot_control_ros2::msg::rosidl_typesupport_introspection_cpp::JointStateCommand_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
