cmake_minimum_required(VERSION 3.10)

project(trajectory_buffer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXx_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(test_trajectory_buffer
    src/trajectory_buffer.cpp
    examples/main.cpp)