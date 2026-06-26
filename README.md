# cpp_practice

这个仓库用于练习和整理机器人控制相关的 C++ 模块。目前最完整的纯 C++ 控制库位于：

```text
robot_control_ros2/src/robot_control
```

它虽然放在 ROS2 工作区中，并使用 `ament_cmake` 管理构建，但核心控制逻辑是普通 C++ 代码：状态、限幅、插值、轨迹缓冲和雅可比求解都可以作为控制算法库单独理解和测试。

## 解决什么问题

这个库解决的是机器人关节空间控制循环中的基础数据流问题：

1. 用统一的数据结构保存机器人当前或目标关节状态。
2. 从起点状态和目标状态生成离散轨迹点。
3. 在执行前检查位置和速度是否超过安全限制。
4. 将轨迹点放入缓冲区，按控制周期逐点取出执行。
5. 为后续速度/逆运动学控制提供雅可比伪逆和阻尼最小二乘求解工具。

换句话说，它不是完整机器人控制器，而是一组可以组合成控制循环的基础模块。

## 模块说明

### `RobotState`

文件：

```text
robot_control_ros2/src/robot_control/include/robot_state.hpp
robot_control_ros2/src/robot_control/src/robot_state.cpp
```

职责：

- 保存机器人自由度 `dof`。
- 保存关节位置 `q` 和关节速度 `dq`。
- 提供整体设置、按索引设置、读取、打印和比较接口。

典型用途：表示起点状态、目标状态、插值后的轨迹点或控制循环中的当前状态。

### `SafetyLimiter`

文件：

```text
robot_control_ros2/src/robot_control/include/safety_limiter.hpp
robot_control_ros2/src/robot_control/src/safety_limiter.cpp
```

职责：

- 设置每个关节的位置下限和上限。
- 设置最大速度限制。
- 检查 `RobotState` 或位置/速度数组是否安全。
- 对越界的位置和速度进行 clamp 限幅。

典型用途：轨迹点执行前的安全保护。

### `CubicInterpolator`

文件：

```text
robot_control_ros2/src/robot_control/include/cubic_interpolator.hpp
robot_control_ros2/src/robot_control/src/cubic_interpolator.cpp
```

职责：

- 设置起点状态、目标状态、运动时长 `duration` 和采样周期 `dt`。
- 生成三次插值轨迹 `calculate3Times()`。
- 生成五次插值轨迹 `calculate5Times()`。
- 按索引读取离散轨迹点。

典型用途：根据目标关节位置生成控制周期可执行的轨迹序列。

### `TrajectoryBuffer`

文件：

```text
robot_control_ros2/src/robot_control/include/trajectory_buffer.hpp
robot_control_ros2/src/robot_control/src/trajectory_buffer.cpp
```

职责：

- 使用队列保存多个 `RobotState` 轨迹点。
- 支持 push、pop、hasNext、size 和 clear。

典型用途：模拟控制器中的轨迹点缓存，每个控制周期取出一个点执行。

### `JacobianSolver`

文件：

```text
robot_control_ros2/src/robot_control/include/jacobian_solver.hpp
robot_control_ros2/src/robot_control/src/jacobian_solver.cpp
```

职责：

- 使用 Eigen 求解雅可比伪逆。
- 使用阻尼最小二乘法求解。
- 计算雅可比矩阵条件数。

典型用途：速度控制、冗余机械臂控制或逆运动学求解中的线性代数基础工具。

## 目录结构

```text
robot_control_ros2/
  src/
    robot_control/              # 纯 C++ 控制库
      include/                  # 头文件
      src/                      # 模块实现
      examples/                 # 测试程序和控制循环 demo
      CMakeLists.txt
      package.xml
    robot_control_ros2/         # ROS2 节点、消息和服务
    other_package/              # 消费自定义消息的示例包
```

## 编译纯 C++ 控制库

### 1. 准备环境

需要已经安装 ROS2、`colcon` 和 Eigen3。

