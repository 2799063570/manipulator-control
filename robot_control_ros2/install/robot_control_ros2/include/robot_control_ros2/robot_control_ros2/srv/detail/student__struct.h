// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_control_ros2:srv/Student.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_control_ros2/srv/student.h"


#ifndef ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__STRUCT_H_
#define ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'name'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/Student in the package robot_control_ros2.
typedef struct robot_control_ros2__srv__Student_Request
{
  rosidl_runtime_c__String name;
  int32_t num1;
  int32_t num2;
} robot_control_ros2__srv__Student_Request;

// Struct for a sequence of robot_control_ros2__srv__Student_Request.
typedef struct robot_control_ros2__srv__Student_Request__Sequence
{
  robot_control_ros2__srv__Student_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_ros2__srv__Student_Request__Sequence;

// Constants defined in the message

/// Struct defined in srv/Student in the package robot_control_ros2.
typedef struct robot_control_ros2__srv__Student_Response
{
  int32_t num;
  bool is_ok;
} robot_control_ros2__srv__Student_Response;

// Struct for a sequence of robot_control_ros2__srv__Student_Response.
typedef struct robot_control_ros2__srv__Student_Response__Sequence
{
  robot_control_ros2__srv__Student_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_ros2__srv__Student_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  robot_control_ros2__srv__Student_Event__request__MAX_SIZE = 1
};
// response
enum
{
  robot_control_ros2__srv__Student_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/Student in the package robot_control_ros2.
typedef struct robot_control_ros2__srv__Student_Event
{
  service_msgs__msg__ServiceEventInfo info;
  robot_control_ros2__srv__Student_Request__Sequence request;
  robot_control_ros2__srv__Student_Response__Sequence response;
} robot_control_ros2__srv__Student_Event;

// Struct for a sequence of robot_control_ros2__srv__Student_Event.
typedef struct robot_control_ros2__srv__Student_Event__Sequence
{
  robot_control_ros2__srv__Student_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_control_ros2__srv__Student_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_CONTROL_ROS2__SRV__DETAIL__STUDENT__STRUCT_H_
