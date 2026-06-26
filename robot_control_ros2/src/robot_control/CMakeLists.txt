cmake_minimum_required(VERSION 3.8)
project(robot_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies 查找外部的包
find_package(ament_cmake REQUIRED)
# find_package(rclcpp REQUIRED)
# find_package(std_msgs REQUIRED)
find_package(Eigen3 REQUIRED)


add_library(robot_state_control
  src/robot_state.cpp
  src/safety_limiter.cpp
  src/trajectory_buffer.cpp
  src/cubic_interpolator.cpp)  # 输出库
add_library(jacobian_solver_control
  src/jacobian_solver.cpp)

target_include_directories(robot_state_control PUBLIC
  # include
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
) # 链接库的头文件路径 源码头文件+编译头文件
target_include_directories(jacobian_solver_control PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(jacobian_solver_control PUBLIC
  Eigen3::Eigen
)

add_executable(control_loop_demo
  examples/control_loop_demo.cpp
)
target_link_libraries(control_loop_demo PRIVATE
  robot_state_control
) # 链接生成的库

add_executable(robot_state_test
  examples/robot_state_test.cpp
)
target_link_libraries(robot_state_test PRIVATE
  robot_state_control
)

add_executable(cubic_interpolator_test
  examples/cubic_interpolator_test.cpp
)
# target_include_directories(cubic_interpolator_test PRIVATE
#   include
# )
target_link_libraries(cubic_interpolator_test PRIVATE
  robot_state_control
)

add_executable(safety_limiter_test
  examples/safety_limiter_test.cpp
)
target_link_libraries(safety_limiter_test PRIVATE
  robot_state_control
)

add_executable(trajectory_buffer_test
  examples/trajectory_buffer_test.cpp
)
target_link_libraries(trajectory_buffer_test PRIVATE
  robot_state_control
)

add_executable(jacobian_solver_test
  examples/jacobian_solver_test.cpp
)
target_link_libraries(jacobian_solver_test PRIVATE
  jacobian_solver_control
)

install(DIRECTORY include/
  DESTINATION include
)

install(TARGETS robot_state_control jacobian_solver_control
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(TARGETS control_loop_demo robot_state_test cubic_interpolator_test safety_limiter_test
  trajectory_buffer_test jacobian_solver_test
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_libraries(
  robot_state_control
  jacobian_solver_control
)

ament_export_include_directories(include)
ament_export_dependencies(Eigen3)

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