如果使用 Ubuntu + ROS2 Humble，常见环境加载方式如下：

```bash
source /opt/ros/humble/setup.bash
```

如果你使用的是其他 ROS2 版本，把 `humble` 换成自己的版本名。

### 2. 进入 ROS2 工作区

```bash
cd ~/cpp_practice/robot_control_ros2
```

### 3. 编译 `robot_control`

只编译纯 C++ 控制库：

```bash
colcon build --packages-select robot_control
```

编译完成后，加载本工作区环境：

```bash
source install/setup.bash
```

### 4. 编译整个 ROS2 工作区

如果还想同时编译 ROS2 节点、自定义消息和其他示例包：

```bash
colcon build
source install/setup.bash
```

如果之前构建失败，建议先清理缓存再重新编译：

```bash
rm -rf build install log
colcon build
source install/setup.bash
```

## 运行测试程序

这些测试目前是普通可执行程序，不是 gtest。编译 `robot_control` 后，可以直接用 `ros2 run` 运行。

```bash
cd ~/cpp_practice/robot_control_ros2
source install/setup.bash
```

运行状态模块测试：

```bash
ros2 run robot_control robot_state_test
```

运行安全限幅测试：

```bash
ros2 run robot_control safety_limiter_test
```

运行轨迹缓冲测试：

```bash
ros2 run robot_control trajectory_buffer_test
```

运行插值器测试：

```bash
ros2 run robot_control cubic_interpolator_test
```

运行雅可比求解测试：

```bash
ros2 run robot_control jacobian_solver_test
```

运行控制循环 demo：

```bash
ros2 run robot_control control_loop_demo
```

也可以直接执行安装目录下的程序，例如：

```bash
./install/robot_control/lib/robot_control/control_loop_demo
```

## 控制循环 demo 流程

demo 文件：

```text
robot_control_ros2/src/robot_control/examples/control_loop_demo.cpp
```

流程如下：

1. 创建 `SafetyLimiter`，设置 6 自由度机器人限制。
2. 设置每个关节的位置范围为 `[-3.14, 3.14]`。
3. 设置最大关节速度为 `2.0`。
4. 创建起点关节位置：

```text
[0, 0, 0, 0, 0, 0]
```

5. 创建目标关节位置：

```text
[1, 15, -1, 1.5, 6, 21]
```

6. 用 `RobotState` 分别保存起点和目标。
7. 创建 `CubicInterpolator(6, 5.0, 0.04)`：

- `6` 表示 6 自由度。
- `5.0` 表示轨迹总时长 5 秒。
- `0.04` 表示每 0.04 秒生成一个轨迹点。

8. 调用 `calculate3Times()` 生成三次插值轨迹。
9. 将插值器生成的每个 `RobotState` 放入 `TrajectoryBuffer`。
10. 控制循环开始：

- 如果 buffer 里还有轨迹点，就取出下一个点。
- 用 `SafetyLimiter::isSafe()` 检查这个点是否安全。
- 如果安全，打印该状态。
- 如果不安全，打印停止提示，调用 `clampRobotState()` 把状态限制到安全范围内，再打印限幅后的状态。

这个 demo 故意设置了超出关节范围的目标值，例如 `15`、`6`、`21`，因此可以观察到安全限幅模块如何处理非法轨迹点。

## 最小使用流程

如果别人只想编译并运行纯 C++ 控制库，可以按下面步骤：

```bash
cd ~/cpp_practice/robot_control_ros2
source /opt/ros/humble/setup.bash
colcon build --packages-select robot_control
source install/setup.bash
ros2 run robot_control control_loop_demo
```

如果要逐个验证模块：

```bash
ros2 run robot_control robot_state_test
ros2 run robot_control safety_limiter_test
ros2 run robot_control trajectory_buffer_test
ros2 run robot_control cubic_interpolator_test
ros2 run robot_control jacobian_solver_test
```
