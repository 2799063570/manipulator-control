// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from robot_control_ros2:msg/JointPositionCmd.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "robot_control_ros2/msg/detail/joint_position_cmd__functions.h"
#include "robot_control_ros2/msg/detail/joint_position_cmd__struct.hpp"
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

void JointPositionCmd_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) robot_control_ros2::msg::JointPositionCmd(_init);
}

void JointPositionCmd_fini_function(void * message_memory)
{
  auto typed_message = static_cast<robot_control_ros2::msg::JointPositionCmd *>(message_memory);
  typed_message->~JointPositionCmd();
}

size_t size_function__JointPositionCmd__position_cmd(const void * untyped_member)
{
  (void)untyped_member;
  return 6;
}

const void * get_const_function__JointPositionCmd__position_cmd(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<float, 6> *>(untyped_member);
  return &member[index];
}

void * get_function__JointPositionCmd__position_cmd(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<float, 6> *>(untyped_member);
  return &member[index];
}

void fetch_function__JointPositionCmd__position_cmd(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__JointPositionCmd__position_cmd(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__JointPositionCmd__position_cmd(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__JointPositionCmd__position_cmd(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember JointPositionCmd_message_member_array[1] = {
  {
    "position_cmd",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    6,  // array size
    false,  // is upper bound
    offsetof(robot_control_ros2::msg::JointPositionCmd, position_cmd),  // bytes offset in struct
    nullptr,  // default value
    size_function__JointPositionCmd__position_cmd,  // size() function pointer
    get_const_function__JointPositionCmd__position_cmd,  // get_const(index) function pointer
    get_function__JointPositionCmd__position_cmd,  // get(index) function pointer
    fetch_function__JointPositionCmd__position_cmd,  // fetch(index, &value) function pointer
    assign_function__JointPositionCmd__position_cmd,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers JointPositionCmd_message_members = {
  "robot_control_ros2::msg",  // message namespace
  "JointPositionCmd",  // message name
  1,  // number of fields
  sizeof(robot_control_ros2::msg::JointPositionCmd),
  false,  // has_any_key_member_
  JointPositionCmd_message_member_array,  // message members
  JointPositionCmd_init_function,  // function to initialize message memory (memory has to be allocated)
  JointPositionCmd_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t JointPositionCmd_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &JointPositionCmd_message_members,
  get_message_typesupport_handle_function,
  &robot_control_ros2__msg__JointPositionCmd__get_type_hash,
  &robot_control_ros2__msg__JointPositionCmd__get_type_description,
  &robot_control_ros2__msg__JointPositionCmd__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace robot_control_ros2


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<robot_control_ros2::msg::JointPositionCmd>()
{
  return &::robot_control_ros2::msg::rosidl_typesupport_introspection_cpp::JointPositionCmd_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, robot_control_ros2, msg, JointPositionCmd)() {
  return &::robot_control_ros2::msg::rosidl_typesupport_introspection_cpp::JointPositionCmd_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
