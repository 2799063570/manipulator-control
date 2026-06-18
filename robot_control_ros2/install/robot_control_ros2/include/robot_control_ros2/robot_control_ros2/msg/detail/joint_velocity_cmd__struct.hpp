// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from robot_control_ros2:msg/JointVelocityCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_velocity_cmd.hpp"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__STRUCT_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__robot_control_ros2__msg__JointVelocityCmd __attribute__((deprecated))
#else
# define DEPRECATED__robot_control_ros2__msg__JointVelocityCmd __declspec(deprecated)
#endif

namespace robot_control_ros2
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct JointVelocityCmd_
{
  using Type = JointVelocityCmd_<ContainerAllocator>;

  explicit JointVelocityCmd_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<float, 6>::iterator, float>(this->velocity_cmd.begin(), this->velocity_cmd.end(), 0.0f);
    }
  }

  explicit JointVelocityCmd_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : velocity_cmd(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<float, 6>::iterator, float>(this->velocity_cmd.begin(), this->velocity_cmd.end(), 0.0f);
    }
  }

  // field types and members
  using _velocity_cmd_type =
    std::array<float, 6>;
  _velocity_cmd_type velocity_cmd;

  // setters for named parameter idiom
  Type & set__velocity_cmd(
    const std::array<float, 6> & _arg)
  {
    this->velocity_cmd = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_control_ros2__msg__JointVelocityCmd
    std::shared_ptr<robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_control_ros2__msg__JointVelocityCmd
    std::shared_ptr<robot_control_ros2::msg::JointVelocityCmd_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const JointVelocityCmd_ & other) const
  {
    if (this->velocity_cmd != other.velocity_cmd) {
      return false;
    }
    return true;
  }
  bool operator!=(const JointVelocityCmd_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct JointVelocityCmd_

// alias to use template instance with default allocator
using JointVelocityCmd =
  robot_control_ros2::msg::JointVelocityCmd_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace robot_control_ros2

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_VELOCITY_CMD__STRUCT_HPP_
