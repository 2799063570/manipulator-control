---
title: OMPL 与 MoveIt 规划管线学习手册
tags: [MoveIt2, OMPL, RRTConnect, ros2_control, Gazebo, AUBO-i5]
---

# OMPL 与 MoveIt 规划管线学习手册

> [!abstract] 目标
> 学完后，你应该能在 AUBO 项目中定位：规划失败是 **IK、碰撞、OMPL、时间参数化，还是控制器执行** 的问题；而不是一开始就研究或重写 RRT 算法。

## 先记住这一句话

> [!important] OMPL 只找无碰撞的几何路径；控制器只执行带时间的关节轨迹。
>
> 所以 `Plan` 成功而 `Execute` 失败时，通常不要先怀疑 OMPL。

`move_group` 是总调度者，`PlanningSceneMonitor` 负责维护机器人和环境，IK 插件负责位姿到关节状态的转换，OMPL 负责搜索几何路径，MoveIt 碰撞环境负责判断状态是否有效，规划流水线负责前处理和后处理，最后得到可执行轨迹。

无论在 ROS 1 MoveIt 还是 ROS 2 MoveIt 2 中，`move_group` 的核心规划流程基本相同。它接收关节目标、末端位姿目标以及路径约束等规划请求，通过 `PlanningSceneMonitor` 获取当前机器人状态和环境状态，并将规划请求与当前规划场景交给 `PlanningPipeline`。规划流水线首先对请求进行验证和预处理，然后调用 OMPL 等规划插件完成几何路径搜索。在目标或约束以笛卡尔位姿表示时，规划过程中可能调用 IK 求解器生成满足约束的关节状态。OMPL 在扩展和连接路径时，通过 MoveIt 提供的状态有效性接口反复进行关节边界、碰撞和路径约束检查。规划成功后，系统对几何路径进行验证、简化、插值和时间参数化，最终生成可供控制器执行的 `RobotTrajectory`。

```text
客户端 / RViz（负责：指定目标、规划参数、是否执行）
          │
          ▼
move_group（负责：统一接收请求、组织各组件）
          │
          ▼
PlanningSceneMonitor（负责：当前机械臂状态 + 当前环境状态）
          │
          ▼
PlanningPipeline（负责：前处理 → 调用规划器 → 后处理）
          │
          ├──────────────┐
          ▼              ▼
Kinematics Plugin      OMPL Plugin
负责 IK / FK           负责路径搜索
                         │
                         ▼
                  State Validity Callback
                         │
                         ▼
                   PlanningScene
                   负责碰撞与约束检查
                         │
                         ▼
                   Geometric Path
                         │
                         ▼
              时间参数化和结果验证
                         │
                         ▼
                   RobotTrajectory
                         │
                         ▼
              FollowJointTrajectory
                         │
                         ▼
              ros_control / ros2_control
```

```mermaid
flowchart LR
  A["目标<br/>关节角或 TCP 位姿"] --> B["MoveGroup<br/>接收 MotionPlanRequest"]
  B --> C["Request adapters<br/>修正/检查起点和约束"]
  C --> D["IK + Planning Scene<br/>可达性、限位、碰撞"]
  D --> E["OMPL<br/>搜索无碰撞几何路径"]
  E --> F["Response adapters<br/>简化、验证、时间参数化"]
  F --> G["RobotTrajectory<br/>q、dq、ddq、time_from_start"]
  G --> H["FollowJointTrajectory"]
  H --> I["ros2_control<br/>Fake / Gazebo / 真机"]
```

## 每层到底负责什么

| 组件 | 它负责 | 它不负责 | 典型失败表现 |
| --- | --- | --- | --- |
| IK 插件 | TCP 位姿 → 可行关节角 | 避障、控制电机 | `No IK solution`、目标不可达。 |
| Planning Scene | 当前状态、障碍物、碰撞与约束 | 搜索路径、控制电机 | 起点/目标处于碰撞。 |
| OMPL | 在配置空间搜索无碰撞路径 | 时间、速度、加速度 | 超时、狭窄通道无解。 |
| 时间参数化 | 为路径添加速度、加速度和时间 | 绕障碍物 | 轨迹时间或限制不合法。 |
| 轨迹控制器 | 跟踪 `JointTrajectory` 并反馈状态 | IK、碰撞、重新规划 | action 拒绝、跟随误差、超时。 |

