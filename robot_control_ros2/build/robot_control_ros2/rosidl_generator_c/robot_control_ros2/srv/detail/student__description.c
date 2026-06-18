// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from robot_control_ros2:srv/Student.idl
// generated code does not contain a copyright notice

#include "robot_control_ros2/srv/detail/student__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_robot_control_ros2
const rosidl_type_hash_t *
robot_control_ros2__srv__Student__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x9d, 0xa4, 0x71, 0x48, 0x81, 0x2e, 0xa9, 0x53,
      0x1d, 0xfa, 0x90, 0x82, 0x4e, 0x59, 0x3b, 0x13,
      0xbf, 0x70, 0x51, 0x36, 0x6c, 0x14, 0xd1, 0x54,
      0x78, 0xbc, 0xc6, 0x2e, 0xb9, 0xce, 0xc8, 0xed,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_robot_control_ros2
const rosidl_type_hash_t *
robot_control_ros2__srv__Student_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x9d, 0xd3, 0xe1, 0x27, 0x0e, 0xeb, 0x97, 0x27,
      0xdb, 0x8d, 0x73, 0xd7, 0xb2, 0x2f, 0x14, 0xdb,
      0x28, 0x0f, 0xe2, 0xd2, 0xa4, 0x44, 0xe0, 0xfa,
      0x4a, 0xf4, 0x2a, 0x59, 0x26, 0x99, 0xe5, 0xc7,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_robot_control_ros2
const rosidl_type_hash_t *
robot_control_ros2__srv__Student_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x3c, 0xfb, 0xc7, 0xc8, 0x23, 0xbd, 0xee, 0x55,
      0x61, 0x94, 0x49, 0x4f, 0xad, 0x97, 0x36, 0xa5,
      0xaf, 0x23, 0x1c, 0xa6, 0xbb, 0x6c, 0x34, 0x27,
      0x5f, 0x81, 0x2a, 0x23, 0xde, 0x1c, 0xe5, 0x89,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_robot_control_ros2
const rosidl_type_hash_t *
robot_control_ros2__srv__Student_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xe3, 0x28, 0x07, 0x78, 0x6e, 0x13, 0x6a, 0x13,
      0xb9, 0xa6, 0x64, 0x6d, 0xcd, 0xa5, 0xe4, 0x8e,
      0x28, 0xa1, 0x48, 0xa1, 0x34, 0x0c, 0x53, 0x18,
      0xbc, 0xf5, 0x57, 0xea, 0x92, 0xae, 0xfb, 0x71,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "service_msgs/msg/detail/service_event_info__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char robot_control_ros2__srv__Student__TYPE_NAME[] = "robot_control_ros2/srv/Student";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char robot_control_ros2__srv__Student_Event__TYPE_NAME[] = "robot_control_ros2/srv/Student_Event";
