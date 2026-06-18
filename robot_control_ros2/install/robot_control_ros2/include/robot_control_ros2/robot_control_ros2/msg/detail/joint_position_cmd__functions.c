// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from robot_control_ros2:msg/JointPositionCmd.idl
// generated code does not contain a copyright notice
#include "robot_control_ros2/msg/detail/joint_position_cmd__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
robot_control_ros2__msg__JointPositionCmd__init(robot_control_ros2__msg__JointPositionCmd * msg)
{
  if (!msg) {
    return false;
  }
  // position_cmd
  return true;
}

void
robot_control_ros2__msg__JointPositionCmd__fini(robot_control_ros2__msg__JointPositionCmd * msg)
{
  if (!msg) {
    return;
  }
  // position_cmd
}

bool
robot_control_ros2__msg__JointPositionCmd__are_equal(const robot_control_ros2__msg__JointPositionCmd * lhs, const robot_control_ros2__msg__JointPositionCmd * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // position_cmd
  for (size_t i = 0; i < 6; ++i) {
    if (lhs->position_cmd[i] != rhs->position_cmd[i]) {
      return false;
    }
  }
  return true;
}

bool
robot_control_ros2__msg__JointPositionCmd__copy(
  const robot_control_ros2__msg__JointPositionCmd * input,
  robot_control_ros2__msg__JointPositionCmd * output)
{
  if (!input || !output) {
    return false;
  }
  // position_cmd
  for (size_t i = 0; i < 6; ++i) {
    output->position_cmd[i] = input->position_cmd[i];
  }
  return true;
}

robot_control_ros2__msg__JointPositionCmd *
robot_control_ros2__msg__JointPositionCmd__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robot_control_ros2__msg__JointPositionCmd * msg = (robot_control_ros2__msg__JointPositionCmd *)allocator.allocate(sizeof(robot_control_ros2__msg__JointPositionCmd), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(robot_control_ros2__msg__JointPositionCmd));
  bool success = robot_control_ros2__msg__JointPositionCmd__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
robot_control_ros2__msg__JointPositionCmd__destroy(robot_control_ros2__msg__JointPositionCmd * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    robot_control_ros2__msg__JointPositionCmd__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
robot_control_ros2__msg__JointPositionCmd__Sequence__init(robot_control_ros2__msg__JointPositionCmd__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robot_control_ros2__msg__JointPositionCmd * data = NULL;

  if (size) {
    data = (robot_control_ros2__msg__JointPositionCmd *)allocator.zero_allocate(size, sizeof(robot_control_ros2__msg__JointPositionCmd), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = robot_control_ros2__msg__JointPositionCmd__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        robot_control_ros2__msg__JointPositionCmd__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
robot_control_ros2__msg__JointPositionCmd__Sequence__fini(robot_control_ros2__msg__JointPositionCmd__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      robot_control_ros2__msg__JointPositionCmd__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

robot_control_ros2__msg__JointPositionCmd__Sequence *
robot_control_ros2__msg__JointPositionCmd__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robot_control_ros2__msg__JointPositionCmd__Sequence * array = (robot_control_ros2__msg__JointPositionCmd__Sequence *)allocator.allocate(sizeof(robot_control_ros2__msg__JointPositionCmd__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = robot_control_ros2__msg__JointPositionCmd__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
robot_control_ros2__msg__JointPositionCmd__Sequence__destroy(robot_control_ros2__msg__JointPositionCmd__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    robot_control_ros2__msg__JointPositionCmd__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
robot_control_ros2__msg__JointPositionCmd__Sequence__are_equal(const robot_control_ros2__msg__JointPositionCmd__Sequence * lhs, const robot_control_ros2__msg__JointPositionCmd__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!robot_control_ros2__msg__JointPositionCmd__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
robot_control_ros2__msg__JointPositionCmd__Sequence__copy(
  const robot_control_ros2__msg__JointPositionCmd__Sequence * input,
  robot_control_ros2__msg__JointPositionCmd__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(robot_control_ros2__msg__JointPositionCmd);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    robot_control_ros2__msg__JointPositionCmd * data =
      (robot_control_ros2__msg__JointPositionCmd *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!robot_control_ros2__msg__JointPositionCmd__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          robot_control_ros2__msg__JointPositionCmd__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!robot_control_ros2__msg__JointPositionCmd__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
