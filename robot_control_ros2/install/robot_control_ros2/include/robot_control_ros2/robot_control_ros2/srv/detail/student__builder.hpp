// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_control_ros2:srv/Student.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/srv/student.hpp"


#ifndef ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__BUILDER_HPP_
#define ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_control_ros2/srv/detail/student__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_control_ros2
{

namespace srv
{

namespace builder
{

class Init_Student_Request_num2
{
public:
  explicit Init_Student_Request_num2(::robot_control_ros2::srv::Student_Request & msg)
  : msg_(msg)
  {}
  ::robot_control_ros2::srv::Student_Request num2(::robot_control_ros2::srv::Student_Request::_num2_type arg)
  {
    msg_.num2 = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_ros2::srv::Student_Request msg_;
};

class Init_Student_Request_num1
{
public:
  explicit Init_Student_Request_num1(::robot_control_ros2::srv::Student_Request & msg)
  : msg_(msg)
  {}
  Init_Student_Request_num2 num1(::robot_control_ros2::srv::Student_Request::_num1_type arg)
  {
    msg_.num1 = std::move(arg);
    return Init_Student_Request_num2(msg_);
  }

private:
  ::robot_control_ros2::srv::Student_Request msg_;
};

class Init_Student_Request_name
{
public:
  Init_Student_Request_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Student_Request_num1 name(::robot_control_ros2::srv::Student_Request::_name_type arg)
  {
    msg_.name = std::move(arg);
    return Init_Student_Request_num1(msg_);
  }

private:
  ::robot_control_ros2::srv::Student_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_ros2::srv::Student_Request>()
{
  return robot_control_ros2::srv::builder::Init_Student_Request_name();
}

}  // namespace robot_control_ros2


namespace robot_control_ros2
{

namespace srv
{

namespace builder
{

class Init_Student_Response_is_ok
{
public:
  explicit Init_Student_Response_is_ok(::robot_control_ros2::srv::Student_Response & msg)
  : msg_(msg)
  {}
  ::robot_control_ros2::srv::Student_Response is_ok(::robot_control_ros2::srv::Student_Response::_is_ok_type arg)
  {
    msg_.is_ok = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_ros2::srv::Student_Response msg_;
};

class Init_Student_Response_num
{
public:
  Init_Student_Response_num()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Student_Response_is_ok num(::robot_control_ros2::srv::Student_Response::_num_type arg)
  {
    msg_.num = std::move(arg);
    return Init_Student_Response_is_ok(msg_);
  }

private:
  ::robot_control_ros2::srv::Student_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_ros2::srv::Student_Response>()
{
  return robot_control_ros2::srv::builder::Init_Student_Response_num();
}

}  // namespace robot_control_ros2


namespace robot_control_ros2
{

namespace srv
{

namespace builder
{

class Init_Student_Event_response
{
public:
  explicit Init_Student_Event_response(::robot_control_ros2::srv::Student_Event & msg)
  : msg_(msg)
  {}
  ::robot_control_ros2::srv::Student_Event response(::robot_control_ros2::srv::Student_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_control_ros2::srv::Student_Event msg_;
};

class Init_Student_Event_request
{
public:
  explicit Init_Student_Event_request(::robot_control_ros2::srv::Student_Event & msg)
  : msg_(msg)
  {}
  Init_Student_Event_response request(::robot_control_ros2::srv::Student_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_Student_Event_response(msg_);
  }

private:
  ::robot_control_ros2::srv::Student_Event msg_;
};

class Init_Student_Event_info
{
public:
  Init_Student_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Student_Event_request info(::robot_control_ros2::srv::Student_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_Student_Event_request(msg_);
  }

private:
  ::robot_control_ros2::srv::Student_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_control_ros2::srv::Student_Event>()
{
  return robot_control_ros2::srv::builder::Init_Student_Event_info();
}

}  // namespace robot_control_ros2

#endif  // ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__BUILDER_HPP_
