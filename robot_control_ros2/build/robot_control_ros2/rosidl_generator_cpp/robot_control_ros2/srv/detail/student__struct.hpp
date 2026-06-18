// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from robot_control_ros2:srv/Student.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/srv/student.hpp"


#ifndef ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__STRUCT_HPP_
#define ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__robot_control_ros2__srv__Student_Request __attribute__((deprecated))
#else
# define DEPRECATED__robot_control_ros2__srv__Student_Request __declspec(deprecated)
#endif

namespace robot_control_ros2
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Student_Request_
{
  using Type = Student_Request_<ContainerAllocator>;

  explicit Student_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->name = "";
      this->num1 = 0l;
      this->num2 = 0l;
    }
  }

  explicit Student_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : name(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->name = "";
      this->num1 = 0l;
      this->num2 = 0l;
    }
  }

  // field types and members
  using _name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _name_type name;
  using _num1_type =
    int32_t;
  _num1_type num1;
  using _num2_type =
    int32_t;
  _num2_type num2;

  // setters for named parameter idiom
  Type & set__name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->name = _arg;
    return *this;
  }
  Type & set__num1(
    const int32_t & _arg)
  {
    this->num1 = _arg;
    return *this;
  }
  Type & set__num2(
    const int32_t & _arg)
  {
    this->num2 = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_control_ros2::srv::Student_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_control_ros2::srv::Student_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_control_ros2::srv::Student_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_control_ros2::srv::Student_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::srv::Student_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::srv::Student_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::srv::Student_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::srv::Student_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_control_ros2::srv::Student_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_control_ros2::srv::Student_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_control_ros2__srv__Student_Request
    std::shared_ptr<robot_control_ros2::srv::Student_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_control_ros2__srv__Student_Request
    std::shared_ptr<robot_control_ros2::srv::Student_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Student_Request_ & other) const
  {
    if (this->name != other.name) {
      return false;
    }
    if (this->num1 != other.num1) {
      return false;
    }
    if (this->num2 != other.num2) {
      return false;
    }
    return true;
  }
  bool operator!=(const Student_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Student_Request_

// alias to use template instance with default allocator
using Student_Request =
  robot_control_ros2::srv::Student_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace robot_control_ros2


#ifndef _WIN32
# define DEPRECATED__robot_control_ros2__srv__Student_Response __attribute__((deprecated))
#else
# define DEPRECATED__robot_control_ros2__srv__Student_Response __declspec(deprecated)
#endif

namespace robot_control_ros2
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Student_Response_
{
  using Type = Student_Response_<ContainerAllocator>;

  explicit Student_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->num = 0l;
      this->is_ok = false;
    }
  }

  explicit Student_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->num = 0l;
      this->is_ok = false;
    }
  }

  // field types and members
  using _num_type =
    int32_t;
  _num_type num;
  using _is_ok_type =
    bool;
  _is_ok_type is_ok;

  // setters for named parameter idiom
  Type & set__num(
    const int32_t & _arg)
  {
    this->num = _arg;
    return *this;
  }
  Type & set__is_ok(
    const bool & _arg)
  {
    this->is_ok = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_control_ros2::srv::Student_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_control_ros2::srv::Student_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_control_ros2::srv::Student_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_control_ros2::srv::Student_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::srv::Student_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::srv::Student_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::srv::Student_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::srv::Student_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_control_ros2::srv::Student_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_control_ros2::srv::Student_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_control_ros2__srv__Student_Response
    std::shared_ptr<robot_control_ros2::srv::Student_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_control_ros2__srv__Student_Response
    std::shared_ptr<robot_control_ros2::srv::Student_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Student_Response_ & other) const
  {
    if (this->num != other.num) {
      return false;
    }
    if (this->is_ok != other.is_ok) {
      return false;
    }
    return true;
  }
  bool operator!=(const Student_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Student_Response_

// alias to use template instance with default allocator
using Student_Response =
  robot_control_ros2::srv::Student_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace robot_control_ros2


// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__robot_control_ros2__srv__Student_Event __attribute__((deprecated))
#else
# define DEPRECATED__robot_control_ros2__srv__Student_Event __declspec(deprecated)
#endif

namespace robot_control_ros2
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Student_Event_
{
  using Type = Student_Event_<ContainerAllocator>;

  explicit Student_Event_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : info(_init)
  {
    (void)_init;
  }

  explicit Student_Event_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : info(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _info_type =
    service_msgs::msg::ServiceEventInfo_<ContainerAllocator>;
  _info_type info;
  using _request_type =
    rosidl_runtime_cpp::BoundedVector<robot_control_ros2::srv::Student_Request_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<robot_control_ros2::srv::Student_Request_<ContainerAllocator>>>;
  _request_type request;
  using _response_type =
    rosidl_runtime_cpp::BoundedVector<robot_control_ros2::srv::Student_Response_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<robot_control_ros2::srv::Student_Response_<ContainerAllocator>>>;
  _response_type response;

  // setters for named parameter idiom
  Type & set__info(
    const service_msgs::msg::ServiceEventInfo_<ContainerAllocator> & _arg)
  {
    this->info = _arg;
    return *this;
  }
  Type & set__request(
    const rosidl_runtime_cpp::BoundedVector<robot_control_ros2::srv::Student_Request_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<robot_control_ros2::srv::Student_Request_<ContainerAllocator>>> & _arg)
  {
    this->request = _arg;
    return *this;
  }
  Type & set__response(
    const rosidl_runtime_cpp::BoundedVector<robot_control_ros2::srv::Student_Response_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<robot_control_ros2::srv::Student_Response_<ContainerAllocator>>> & _arg)
  {
    this->response = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_control_ros2::srv::Student_Event_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_control_ros2::srv::Student_Event_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_control_ros2::srv::Student_Event_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_control_ros2::srv::Student_Event_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::srv::Student_Event_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::srv::Student_Event_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_control_ros2::srv::Student_Event_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_control_ros2::srv::Student_Event_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_control_ros2::srv::Student_Event_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_control_ros2::srv::Student_Event_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_control_ros2__srv__Student_Event
    std::shared_ptr<robot_control_ros2::srv::Student_Event_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_control_ros2__srv__Student_Event
    std::shared_ptr<robot_control_ros2::srv::Student_Event_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Student_Event_ & other) const
  {
    if (this->info != other.info) {
      return false;
    }
    if (this->request != other.request) {
      return false;
    }
    if (this->response != other.response) {
      return false;
    }
    return true;
  }
  bool operator!=(const Student_Event_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Student_Event_

// alias to use template instance with default allocator
using Student_Event =
  robot_control_ros2::srv::Student_Event_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace robot_control_ros2

namespace robot_control_ros2
{

namespace srv
{

struct Student
{
  using Request = robot_control_ros2::srv::Student_Request;
  using Response = robot_control_ros2::srv::Student_Response;
  using Event = robot_control_ros2::srv::Student_Event;
};

}  // namespace srv

}  // namespace robot_control_ros2

#endif  // ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__STRUCT_HPP_
