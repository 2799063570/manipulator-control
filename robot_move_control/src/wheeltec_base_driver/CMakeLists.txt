cmake_minimum_required(VERSION 3.22)
project(wheeltec_base_driver)

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)

add_executable(wheeltec_base_driver_node
  src/wheeltec_base_driver_node.cpp
)
target_compile_features(wheeltec_base_driver_node PUBLIC cxx_std_17)
ament_target_dependencies(wheeltec_base_driver_node
  geometry_msgs nav_msgs rclcpp sensor_msgs std_msgs tf2 tf2_ros
)

install(TARGETS wheeltec_base_driver_node
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
