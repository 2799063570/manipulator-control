// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from robot_control_ros2:msg/JointVelocityCmd.idl
// generated code does not contain a copyright notice
#include "robot_control_ros2/msg/detail/joint_velocity_cmd__rosidl_typesupport_fastrtps_cpp.hpp"
#include "robot_control_ros2/msg/detail/joint_velocity_cmd__functions.h"
#include "robot_control_ros2/msg/detail/joint_velocity_cmd__struct.hpp"

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/serialization_helpers.hpp"
#include "rosidl_typesupport_fastrtps_cpp/wstring_conversion.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

namespace robot_control_ros2
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{


bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
cdr_serialize(
  const robot_control_ros2::msg::JointVelocityCmd & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: velocity_cmd
  {
    cdr << ros_message.velocity_cmd;
  }

  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  robot_control_ros2::msg::JointVelocityCmd & ros_message)
{
  // Member: velocity_cmd
  {
    cdr >> ros_message.velocity_cmd;
  }

  return true;
}  // NOLINT(readability/fn_size)


size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
get_serialized_size(
  const robot_control_ros2::msg::JointVelocityCmd & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: velocity_cmd
  {
    size_t array_size = 6;
    size_t item_size = sizeof(ros_message.velocity_cmd[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
max_serialized_size_JointVelocityCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Member: velocity_cmd
  {
    size_t array_size = 6;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = robot_control_ros2::msg::JointVelocityCmd;
    is_plain =
      (
      offsetof(DataType, velocity_cmd) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
cdr_serialize_key(
  const robot_control_ros2::msg::JointVelocityCmd & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: velocity_cmd
  {
    cdr << ros_message.velocity_cmd;
  }

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
get_serialized_size_key(
  const robot_control_ros2::msg::JointVelocityCmd & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: velocity_cmd
  {
    size_t array_size = 6;
    size_t item_size = sizeof(ros_message.velocity_cmd[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_robot_control_ros2
max_serialized_size_key_JointVelocityCmd(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Member: velocity_cmd
  {
    size_t array_size = 6;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = robot_control_ros2::msg::JointVelocityCmd;
    is_plain =
      (
      offsetof(DataType, velocity_cmd) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}


static bool _JointVelocityCmd__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const robot_control_ros2::msg::JointVelocityCmd *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _JointVelocityCmd__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<robot_control_ros2::msg::JointVelocityCmd *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _JointVelocityCmd__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const robot_control_ros2::msg::JointVelocityCmd *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _JointVelocityCmd__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_JointVelocityCmd(full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}

static message_type_support_callbacks_t _JointVelocityCmd__callbacks = {
  "robot_control_ros2::msg",
  "JointVelocityCmd",
  _JointVelocityCmd__cdr_serialize,
  _JointVelocityCmd__cdr_deserialize,
  _JointVelocityCmd__get_serialized_size,
  _JointVelocityCmd__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _JointVelocityCmd__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_JointVelocityCmd__callbacks,
  get_message_typesupport_handle_function,
  &robot_control_ros2__msg__JointVelocityCmd__get_type_hash,
  &robot_control_ros2__msg__JointVelocityCmd__get_type_description,
  &robot_control_ros2__msg__JointVelocityCmd__get_type_description_sources,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace robot_control_ros2

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_robot_control_ros2
const rosidl_message_type_support_t *
get_message_type_support_handle<robot_control_ros2::msg::JointVelocityCmd>()
{
  return &robot_control_ros2::msg::typesupport_fastrtps_cpp::_JointVelocityCmd__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, robot_control_ros2, msg, JointVelocityCmd)() {
  return &robot_control_ros2::msg::typesupport_fastrtps_cpp::_JointVelocityCmd__handle;
}

#ifdef __cplusplus
}
#endif
