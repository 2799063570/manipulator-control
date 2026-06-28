# 第 2 周：Eigen、雅可比求解与项目化

## 本周目标

第 2 周在第 1 周控制库基础上，补充线性代数和简单运动学相关能力。核心是引入 Eigen，并实现雅可比相关求解，让项目从“状态与轨迹管理”推进到“速度映射和数值求解”。

## 实现内容

### JacobianSolver

文件：

```text
robot_control_ros2/src/robot_control/include/jacobian_solver.hpp
robot_control_ros2/src/robot_control/src/jacobian_solver.cpp
```

职责：

- 使用 Eigen 表示矩阵和向量
- 支持雅可比伪逆求解
- 支持阻尼最小二乘求解
- 支持计算雅可比矩阵条件数

## 技术链路

```mermaid
flowchart LR
    A["关节速度 dq"] --> B["Jacobian"]
    B --> C["末端速度 dx"]
    C --> D["伪逆 / 阻尼最小二乘"]
    D --> E["由目标末端速度反推关节速度"]
```

## 工程整理

这一周也把控制库继续项目化：

- 控制模块拆到 `include/` 和 `src/`
- 示例程序放到 `examples/`
- 通过 `CMakeLists.txt` 管理库和可执行程序
- 让模块测试可以通过 `ros2 run` 或安装目录执行

## 本周收获

雅可比矩阵的作用是连接关节空间速度和末端空间速度：

```text
dx = J(q) * dq
```

如果已知目标末端速度 `dx`，可以通过伪逆或阻尼最小二乘反求关节速度 `dq`。阻尼最小二乘可以缓解奇异点附近的数值不稳定，条件数可以帮助观察矩阵是否接近奇异。

更重要的是，这些求解逻辑仍然保持在纯 C++ 层，不直接依赖 ROS2。后续无论接 ROS2 topic、MoveIt2，还是 ros2_control，都可以复用这部分算法模块。

## 当前限制

- 当前雅可比求解模块偏数学工具，没有绑定具体机械臂模型
- 还没有完整正逆运动学链路
- 没有根据 URDF 自动生成雅可比
- 主要用于建立速度控制和数值稳定性的基础认知
