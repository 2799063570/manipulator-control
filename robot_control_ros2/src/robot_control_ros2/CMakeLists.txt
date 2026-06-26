cmake_minimum_required(VERSION 3.8)
project(robot_control_ros2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/JointStateCommand.msg"
  "srv/Student.srv"
  "msg/JointVelocityCmd.msg"
  "msg/JointPositionCmd.msg")

add_executable(joint_velocity_publisher src/joint_velocity_publisher.cpp)
ament_target_dependencies(
  joint_velocity_publisher
  "rclcpp"
  "std_msgs"
)
add_executable(joint_velocity_subscriber src/joint_velocity_subscriber.cpp)
ament_target_dependencies(
  joint_velocity_subscriber
  "rclcpp"
  "std_msgs"
)
add_executable(timer_control_node src/timer_control_node.cpp)
ament_target_dependencies(
  timer_control_node
  "rclcpp"
  "std_msgs"
)
add_executable(robot_state_pub src/robot_state_pub.cpp)
ament_target_dependencies(
  robot_state_pub
  "rclcpp"
  "std_msgs"
)
add_executable(student_servier src/student_servier.cpp)
ament_target_dependencies(
  student_servier
  "rclcpp"
  "std_msgs"
)
add_executable(student_client src/student_client.cpp)
ament_target_dependencies(
  student_client
  "rclcpp"
  "std_msgs"
)
add_executable(vel_to_pos_node 
  src/vel_to_pos_node.cpp
  src/robot_utils/robot_state.cpp
  src/robot_utils/safety_limiter.cpp
  src/robot_utils/cubic_interpolator.cpp
  src/robot_utils/trajectory_buffer.cpp)

target_include_directories(vel_to_pos_node PUBLIC
  include
)

ament_target_dependencies(
  vel_to_pos_node
  "rclcpp"
  "std_msgs"
)

add_executable(cubic_interpolator_test 
  src/examples/cubic_interpolator_test.cpp
  src/robot_utils/robot_state.cpp
  src/robot_utils/cubic_interpolator.cpp)

target_include_directories(cubic_interpolator_test PUBLIC
  include
)

ament_target_dependencies(
  cubic_interpolator_test
  "rclcpp"
  "std_msgs"
)

rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME}
  rosidl_typesupport_cpp
)

target_link_libraries(robot_state_pub
  "${cpp_typesupport_target}"
)
target_link_libraries(student_servier
  "${cpp_typesupport_target}"
)
target_link_libraries(student_client
  "${cpp_typesupport_target}"
)
target_link_libraries(vel_to_pos_node
  "${cpp_typesupport_target}"
)
target_link_libraries(joint_velocity_publisher
  "${cpp_typesupport_target}"
)
install(TARGETS
  joint_velocity_publisher 
  joint_velocity_subscriber 
  timer_control_node 
  robot_state_pub 
  student_servier
  student_client
  vel_to_pos_node
  cubic_interpolator_test
  DESTINATION lib/${PROJECT_NAME})
ament_export_dependencies(rosidl_default_runtime)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