static char robot_control_ros2__srv__Student_Request__TYPE_NAME[] = "robot_control_ros2/srv/Student_Request";
static char robot_control_ros2__srv__Student_Response__TYPE_NAME[] = "robot_control_ros2/srv/Student_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char robot_control_ros2__srv__Student__FIELD_NAME__request_message[] = "request_message";
static char robot_control_ros2__srv__Student__FIELD_NAME__response_message[] = "response_message";
static char robot_control_ros2__srv__Student__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field robot_control_ros2__srv__Student__FIELDS[] = {
  {
    {robot_control_ros2__srv__Student__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {robot_control_ros2__srv__Student_Request__TYPE_NAME, 38, 38},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {robot_control_ros2__srv__Student_Response__TYPE_NAME, 39, 39},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {robot_control_ros2__srv__Student_Event__TYPE_NAME, 36, 36},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription robot_control_ros2__srv__Student__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Event__TYPE_NAME, 36, 36},
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Request__TYPE_NAME, 38, 38},
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Response__TYPE_NAME, 39, 39},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
robot_control_ros2__srv__Student__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {robot_control_ros2__srv__Student__TYPE_NAME, 30, 30},
      {robot_control_ros2__srv__Student__FIELDS, 3, 3},
    },
    {robot_control_ros2__srv__Student__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = robot_control_ros2__srv__Student_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = robot_control_ros2__srv__Student_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = robot_control_ros2__srv__Student_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char robot_control_ros2__srv__Student_Request__FIELD_NAME__name[] = "name";
static char robot_control_ros2__srv__Student_Request__FIELD_NAME__num1[] = "num1";
static char robot_control_ros2__srv__Student_Request__FIELD_NAME__num2[] = "num2";

static rosidl_runtime_c__type_description__Field robot_control_ros2__srv__Student_Request__FIELDS[] = {
  {
    {robot_control_ros2__srv__Student_Request__FIELD_NAME__name, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Request__FIELD_NAME__num1, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Request__FIELD_NAME__num2, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
robot_control_ros2__srv__Student_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {robot_control_ros2__srv__Student_Request__TYPE_NAME, 38, 38},
      {robot_control_ros2__srv__Student_Request__FIELDS, 3, 3},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char robot_control_ros2__srv__Student_Response__FIELD_NAME__num[] = "num";
static char robot_control_ros2__srv__Student_Response__FIELD_NAME__is_ok[] = "is_ok";

static rosidl_runtime_c__type_description__Field robot_control_ros2__srv__Student_Response__FIELDS[] = {
  {
    {robot_control_ros2__srv__Student_Response__FIELD_NAME__num, 3, 3},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Response__FIELD_NAME__is_ok, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
robot_control_ros2__srv__Student_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {robot_control_ros2__srv__Student_Response__TYPE_NAME, 39, 39},
      {robot_control_ros2__srv__Student_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char robot_control_ros2__srv__Student_Event__FIELD_NAME__info[] = "info";
static char robot_control_ros2__srv__Student_Event__FIELD_NAME__request[] = "request";
static char robot_control_ros2__srv__Student_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field robot_control_ros2__srv__Student_Event__FIELDS[] = {
  {
    {robot_control_ros2__srv__Student_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {robot_control_ros2__srv__Student_Request__TYPE_NAME, 38, 38},
    },
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {robot_control_ros2__srv__Student_Response__TYPE_NAME, 39, 39},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription robot_control_ros2__srv__Student_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Request__TYPE_NAME, 38, 38},
    {NULL, 0, 0},
  },
  {
    {robot_control_ros2__srv__Student_Response__TYPE_NAME, 39, 39},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
robot_control_ros2__srv__Student_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {robot_control_ros2__srv__Student_Event__TYPE_NAME, 36, 36},
      {robot_control_ros2__srv__Student_Event__FIELDS, 3, 3},
    },
    {robot_control_ros2__srv__Student_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = robot_control_ros2__srv__Student_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = robot_control_ros2__srv__Student_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "string name\n"
  "int32 num1\n"
  "int32 num2\n"
  "\n"
  "---\n"
  "\n"
  "int32 num\n"
  "bool is_ok";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
robot_control_ros2__srv__Student__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {robot_control_ros2__srv__Student__TYPE_NAME, 30, 30},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 60, 60},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
robot_control_ros2__srv__Student_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {robot_control_ros2__srv__Student_Request__TYPE_NAME, 38, 38},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
robot_control_ros2__srv__Student_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {robot_control_ros2__srv__Student_Response__TYPE_NAME, 39, 39},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
robot_control_ros2__srv__Student_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {robot_control_ros2__srv__Student_Event__TYPE_NAME, 36, 36},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
robot_control_ros2__srv__Student__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *robot_control_ros2__srv__Student__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *robot_control_ros2__srv__Student_Event__get_individual_type_description_source(NULL);
    sources[3] = *robot_control_ros2__srv__Student_Request__get_individual_type_description_source(NULL);
    sources[4] = *robot_control_ros2__srv__Student_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
robot_control_ros2__srv__Student_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *robot_control_ros2__srv__Student_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
robot_control_ros2__srv__Student_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *robot_control_ros2__srv__Student_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
robot_control_ros2__srv__Student_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *robot_control_ros2__srv__Student_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *robot_control_ros2__srv__Student_Request__get_individual_type_description_source(NULL);
    sources[3] = *robot_control_ros2__srv__Student_Response__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
