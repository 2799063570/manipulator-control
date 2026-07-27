cmake_minimum_required(VERSION 3.22)
project(wheeltec_navigation)

find_package(ament_cmake REQUIRED)

install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME}
)
install(PROGRAMS
  scripts/nav2_lifecycle_bringup.py
  DESTINATION lib/${PROJECT_NAME}
)
install(FILES README.md
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
