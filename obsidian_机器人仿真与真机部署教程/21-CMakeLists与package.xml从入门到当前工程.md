---
title: CMakeLists 与 package.xml：从入门到当前工程
tags: [CMake, package.xml, ament_cmake, colcon, ROS2]
---

# CMakeLists 与 package.xml：从入门到当前工程

## 0. 这一章要解决什么问题

写 ROS 2 C++ 包时，你经常会遇到“头文件找不到”“能编译但 ros2 run 找不到”“自定义 msg 生成不了”“运行时报库找不到”。这些不是 C++ 算法问题，而是构建系统没有把**依赖、目标、安装产物、运行环境**串起来。

本工程使用 `ament_cmake`：

```text
package.xml    声明：这个包是谁、依赖什么、如何被生态识别
CMakeLists.txt 定义：源码如何编译为库/可执行文件，哪些文件安装
colcon build   按包依赖顺序调用 CMake/ament 构建整个工作区
install/       构建产物和环境钩子；source 后 ROS 才能发现 package
```

一句话记忆：**package.xml 给 ROS/rosdep 看，CMakeLists.txt 给 CMake/编译器看；两边依赖应保持一致。**

本教程默认 underlay 为 `/opt/ros/jazzy`。CMake 与 package.xml 中的包名通常不含发行版前缀（如 `rclcpp`、`gz_ros2_control`）；`ros-jazzy-` 只出现在 Ubuntu apt 安装包名中，不能写进 `find_package()` 或 `<depend>`。

## 1. 工作区、package 与 overlay

你的目录 `robot_control_ros2` 是一个 workspace；它的 `src/` 下有多个 package，例如 `robot_control`、`robot_control_ros2`、`simple_arm_description`、`simple_arm_gazebo`、`simple_arm_moveit_config`。执行：

```bash
cd ~/manipulator-control/robot_control_ros2
source /opt/ros/jazzy/setup.bash        # underlay：系统 ROS
colcon build --symlink-install
source install/setup.bash               # overlay：当前工作区
```

source 顺序很重要：后 source 的同名 package 覆盖前者。诊断实际使用哪个包：

```bash
ros2 pkg prefix robot_control_ros2
ros2 pkg prefix simple_arm_gazebo
echo $AMENT_PREFIX_PATH
```

不要在仓库根目录构建，因为它没有一个统一的 `src/` 工作区结构；AUBO、小车和二维臂目前各自是独立工作区/包集合，应在相应目录单独 build。

## 2. package.xml 逐项理解

以 format=3 为例：

```xml
<?xml version="1.0"?>
<package format="3">
  <name>robot_control</name>
  <version>0.0.0</version>
  <description>Reusable robot control algorithms.</description>
  <maintainer email="you@example.com">Your Name</maintainer>
  <license>Apache-2.0</license>

  <buildtool_depend>ament_cmake</buildtool_depend>
  <depend>rclcpp</depend>
  <depend>Eigen3</depend>
  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
```

| 标签 | 含义 | 什么时候写 |
|---|---|---|
| `name` | 包唯一名，也是 `ros2 pkg` 名 | 必须与 `project()` 相同 |
| `version` | 版本号 | 发布/变更时维护 |
| `description`、`maintainer`、`license` | 元数据 | 不能长期保留 TODO |
| `buildtool_depend` | 构建工具 | C++ ament 包写 `ament_cmake` |
| `build_depend` | 仅编译需要 | 例如代码生成器工具 |
| `exec_depend` | 仅运行需要 | launch 运行时调用的包常写这里 |
| `depend` | build + export + exec 的简写 | 大部分普通 C++ API 依赖优先用它 |
| `build_export_depend` | 下游编译链接本包时需要 | 当本包公开头文件暴露此依赖时需要 |
| `test_depend` | 测试/代码规范工具 | ament lint、gtest |
| `member_of_group` | 接口包归类 | msg/srv/action 包通常加 rosidl_interface_packages |
| `export/build_type` | 告诉 ament 使用何种构建类型 | ament_cmake 必需 |

### 当前工程怎么读

- `robot_control` 是算法库，公开头文件使用 Eigen，所以 `Eigen3` 必须出现在 CMake 的 find_package，也应该在 package.xml 声明。
- `robot_control_ros2` 有 `.msg/.srv`，必须声明 `rosidl_default_generators`（构建时）和 `rosidl_default_runtime`（运行时），并把接口包加入 `rosidl_interface_packages` 组。
- `simple_arm_gazebo/package.xml` 中列出 controller、`gz_ros2_control`、`ros_gz_bridge`、MoveIt、RViz 等 `exec_depend`，因为 launch 时才要使用它们。

