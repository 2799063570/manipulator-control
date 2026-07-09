cmake_minimum_required(VERSION 3.22)
project(simple_arm_moveit_config)

find_package(ament_cmake REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(planner_comparison src/planner_comparison.cpp)
ament_target_dependencies(
  planner_comparison
  moveit_ros_planning_interface
  rclcpp
)

add_executable(moveit_cpp_demo src/moveit_cpp_demo.cpp)
ament_target_dependencies(
  moveit_cpp_demo
  moveit_ros_planning_interface
  rclcpp
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/launch")
  install(
    DIRECTORY launch
    DESTINATION share/${PROJECT_NAME}
    PATTERN "setup_assistant.launch" EXCLUDE)
endif()

install(DIRECTORY config DESTINATION share/${PROJECT_NAME})
install(DIRECTORY docs DESTINATION share/${PROJECT_NAME})
install(FILES .setup_assistant DESTINATION share/${PROJECT_NAME})

install(
  TARGETS
    planner_comparison
    moveit_cpp_demo
  DESTINATION lib/${PROJECT_NAME})

ament_package()
