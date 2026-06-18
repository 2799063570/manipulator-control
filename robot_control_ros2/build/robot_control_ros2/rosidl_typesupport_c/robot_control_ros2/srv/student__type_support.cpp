// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from robot_control_ros2:srv/Student.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "robot_control_ros2/srv/detail/student__struct.h"
#include "robot_control_ros2/srv/detail/student__type_support.h"
#include "robot_control_ros2/srv/detail/student__functions.h"
#include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/message_type_support_dispatch.h"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_c/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace robot_control_ros2
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _Student_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _Student_Request_type_support_ids_t;

static const _Student_Request_type_support_ids_t _Student_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _Student_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _Student_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _Student_Request_type_support_symbol_names_t _Student_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, robot_control_ros2, srv, Student_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, robot_control_ros2, srv, Student_Request)),
  }
};

typedef struct _Student_Request_type_support_data_t
{
  void * data[2];
} _Student_Request_type_support_data_t;

static _Student_Request_type_support_data_t _Student_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _Student_Request_message_typesupport_map = {
  2,
  "robot_control_ros2",
  &_Student_Request_message_typesupport_ids.typesupport_identifier[0],
  &_Student_Request_message_typesupport_symbol_names.symbol_name[0],
  &_Student_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t Student_Request_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_Student_Request_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &robot_control_ros2__srv__Student_Request__get_type_hash,
  &robot_control_ros2__srv__Student_Request__get_type_description,
  &robot_control_ros2__srv__Student_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace robot_control_ros2

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, robot_control_ros2, srv, Student_Request)() {
  return &::robot_control_ros2::srv::rosidl_typesupport_c::Student_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "robot_control_ros2/srv/detail/student__struct.h"
// already included above
// #include "robot_control_ros2/srv/detail/student__type_support.h"
// already included above
// #include "robot_control_ros2/srv/detail/student__functions.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
// already included above
// #include "rosidl_typesupport_c/message_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_c/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace robot_control_ros2
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _Student_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _Student_Response_type_support_ids_t;

static const _Student_Response_type_support_ids_t _Student_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _Student_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _Student_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _Student_Response_type_support_symbol_names_t _Student_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, robot_control_ros2, srv, Student_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, robot_control_ros2, srv, Student_Response)),
  }
};

typedef struct _Student_Response_type_support_data_t
{
  void * data[2];
} _Student_Response_type_support_data_t;

static _Student_Response_type_support_data_t _Student_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _Student_Response_message_typesupport_map = {
  2,
  "robot_control_ros2",
  &_Student_Response_message_typesupport_ids.typesupport_identifier[0],
  &_Student_Response_message_typesupport_symbol_names.symbol_name[0],
  &_Student_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t Student_Response_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_Student_Response_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &robot_control_ros2__srv__Student_Response__get_type_hash,
  &robot_control_ros2__srv__Student_Response__get_type_description,
  &robot_control_ros2__srv__Student_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace robot_control_ros2

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, robot_control_ros2, srv, Student_Response)() {
  return &::robot_control_ros2::srv::rosidl_typesupport_c::Student_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "robot_control_ros2/srv/detail/student__struct.h"
// already included above
// #include "robot_control_ros2/srv/detail/student__type_support.h"
// already included above
// #include "robot_control_ros2/srv/detail/student__functions.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
// already included above
// #include "rosidl_typesupport_c/message_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_c/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace robot_control_ros2
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _Student_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _Student_Event_type_support_ids_t;

static const _Student_Event_type_support_ids_t _Student_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _Student_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _Student_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _Student_Event_type_support_symbol_names_t _Student_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, robot_control_ros2, srv, Student_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, robot_control_ros2, srv, Student_Event)),
  }
};

typedef struct _Student_Event_type_support_data_t
{
  void * data[2];
} _Student_Event_type_support_data_t;

static _Student_Event_type_support_data_t _Student_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _Student_Event_message_typesupport_map = {
  2,
  "robot_control_ros2",
  &_Student_Event_message_typesupport_ids.typesupport_identifier[0],
  &_Student_Event_message_typesupport_symbol_names.symbol_name[0],
  &_Student_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t Student_Event_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_Student_Event_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &robot_control_ros2__srv__Student_Event__get_type_hash,
  &robot_control_ros2__srv__Student_Event__get_type_description,
  &robot_control_ros2__srv__Student_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace robot_control_ros2

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, robot_control_ros2, srv, Student_Event)() {
  return &::robot_control_ros2::srv::rosidl_typesupport_c::Student_Event_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "robot_control_ros2/srv/detail/student__type_support.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/service_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
#include "service_msgs/msg/service_event_info.h"
#include "builtin_interfaces/msg/time.h"

namespace robot_control_ros2
{

namespace srv
{

namespace rosidl_typesupport_c
{
typedef struct _Student_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _Student_type_support_ids_t;

static const _Student_type_support_ids_t _Student_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _Student_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _Student_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _Student_type_support_symbol_names_t _Student_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, robot_control_ros2, srv, Student)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, robot_control_ros2, srv, Student)),
  }
};

typedef struct _Student_type_support_data_t
{
  void * data[2];
} _Student_type_support_data_t;

static _Student_type_support_data_t _Student_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _Student_service_typesupport_map = {
  2,
  "robot_control_ros2",
  &_Student_service_typesupport_ids.typesupport_identifier[0],
  &_Student_service_typesupport_symbol_names.symbol_name[0],
  &_Student_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t Student_service_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_Student_service_typesupport_map),
  rosidl_typesupport_c__get_service_typesupport_handle_function,
  &Student_Request_message_type_support_handle,
  &Student_Response_message_type_support_handle,
  &Student_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    robot_control_ros2,
    srv,
    Student
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    robot_control_ros2,
    srv,
    Student
  ),
  &robot_control_ros2__srv__Student__get_type_hash,
  &robot_control_ros2__srv__Student__get_type_description,
  &robot_control_ros2__srv__Student__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace robot_control_ros2

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_c, robot_control_ros2, srv, Student)() {
  return &::robot_control_ros2::srv::rosidl_typesupport_c::Student_service_type_support_handle;
}

#ifdef __cplusplus
}
#endif