## 当前项目：先看这 6 个文件

```text
aubo_i5_moveit_config/
  config/ompl_planning.yaml       # OMPL pipeline、planner、adapters
  config/kinematics.yaml          # IK 插件与超时
  config/joint_limits.yaml        # 速度、加速度、缩放
  config/moveit_controllers.yaml  # MoveIt → action 映射
  config/ros2_controllers.yaml    # JointTrajectoryController
  launch/move_group.launch.py     # 把所有配置传给 move_group
```

> [!warning] 先补齐配置
> 当前 AUBO 包还没有 `config/ompl_planning.yaml`。新增该文件后，`MoveItConfigsBuilder(...).to_moveit_configs()` 会按标准命名自动读取它；在 launch 中显式指定 `pipelines=["ompl"]` 只是让意图更明确。

## 7 天学习路线

每次实验只改一个变量。每次结束保存：YAML diff、RViz 截图、`move_group` 日志和结果表。

### Day 1 · 分清 Plan 与 Execute

**任务**：Fake 模式下设置一个可达关节目标，依次点 `Plan` 和 `Execute`。

```bash
ros2 control list_controllers
ros2 action list | grep follow_joint_trajectory
ros2 topic echo /joint_states --once
```

- [ ] `Plan` 后能看到轨迹预览，但机械臂尚未执行。
- [ ] `Execute` 后确认轨迹控制器 action 被调用。
- [ ] 能说明 `RobotTrajectory` 和 `JointTrajectory` 的区别。

