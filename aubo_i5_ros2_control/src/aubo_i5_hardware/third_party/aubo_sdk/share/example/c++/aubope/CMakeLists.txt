cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

file(GLOB _src "*.h" "*.cpp")

add_executable(example_aubope ${_src})

target_link_libraries(example_aubope ${SDK_LIB} eigen)
