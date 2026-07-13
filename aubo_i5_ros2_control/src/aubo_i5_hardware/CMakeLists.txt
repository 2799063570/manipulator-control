cmake_minimum_required(VERSION 3.22)
project(aubo_i5_hardware)

find_package(ament_cmake REQUIRED)
find_package(aubo_dashboard_msgs REQUIRED)
find_package(aubo_msgs REQUIRED)
find_package(controller_manager REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)

# Install the AUBO SDK separately on the target Ubuntu host and point CMake at
# its lib/cmake/aubo_sdk directory with -Daubo_sdk_DIR=... .  Do not download
# a binary SDK implicitly as part of a real-robot build.
find_package(aubo_sdk REQUIRED)

add_library(aubo_i5_hardware_plugin SHARED
  src/aubo_i5_system.cpp
)
target_compile_features(aubo_i5_hardware_plugin PUBLIC cxx_std_17)
target_include_directories(aubo_i5_hardware_plugin PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(aubo_i5_hardware_plugin aubo_sdk::aubo_sdk)
ament_target_dependencies(aubo_i5_hardware_plugin
  aubo_dashboard_msgs aubo_msgs
  hardware_interface pluginlib rclcpp rclcpp_lifecycle
)

pluginlib_export_plugin_description_file(
  hardware_interface hardware_interface_plugin.xml
)

install(TARGETS aubo_i5_hardware_plugin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY config launch DESTINATION share/${PROJECT_NAME})

ament_export_dependencies(
  aubo_dashboard_msgs aubo_msgs
  hardware_interface pluginlib rclcpp rclcpp_lifecycle
)
ament_package()