> [!warning] 常见坑
> 只在 CMake `find_package(rclcpp REQUIRED)`，却忘记 package.xml：本机可能碰巧能编，别人或 CI 的 rosdep 无法自动安装依赖。反过来只写 package.xml，CMake 不知道 include path/链接库，也会编译失败。

## 3. CMakeLists.txt 的基本骨架

```cmake
cmake_minimum_required(VERSION 3.8)
project(robot_control)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(Eigen3 REQUIRED)

add_library(${PROJECT_NAME} SHARED
  src/robot_state.cpp
  src/safety_limiter.cpp
  src/cubic_interpolator.cpp
  src/trajectory_buffer.cpp
  src/jacobian_solver.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_dependencies(Eigen3)
ament_package()
```

从上到下理解：`project` 设项目名；`find_package` 查依赖；`add_library` 把 cpp 编成可复用库；`target_include_directories` 区分构建时与安装后头文件路径；`target_link_libraries` 链接 Eigen；`install` 决定产物进入 install 空间的位置；`ament_export_*` 让其他 package 能链接这个库；`ament_package()` 必须放最后。

## 4. 当前 `robot_control`：算法库加测试可执行文件

算法源码不应该复制进每个 ROS node。正确方式是先做一个 library，再让 node 和 example 链接它：

```cmake
add_executable(jacobian_solver_test examples/jacobian_solver_test.cpp)
target_link_libraries(jacobian_solver_test ${PROJECT_NAME})

install(TARGETS jacobian_solver_test
  DESTINATION lib/${PROJECT_NAME}
)
```

为什么可执行文件安装到 `lib/${PROJECT_NAME}`，而不是 `bin`？这是 ROS 2 的常见约定，`ros2 run robot_control jacobian_solver_test` 就会在那里寻找。库本身仍通常安装到 `lib`。

编译单个包：

```bash
colcon build --packages-select robot_control
source install/setup.bash
ros2 run robot_control jacobian_solver_test
```

如果你改变了公开头文件/库接口，需要同时重编依赖它的 package：

```bash
colcon build --packages-up-to robot_control_ros2
```

## 5. 当前 `robot_control_ros2`：rclcpp 可执行文件

典型 node：

```cmake
find_package(rclcpp REQUIRED)
find_package(robot_control REQUIRED)

add_executable(vel_to_pos_node src/vel_to_pos_node.cpp)
ament_target_dependencies(vel_to_pos_node rclcpp robot_control)

install(TARGETS vel_to_pos_node
  DESTINATION lib/${PROJECT_NAME}
)
```

`ament_target_dependencies` 不只是链接；它会添加 ament package 导出的 include、库和编译定义。对非 ament 原生 CMake 库（如 Eigen）通常用 `target_link_libraries`；对 ROS package 优先 `ament_target_dependencies`。

在本项目中 `vel_to_pos_node.cpp` 包含 robot_control 的头文件，故它必须链接 robot_control；否则有时能编译但链接阶段出现 undefined reference，或下游安装后找不到头文件。

## 6. 自定义 msg、srv、action 的 CMake 与 package.xml

当前 `robot_control_ros2` 下已有 `msg/JointVelocityCmd.msg`、`JointPositionCmd.msg` 和 `srv/Student.srv`。标准写法：

```cmake
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/JointVelocityCmd.msg"
  "msg/JointPositionCmd.msg"
  "srv/Student.srv"
  DEPENDENCIES std_msgs
)
```

package.xml 对应：

```xml
<buildtool_depend>rosidl_default_generators</buildtool_depend>
<exec_depend>rosidl_default_runtime</exec_depend>
<member_of_group>rosidl_interface_packages</member_of_group>
```

如果同一个 C++ target 也使用自己生成的接口，需要让 target 等待 typesupport：

```cmake
rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(vel_to_pos_node "${cpp_typesupport_target}")
```

不同 ROS 版本和生成结构可能存在细节差异；遇到链接问题优先对照当前 `robot_control_ros2/CMakeLists.txt` 和 `rosidl` 官方示例，而不是随意复制 ROS 1 的 `message_generation` 写法。

## 7. description、launch、config、mesh 如何安装

ROS 启动时通过 package share directory 找 launch、URDF、YAML、world、mesh。仅把它们放在 src 目录不够，必须安装：

