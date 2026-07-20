cmake_minimum_required(VERSION 3.22)
project(wheeltec_gazebo)

find_package(ament_cmake REQUIRED)

install(DIRECTORY launch urdf worlds
  DESTINATION share/${PROJECT_NAME}
)
install(FILES README.md
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
