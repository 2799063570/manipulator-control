cmake_minimum_required(VERSION 3.16)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example_robot_proxy
    main.cpp
    widget.cpp
    widget.h
    widget.ui
)
target_link_libraries(example_robot_proxy PRIVATE ${SDK_LIB} ${PROXY_LIB} Qt5::Widgets)
