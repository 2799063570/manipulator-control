// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_control_ros2:msg/JointStateCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_state_command.hpp"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__BUILDER_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_control_ros2/msg/detail/joint_state_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_control_ros2
{

namespace msg
{

namespace builder
{

class Init_JointStateCommand_is_exc
{
public:
  explicit Init_JointStateCommand_is_exc(::robot_control_ros2::msg::JointStateCommand & msg)
  : msg_(msg)
  {}
  ::robot_control_ros2::msg::JointStateCommand is_exc(::robot_control_ros2::msg::JointStateCommand::_is_exc_type arg)
  {
    msg_.is_exc = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_ros2::msg::JointStateCommand msg_;
};

class Init_JointStateCommand_velocity
{
public:
  explicit Init_JointStateCommand_velocity(::robot_control_ros2::msg::JointStateCommand & msg)
  : msg_(msg)
  {}
  Init_JointStateCommand_is_exc velocity(::robot_control_ros2::msg::JointStateCommand::_velocity_type arg)
  {
    msg_.velocity = std::move(arg);
    return Init_JointStateCommand_is_exc(msg_);
  }

private:
  ::robot_control_ros2::msg::JointStateCommand msg_;
};

class Init_JointStateCommand_position
{
public:
  Init_JointStateCommand_position()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_JointStateCommand_velocity position(::robot_control_ros2::msg::JointStateCommand::_position_type arg)
  {
    msg_.position = std::move(arg);
    return Init_JointStateCommand_velocity(msg_);
  }

private:
  ::robot_control_ros2::msg::JointStateCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_ros2::msg::JointStateCommand>()
{
  return robot_control_ros2::msg::builder::Init_JointStateCommand_position();
}

}  // namespace robot_control_ros2

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_STATE_COMMAND__BUILDER_HPP_
