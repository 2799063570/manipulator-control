// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from robot_control_ros2:msg/JointPositionCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_position_cmd.hpp"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__STRUCT_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__robot_control_ros2__msg__JointPositionCmd __attribute__((deprecated))
#else
# define DEPRECATED__robot_control_ros2__msg__JointPositionCmd __declspec(deprecated)
#endif

namespace robot_control_ros2
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct JointPositionCmd_
{
  using Type = JointPositionCmd_<ContainerAllocator>;

  explicit JointPositionCmd_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<float, 6>::iterator, float>(this->position_cmd.begin(), this->position_cmd.end(), 0.0f);
    }
  }

  explicit JointPositionCmd_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : position_cmd(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<float, 6>::iterator, float>(this->position_cmd.begin(), this->position_cmd.end(), 0.0f);
    }
  }

  // field types and members
  using _position_cmd_type =
    std::array<float, 6>;
  _position_cmd_type position_cmd;

  // setters for named parameter idiom
  Type & set__position_cmd(
    const std::array<float, 6> & _arg)
  {
    this->position_cmd = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_control_ros2__msg__JointPositionCmd
    std::shared_ptr<robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_control_ros2__msg__JointPositionCmd
    std::shared_ptr<robot_control_ros2::msg::JointPositionCmd_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const JointPositionCmd_ & other) const
  {
    if (this->position_cmd != other.position_cmd) {
      return false;
    }
    return true;
  }
  bool operator!=(const JointPositionCmd_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct JointPositionCmd_

// alias to use template instance with default allocator
using JointPositionCmd =
  robot_control_ros2::msg::JointPositionCmd_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace robot_control_ros2

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__STRUCT_HPP_