阅读：[MoveIt Motion Planning 概念](https://moveit.picknik.ai/main/doc/concepts/motion_planning.html)。

### Day 2 · 让碰撞真正影响规划

**任务**：在 RViz Planning Scene 中添加一个盒子，对同一起点、同一目标对比“有/无障碍物”。

| 场景 | 要观察什么 |
| --- | --- |
| 空场景 | 基线规划时间和路径形状。 |
| 路径中间有盒子 | 是否绕行、路径是否变长。 |
| 目标位置有盒子 | 目标是否直接被判为碰撞。 |
| 狭窄通道 | 成功率是否随规划时间变化。 |

- [ ] 能区分：起点碰撞、目标碰撞、路径无通道。

阅读：[MoveIt 规划绕障碍物](https://moveit.picknik.ai/main/doc/tutorials/planning_around_objects/planning_around_objects.html)。

> [!note] `longest_valid_segment_fraction`
> 该参数定义用于碰撞检测的机器人运动离散化方式，并极大影响基于 OMPL 的规划性能和可靠性。可以把两个节点之间的边理解为一段连续运动：如果离散检查太稀，中间可能漏掉碰撞；如果离散检查太密，规划会变慢。它不是“越小越好”的参数。

### Day 3 · IK、关节空间与 TCP 目标

**任务**：把末端交互标记放到明显不可达的位置，再换成关节目标。

- [ ] 能解释：关节目标已给出构型，通常不需要先求 IK；TCP 目标需要求出满足位姿的关节构型。
- [ ] 能区分：IK 无解、IK 有解但碰撞、IK 有解但 OMPL 超时。
- [ ] 阅读并解释当前 `kinematics.yaml` 的 solver、timeout、search resolution。

```yaml
arm:
  kinematics_solver: kdl_kinematics_plugin/KDLKinematicsPlugin
  kinematics_solver_search_resolution: 0.005
  kinematics_solver_timeout: 0.05
  kinematics_solver_attempts: 3
```

阅读：[MoveIt 配置中的 IK](https://moveit.picknik.ai/main/doc/how_to_guides/moveit_configuration/moveit_configuration_tutorial.html)。

> [!example] `kinematics.yaml` 示例
>
> ```yaml
> left_arm:  # 规划组的名称
>   kinematics_solver: kdl_kinematics_plugin/KDLKinematicsPlugin # 插件名称
>   kinematics_solver_search_resolution: 0.005 # 步长/分辨率
>   kinematics_solver_timeout: 0.05  # 规划时间
>   kinematics_solver_attempts: 3  # 最大尝试次数
>
> right_arm:
>   kinematics_solver: kdl_kinematics_plugin/KDLKinematicsPlugin
>   kinematics_solver_search_resolution: 0.005
>   kinematics_solver_timeout: 0.05
>   kinematics_solver_attempts: 3
> ```

### Day 4 · 理解 RRTConnect 的行为

**任务**：同一场景重复规划 10 次，记录成功次数、规划时间和路径长度。

RRTConnect 从起点和目标分别扩展随机树，尝试把两棵树连接。它追求快速获得**一条可行路**，因此：

- 每次随机样本不同，路径可能不同；
- TCP 目标可能有多个 IK 解；
- 第一条解不保证最短；
- 窄通道或复杂障碍物会降低成功率。

- [ ] 将允许规划时间分别改为 `0.1 s` 和 `5 s`，仅比较成功率和耗时。
- [ ] 不修改 `range`、`goal_bias`，先理解基线行为。

阅读：[OMPL Primer](https://ompl.kavrakilab.org/OMPL_Primer.pdf)，重点读采样、局部连接和树/路线图。

### Day 5 · 学会读 `ompl_planning.yaml`

配置结构可以这样理解：

```text
OMPL pipeline
├── planning_plugins     用哪个 MoveIt 规划插件
├── request_adapters     规划前检查/修复
├── response_adapters    路径验证与时间参数化
├── planner_configs      RRT、PRM 等算法模板
└── arm                  规划组可用的算法和空间参数
```

最小示例：

```yaml
planning_plugins: [ompl_interface/OMPLPlanner]

request_adapters:
  - default_planning_request_adapters/ResolveConstraintFrames
  - default_planning_request_adapters/ValidateWorkspaceBounds
  - default_planning_request_adapters/CheckStartStateBounds
  - default_planning_request_adapters/CheckStartStateCollision

response_adapters:
  - default_planning_response_adapters/AddTimeOptimalParameterization
  - default_planning_response_adapters/ValidateSolution

planner_configs:
  RRTConnectkConfigDefault:
    type: geometric::RRTConnect
    range: 0.0

arm:
  default_planner_config: RRTConnectkConfigDefault
  planner_configs: [RRTConnectkConfigDefault]
  longest_valid_segment_fraction: 0.01
```

> [!example] 你补充的完整结构示例
>
> ```yaml
> planning_plugins:
>   - ompl_interface/OMPLPlanner           # MoveIt规划插件
>
> # 规划前处理
> # 解析目标约束使用的坐标系和子坐标系
> # 检查或补充规划请求中的工作空间边界
> # 检查起始关节状态是否超出
> # 检查起始状态是否存在
> request_adapters:
>   - default_planning_request_adapters/ResolveConstraintFrames
>   - default_planning_request_adapters/ValidateWorkspaceBounds
>   - default_planning_request_adapters/CheckStartStateBounds
>   - default_planning_request_adapters/CheckStartStateCollision
>
> # 规划后处理：时间参数化、重新检查规划结果、发布规划路径供 RViz 显示
> response_adapters:
>   - default_planning_response_adapters/AddTimeOptimalParameterization
>   - default_planning_response_adapters/ValidateSolution
>   - default_planning_response_adapters/DisplayMotionPath
>
> start_state_max_bounds_error: 0.1
>
> # 定义算法模板
> planner_configs:
>   RRTConnectkConfigDefault:
>     type: geometric::RRTConnect
>     range: 0.0
>
>   RRTkConfigDefault:
>     type: geometric::RRT
>     range: 0.0
>     goal_bias: 0.05
>
>   PRMkConfigDefault:
>     type: geometric::PRM
>     max_nearest_neighbors: 10
>
> arm:
>   planner_configs:
>     - RRTConnectkConfigDefault
>     - RRTkConfigDefault
>     - PRMkConfigDefault
>
>   longest_valid_segment_fraction: 0.01
> ```

OMPL 本身并不了解 URDF、机器人连杆或 MoveIt 规划场景；MoveIt 负责把机器人状态空间、碰撞检测和约束检查接口提供给 OMPL。

| 参数 | 直觉 | 不当设置的后果 |
| --- | --- | --- |
| `planning_time` | 最长搜索时间 | 太短易失败，太长反馈慢。 |
| `range` | 树的一次扩展步长 | 太大难进窄缝，太小搜索慢。通常保持 `0.0` 自动估计。 |
| `goal_bias` | 向目标采样的概率 | 太高探索不足，太低接近目标慢。 |
| `longest_valid_segment_fraction` | 一条路径边上的碰撞检查密度 | 太大可能漏碰撞，太小会显著变慢。 |

> [!tip] 调参纪律
> 复制基线 YAML；一次只改一个参数；每组至少跑 5 次；记录成功率、平均时间和路径长度。一次偶然成功不是优化。

阅读：[MoveIt OMPL Planner 配置](https://moveit.picknik.ai/main/doc/examples/ompl_interface/ompl_interface_tutorial.html)。

### Day 6 · Request/Response adapters 与时间参数化

**任务**：在 `move_group.launch.py` 找到 adapters，按顺序写出职责。

```text
Request adapters
  ResolveConstraintFrames → ValidateWorkspaceBounds
  → Fix/Check Start State → OMPL

Response adapters
  OMPL path → AddTimeOptimalParameterization
  → ValidateSolution → display / execute
```

- [ ] 理解 `FixStartStateBounds`、`FixStartStateCollision` 的适用范围。
- [ ] 对比不同 `joint_limits.yaml` 后的轨迹持续时间。
- [ ] 能解释为什么时间参数化不负责避障。

阅读：[MoveIt Planning Adapters](https://moveit.picknik.ai/main/doc/examples/planning_adapters/planning_adapters_tutorial.html)。

### Day 7 · 用 Gazebo 验证执行边界

**任务**：使用同一套 MoveIt 和控制器配置，在 Fake 与 Gazebo 执行同一安全关节目标。

```text
Fake 成功、Gazebo 失败
  → 惯量 / 碰撞网格 / 关节阻尼 / 仿真时间 / 控制器接口

Plan 成功、action 不存在或被拒绝
  → moveit_controllers.yaml / ros2_controllers.yaml / 控制器状态

Plan 就失败
  → IK / 当前状态 / 碰撞 / 约束 / OMPL 时间
```

- [ ] 先检查日志、`/joint_states` 和 `ros2 control list_controllers`，不先换 planner。
- [ ] 写出一张自己的故障树并保存到项目笔记。

## 三个必做小项目

| 项目 | 交付物 | 目的 |
| --- | --- | --- |
| 规划成功率基线 | 空场景/障碍物/窄通道，各 10 次的结果表 | 用数据理解随机采样规划。 |
| 碰撞离散化对比 | 默认与更细检查密度的耗时、路径和成功率 | 理解性能与漏检风险的权衡。 |
| Plan/Execute 排障 | 不可达 TCP、控制器未激活、限位过低的三份记录 | 建立分层诊断能力。 |

## 下一阶段要不要学其他规划器？

| 你的需求 | 下一步学什么 |
| --- | --- |
| 点到点、直线、圆弧等工业确定性运动 | Pilz Industrial Motion Planner。 |
| 优化已有路径的平滑度/代价 | CHOMP 或 STOMP。 |
| 末端必须走直线/平面或保持姿态 | OMPL Constrained Planning。 |
| 环境稳定、重复执行大量查询 | PRM / LazyPRM 路线图。 |

先完成 RRTConnect、碰撞、时间参数化和控制器排障，再换规划器。

## 完成标准：6 个自测题

- [ ] 为什么 TCP 目标通常要 IK，而关节目标通常不需要？
- [ ] 为什么相同请求会得到不同的 RRTConnect 路径？
- [ ] `longest_valid_segment_fraction` 的安全/性能权衡是什么？
- [ ] `Plan` 成功、`Execute` 失败时，优先检查哪三层？
- [ ] MoveIt、OMPL、时间参数化、轨迹控制器的职责边界是什么？
- [ ] 为什么 Gazebo 通过仍不能直接接入真机？

## 自测题参考答案

### 1. 为什么 TCP 目标通常要 IK，而关节目标通常不需要？

MoveIt 的 OMPL 规划通常在机器人关节空间中进行。关节目标已经直接给出了目标关节构型，因此通常不需要 IK；末端目标位于笛卡尔任务空间，需要通过 IK 或约束采样将其转换为满足末端位姿约束的关节空间状态，然后才能进行关节空间路径搜索。

### 2. 为什么相同的请求会得到不同 RRTConnect 路径？

RRTConnect 是随机采样型双向树规划器。每次规划时随机样本序列可能不同，导致搜索树的扩展方向、连接位置和首先找到的可行路径不同。另外，末端位姿目标可能产生不同 IK 解，路径简化过程也可能引入随机性，因此相同请求不一定产生完全相同的路径。

### 3. `longest_valid_segment_fraction` 影响什么安全/性能权衡？

`longest_valid_segment_fraction` 控制 OMPL 对状态空间边进行碰撞检测时的离散分辨率。值越小，中间碰撞检查越密，漏检风险越低，但碰撞检测开销和规划时间越大；值越大，规划速度可能提高，但可能漏掉两个离散检查点之间的碰撞。

### 4. “Plan 成功、Execute 失败”时，最先检查哪三个对象？

第一，规划结果中的 `JointTrajectory` 是否具有正确的关节名称、时间戳和限制；第二，MoveIt 配置的控制器及 `/follow_joint_trajectory` action 是否存在且处于 active；第三，`/joint_states` 和硬件状态反馈是否正常，实际机器人是否能够跟踪轨迹并满足路径和目标容差。

### 5. MoveIt、OMPL、时间参数化、`JointTrajectoryController` 各自的边界是什么？

MoveIt 是运动规划与执行的总体集成框架，负责机器人模型、状态、规划场景、IK、碰撞检测、规划流水线和控制器连接；OMPL 是其中的几何路径规划器，负责在关节状态空间搜索无碰撞路径；时间参数化是路径后处理模块，负责依据速度和加速度限制给路径添加时间信息；`JointTrajectoryController` 是执行层控制器，负责按时间插值并跟踪关节轨迹，将期望状态转换为硬件命令，但不负责 IK、路径搜索或碰撞规避。

### 6. 为什么 Gazebo 通过不代表可以直接接真机？

Gazebo 验证的是控制与规划流程在仿真模型中的正确性，而真机还存在动力学模型误差、摩擦与齿隙、传感器噪声、通信延迟与抖动、驱动器限制、硬件接口差异、标定误差以及安全约束。因此仿真通过后仍需进行硬件接口验证、低速空载测试、限位与急停测试、通信异常测试，并逐步提高速度和负载。

## 参考与关联笔记

1. [MoveIt Motion Planning](https://moveit.picknik.ai/main/doc/concepts/motion_planning.html)
2. [MoveIt OMPL Planner 配置](https://moveit.picknik.ai/main/doc/examples/ompl_interface/ompl_interface_tutorial.html)
3. [MoveIt Planning Adapters](https://moveit.picknik.ai/main/doc/examples/planning_adapters/planning_adapters_tutorial.html)
4. [OMPL Primer](https://ompl.kavrakilab.org/OMPL_Primer.pdf)
5. [[22-MoveIt与ros2_control加载执行全流程]]
6. [[30-AUBO参考驱动-仿真到真机实施手册]]
7. [[13-实验3-AUBO-i5建模-Gazebo与MoveIt配置]]
