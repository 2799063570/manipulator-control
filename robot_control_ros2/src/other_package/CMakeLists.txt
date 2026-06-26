cmake_minimum_required(VERSION 3.8)
project(other_package)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(robot_control_ros2 REQUIRED)
find_package(robot_control REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(self_msg_test_publisher src/self_msg_test_publisher.cpp)
ament_target_dependencies( self_msg_test_publisher
  "rclcpp"
  "robot_control_ros2"
)
add_executable(self_msg_test_subscriber src/self_msg_test_subscriber.cpp)
ament_target_dependencies( self_msg_test_subscriber
  "rclcpp"
  "robot_control_ros2"
)

add_executable(lib_test src/lib_test.cpp)
ament_target_dependencies(lib_test
  robot_control
)

add_executable(jacobian_lib_test src/jacobian_lib_test.cpp)
ament_target_dependencies(jacobian_lib_test
  robot_control
)
target_link_libraries(jacobian_lib_test #PRIVATE
 Eigen3::Eigen
)


install(TARGETS
  self_msg_test_publisher self_msg_test_subscriber lib_test jacobian_lib_test
  DESTINATION lib/${PROJECT_NAME}
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
