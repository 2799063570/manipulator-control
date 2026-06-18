// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from robot_control_ros2:msg/JointStateCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_state_command.hpp"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__STRUCT_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__robot_control_ros2__msg__JointStateCommand __attribute__((deprecated))
#else
# define DEPRECATED__robot_control_ros2__msg__JointStateCommand __declspec(deprecated)
#endif

namespace robot_control_ros2
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct JointStateCommand_
{
  using Type = JointStateCommand_<ContainerAllocator>;

  explicit JointStateCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->is_exc = false;
    }
  }

  explicit JointStateCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->is_exc = false;
    }
  }

  // field types and members
  using _position_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _position_type position;
  using _velocity_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _velocity_type velocity;
  using _is_exc_type =
    bool;
  _is_exc_type is_exc;

  // setters for named parameter idiom
  Type & set__position(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->position = _arg;
    return *this;
  }
  Type & set__velocity(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->velocity = _arg;
    return *this;
  }
  Type & set__is_exc(
    const bool & _arg)
  {
    this->is_exc = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_control_ros2::msg::JointStateCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_control_ros2::msg::JointStateCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_control_ros2::msg::JointStateCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_control_ros2::msg::JointStateCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::msg::JointStateCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::msg::JointStateCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::msg::JointStateCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::msg::JointStateCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_control_ros2::msg::JointStateCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_control_ros2::msg::JointStateCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_control_ros2__msg__JointStateCommand
    std::shared_ptr<robot_control_ros2::msg::JointStateCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_control_ros2__msg__JointStateCommand
    std::shared_ptr<robot_control_ros2::msg::JointStateCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const JointStateCommand_ & other) const
  {
    if (this->position != other.position) {
      return false;
    }
    if (this->velocity != other.velocity) {
      return false;
    }
    if (this->is_exc != other.is_exc) {
      return false;
    }
    return true;
  }
  bool operator!=(const JointStateCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct JointStateCommand_

// alias to use template instance with default allocator
using JointStateCommand =
  robot_control_ros2::msg::JointStateCommand_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace robot_control_ros2

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__STRUCT_HPP_
