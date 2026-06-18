cmake_minimum_required(VERSION 3.10)

project(cubic_interpolator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(test_cubic_interpolator
    src/cubic_interpolator.cpp
    examples/main.cpp)