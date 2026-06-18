// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_control_ros2:msg/JointPositionCmd.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/msg/joint_position_cmd.hpp"


#ifndef ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__BUILDER_HPP_
#define ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_control_ros2/msg/detail/joint_position_cmd__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_control_ros2
{

namespace msg
{

namespace builder
{

class Init_JointPositionCmd_position_cmd
{
public:
  Init_JointPositionCmd_position_cmd()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::robot_control_ros2::msg::JointPositionCmd position_cmd(::robot_control_ros2::msg::JointPositionCmd::_position_cmd_type arg)
  {
    msg_.position_cmd = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_ros2::msg::JointPositionCmd msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_ros2::msg::JointPositionCmd>()
{
  return robot_control_ros2::msg::builder::Init_JointPositionCmd_position_cmd();
}

}  // namespace robot_control_ros2

#endif  // ROBOT_CONTROL_ROS2__MSG__DETAIL__JOINT_POSITION_CMD__BUILDER_HPP_