```cmake
install(DIRECTORY launch urdf config meshes worlds resources
  DESTINATION share/${PROJECT_NAME}
)
```

应只列出真实存在的目录；CMake 可能会因不存在目录而失败。描述包没有 C++ 时，CMake 可以很短：find ament、install 资源目录、ament_package。当前 `simple_arm_description`、`simple_arm_gazebo`、AUBO `robot_description` 都属于此类或以资源安装为主。

`--symlink-install` 对 launch、URDF、YAML 编辑很方便：install 空间指向源码，通常不必每改一次就编译。但新建文件、改变 install 规则、修改 C++、修改 msg/srv 后仍需 `colcon build`。

## 8. MoveIt/Gazebo launch 包的依赖

launch 文件 import 的每个 ROS package 都应该作为运行依赖。例如 `simple_arm_gazebo/gazebo.launch.py` 使用 `ros_gz_sim`、`ros_gz_bridge`、`controller_manager`、`robot_state_publisher`、`xacro`；`demo_gazebo.launch.py` 还使用 `moveit_configs_utils`、`moveit_ros_move_group`、RViz。它们通常写 `exec_depend`，因为包自身不链接其 C++ API，而是运行时启动其 executable。

易错：launch 能在开发电脑跑，不代表 package 依赖完整；那可能只是系统已安装。使用干净 Docker/另一台电脑的 `rosdep install` 是检查 package.xml 是否完整的好方法。

## 9. 常见报错与精确处理

| 报错/现象 | 最可能原因 | 处理 |
|---|---|---|
| `Could not find a package configuration file` | 缺 apt 依赖或 find_package 名称错 | rosdep；核对实际 CMake package 名 |
| `fatal error: xxx.hpp: No such file` | 没 find/链接/导出 include | CMake 加依赖，库 target PUBLIC include |
| `undefined reference` | 声明有、实现或链接库缺失 | add 源文件到 library，target 链接该库 |
| `ros2 run: No executable found` | target 未 install 到 `lib/${PROJECT_NAME}` | 加 install(TARGETS...)，重编/source |
| `Package ... not found` | 未 source / install 未生成 | 检查 `ros2 pkg prefix` 和 workspace |
| 找不到 launch/URDF | 忘记 install DIRECTORY | 加 share 安装规则，重编 |
| msg 头文件不存在 | 没 rosidl_generate_interfaces 或依赖顺序错 | 添加生成规则/类型支持链接，clean 后重编 |
| 旧代码仍运行 | overlay 或缓存问题 | `colcon build --symlink-install`，重新 source；必要时只删除本包 build/install/log 后重编 |

> [!warning] 清理构建目录
> 不要默认删除整个 workspace 的 `build/install/log`，它会抹掉其他包的构建产物。优先 `colcon build --packages-select 包名`；只有缓存确实损坏时才针对明确的包清理，并确认目录路径正确。

## 10. 推荐的开发工作流

1. 新建 package：`ros2 pkg create --build-type ament_cmake my_package --dependencies rclcpp`。
2. 先让 package.xml 与 CMake 依赖一致，构建空包。
3. 每增加一个库或 node，立即 `colcon build --packages-select` 并 `ros2 run` 验证。
4. 每增加 launch/config/URDF，立即确认 install 后路径：`ros2 pkg prefix 包名` 和 `ros2 pkg executables 包名`。
5. 添加 gtest 或 examples，让控制算法与 ROS 通信分开测试。
6. 提交前运行 `colcon test --packages-select 包名`、`colcon test-result --verbose`。

## 11. 面试背诵

**问：package.xml 和 CMakeLists.txt 有什么区别？**  
答：package.xml 是 ROS package 元信息和依赖声明，用于 rosdep、索引和运行环境；CMakeLists 定义 C++ 如何查找依赖、编译 target、链接库和安装文件。一个依赖若既在代码中使用又需运行，通常两边都要写；只写一边会导致构建、部署或下游使用失败。

**问：为什么 build 完还要 source？**  
答：build 只生成 install 空间，source 会设置 AMENT_PREFIX_PATH、PATH、LD_LIBRARY_PATH 等环境，使 ROS 能发现新 package、可执行文件、库和资源。多个 workspace 形成 overlay，后 source 的优先。

**问：ament_target_dependencies 与 target_link_libraries 如何选择？**  
答：ROS ament package 用 ament_target_dependencies，它继承包导出的 include、库和编译定义；普通 CMake target 常用 target_link_libraries。实际项目可以同时使用，例如 rclcpp 用前者、Eigen3::Eigen 用后者。
