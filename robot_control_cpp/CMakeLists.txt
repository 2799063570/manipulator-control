cmake_minimum_required(VERSION 3.10)

project(robot_control_cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(test_robot_state
    src/robot_state.cpp
    examples/main.cpp
)
