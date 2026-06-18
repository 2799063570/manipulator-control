cmake_minimum_required(VERSION 3.10)

project(jacobian_solver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)

include_directories(include)

add_executable(test_jacobian
    src/jacobian_solver.cpp
    examples/main.cpp)


        
target_link_libraries(test_jacobian
    PRIVATE
        Eigen3::Eigen
)