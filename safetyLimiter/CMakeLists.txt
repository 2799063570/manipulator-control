cmake_minimum_required(VERSION 3.10)

project(safetyLimiter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(safety_test
    src/safetyLimiter.cpp
    examples/main.cpp
)