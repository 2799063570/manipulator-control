cmake_minimum_required(VERSION 3.10)

project(control_loop_demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(test_control
    src/cubic_interpolator.cpp
    src/robot_state.cpp
    src/safety_limiter.cpp
    src/trajectory_buffer.cpp
    examples/main.cpp)