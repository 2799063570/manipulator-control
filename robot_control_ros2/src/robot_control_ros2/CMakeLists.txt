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
find_package(robot_control REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(turtlesim REQUIRED)
# find_package(turtlesim REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(pluginlib REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/JointStateCommand.msg"
  "srv/Student.srv"
  "msg/JointVelocityCmd.msg"
  "msg/JointPositionCmd.msg"
  "action/Fibonacci.action")

# -------------------- pluginlib 动态插件学习示例 --------------------
# 两个派生类被编译进共享库；加载程序不会直接链接这个目标。
add_library(printer_plugins SHARED
  src/pluginlib_demo/printer_plugins.cpp
)
target_compile_features(printer_plugins PUBLIC cxx_std_17)
target_include_directories(printer_plugins
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(printer_plugins pluginlib)

# 主程序只依赖基类和 pluginlib，不链接 printer_plugins。
add_executable(pluginlib_demo
  src/pluginlib_demo/pluginlib_demo.cpp
)
target_compile_features(pluginlib_demo PRIVATE cxx_std_17)
target_include_directories(pluginlib_demo PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(pluginlib_demo pluginlib)

# 把 XML 注册到 ament 索引，pluginlib 才能通过基类包名发现插件。
pluginlib_export_plugin_description_file(robot_control_ros2 printer_plugins.xml)

add_executable(joint_velocity_publisher src/topic/joint_velocity_publisher.cpp)
ament_target_dependencies(
  joint_velocity_publisher
  "rclcpp"
  "std_msgs"
)
add_executable(joint_velocity_subscriber src/topic/joint_velocity_subscriber.cpp)
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
add_executable(student_servier src/service/student_servier.cpp)
ament_target_dependencies(
  student_servier
  "rclcpp"
  "std_msgs"
)
add_executable(student_client src/service/student_client.cpp)
ament_target_dependencies(
  student_client
  "rclcpp"
  "std_msgs"
)

add_executable(vel_to_pos_node 
  src/vel_to_pos_node.cpp
)
ament_target_dependencies(
  vel_to_pos_node
  "rclcpp"
  "std_msgs"
  robot_control
)

add_executable(robot_velocity_pub 
  src/robot_velocity_pub.cpp
)
ament_target_dependencies(
  robot_velocity_pub
  "rclcpp"
)

add_executable(param_server 
  src/param/param_server.cpp
)
ament_target_dependencies(
  param_server
  "rclcpp"
)

add_executable(param_client 
  src/param/param_client.cpp
)
ament_target_dependencies(
  param_client
  "rclcpp"
)

add_executable(tf_static_broadcaster 
  src/tf/tf_static_broadcaster.cpp
)
ament_target_dependencies(
  tf_static_broadcaster
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
)

add_executable(tf_dynamic_broadcaster 
  src/tf/tf_dynamic_broadcaster.cpp
)
ament_target_dependencies(
  tf_dynamic_broadcaster
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "turtlesim"
)

add_executable(tf_point_publisher 
  src/tf/tf_point_publisher.cpp
)
ament_target_dependencies(
  tf_point_publisher
  "rclcpp"
  "geometry_msgs"
)

add_executable(tf_transform_listener 
  src/tf/tf_transform_listener.cpp
)
ament_target_dependencies(
  tf_transform_listener
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
)

add_executable(tf_point_listener 
  src/tf/tf_point_listener.cpp
)
ament_target_dependencies(
  tf_point_listener
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "tf2_geometry_msgs"
  "message_filters"
)

add_executable(joint_position_cmd_to_states 
  src/joint_position_cmd_to_states.cpp
)
ament_target_dependencies(
  joint_position_cmd_to_states
  "rclcpp"
  "sensor_msgs"
  robot_control
)

add_executable(robot_position_cmd 
  src/robot_position_cmd.cpp
)
ament_target_dependencies(
  robot_position_cmd
  "rclcpp"
  robot_control
)
add_executable(fibonacci_server 
  src/action/fibonacci_server.cpp
)
ament_target_dependencies(
  fibonacci_server
  "rclcpp"
  "rclcpp_action"
)
add_executable(fibonacci_client 
  src/action/fibonacci_client.cpp
)
ament_target_dependencies(
  fibonacci_client
  "rclcpp"
  "rclcpp_action"
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
target_link_libraries(robot_velocity_pub
  "${cpp_typesupport_target}"
)
target_link_libraries(joint_position_cmd_to_states
  "${cpp_typesupport_target}"
)
target_link_libraries(robot_position_cmd
  "${cpp_typesupport_target}"
)
target_link_libraries(fibonacci_server
  "${cpp_typesupport_target}"
)
target_link_libraries(fibonacci_client
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
  robot_velocity_pub
  tf_static_broadcaster
  tf_dynamic_broadcaster
  tf_point_publisher
  tf_transform_listener
  tf_point_listener
  joint_position_cmd_to_states
  robot_position_cmd
  param_server
  param_client
  fibonacci_server
  fibonacci_client
  DESTINATION lib/${PROJECT_NAME})

# 插件库遵循 ROS 2/ament 惯例安装到 lib，加载程序安装到 ros2 run 查找的位置。
install(TARGETS printer_plugins
  LIBRARY DESTINATION lib
)
install(TARGETS pluginlib_demo
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY include/
  DESTINATION include
)
ament_export_dependencies(rosidl_default_runtime)

install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}
)

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
