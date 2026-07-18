cmake_minimum_required(VERSION 3.22)
project(aubo_i5_hardware)

find_package(ament_cmake REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(aubo_dashboard_msgs REQUIRED)
find_package(aubo_msgs REQUIRED)
find_package(controller_manager REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(yaml_cpp_vendor REQUIRED)
find_package(yaml-cpp REQUIRED)

# Keep the vendor SDK beside this package so that a copied workspace remains
# self-contained.  A caller can still override this with
# -Daubo_sdk_DIR=/path/to/lib/cmake/aubo_sdk.
set(AUBO_SDK_BUNDLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/aubo_sdk")
if(NOT DEFINED aubo_sdk_DIR OR "${aubo_sdk_DIR}" STREQUAL "")
  set(aubo_sdk_DIR "${AUBO_SDK_BUNDLED_DIR}/lib/cmake/aubo_sdk"
    CACHE PATH "Directory containing aubo_sdkConfig.cmake")
endif()

if(NOT EXISTS "${aubo_sdk_DIR}/aubo_sdkConfig.cmake" AND
   NOT EXISTS "${aubo_sdk_DIR}/aubo_sdk-config.cmake")
  message(FATAL_ERROR
    "AUBO SDK was not found. Extract the vendor SDK so that this file exists:\n"
    "  ${AUBO_SDK_BUNDLED_DIR}/lib/cmake/aubo_sdk/aubo_sdkConfig.cmake\n"
    "Or configure with -Daubo_sdk_DIR=/absolute/path/to/lib/cmake/aubo_sdk.")
endif()
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
set_target_properties(aubo_i5_hardware_plugin PROPERTIES
  BUILD_RPATH "${AUBO_SDK_BUNDLED_DIR}/lib"
  INSTALL_RPATH "$ORIGIN/aubo_i5_hardware/aubo_sdk"
)
ament_target_dependencies(aubo_i5_hardware_plugin
  aubo_dashboard_msgs aubo_msgs
  hardware_interface pluginlib rclcpp rclcpp_lifecycle
)

foreach(test_program
    aubo_sdk_smoke_test
    aubo_i5_connection_test
    aubo_i5_single_joint_test)
  add_executable(${test_program} src/${test_program}.cpp)
  target_compile_features(${test_program} PUBLIC cxx_std_17)
  target_include_directories(${test_program} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
  target_link_libraries(${test_program} aubo_sdk::aubo_sdk)
  ament_target_dependencies(${test_program} ament_index_cpp yaml_cpp_vendor)
  target_link_libraries(${test_program} yaml-cpp)
  set_target_properties(${test_program} PROPERTIES
    BUILD_RPATH "${AUBO_SDK_BUNDLED_DIR}/lib"
    INSTALL_RPATH "$ORIGIN/aubo_sdk"
  )
endforeach()

pluginlib_export_plugin_description_file(
  hardware_interface hardware_interface_plugin.xml
)

install(TARGETS
  aubo_i5_hardware_plugin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(TARGETS
  aubo_sdk_smoke_test
  aubo_i5_connection_test
  aubo_i5_single_joint_test
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY config launch DESTINATION share/${PROJECT_NAME})

# Bundle only the runtime shared objects in the install space.  The headers
# and CMake package files remain in third_party for source builds.
install(DIRECTORY "${AUBO_SDK_BUNDLED_DIR}/lib/"
  DESTINATION lib/aubo_i5_hardware/aubo_sdk
  FILES_MATCHING
  PATTERN "*.so"
  PATTERN "*.so.*"
)

ament_export_dependencies(
  aubo_dashboard_msgs aubo_msgs
  hardware_interface pluginlib rclcpp rclcpp_lifecycle
  ament_index_cpp yaml_cpp_vendor
)
ament_package()
