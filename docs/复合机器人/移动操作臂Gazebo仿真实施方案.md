---
title: 移动操作臂 Gazebo 仿真实施方案
tags: [ROS2, Jazzy, Gazebo, Nav2, SLAM, MoveIt2, AUBO-i5, 移动操作臂]
---

# 移动操作臂 Gazebo 仿真实施方案

## 1. 项目目标与边界

本项目构建一套“移动机器人底盘 + AUBO i5 机械臂”的 ROS 2 Jazzy / Gazebo Sim 仿真系统。机器人配备 2D 激光雷达、IMU 和轮式里程计，具备建图、定位、导航与机械臂抓取能力。

一期以可重复、可验证的仿真闭环为目标：

1. 在 Gazebo 中生成完整复合机器人。
2. 控制底盘行驶，并获得轮式里程计、IMU 与完整 TF。
3. 使用 `slam_toolbox` 建图并保存地图。
4. 使用 AMCL 在已知地图中定位，使用 Nav2 导航到目标工位。
5. 使用 MoveIt2 控制 AUBO i5 完成预抓取、抓取、抬升和放置。
6. 使用状态机或行为树完成“收臂 → 导航 → 停车 → 抓取 → 收臂 → 导航 → 放置”的任务闭环。

一期安全约束：**底盘移动时机械臂必须处于收纳姿态；机械臂规划和执行时底盘必须静止。** 全身协同规划、移动中操作和视觉抓取属于后续阶段。

## 2. 底盘方案决策

### 2.1 推荐方案：四轮差速底盘

建议一期采用左右各两轮的四轮差速底盘，而不是直接使用麦克纳姆轮。

| 对比项 | 四轮差速 | 麦克纳姆轮 |
| --- | --- | --- |
| AUBO i5 搭载稳定性 | 高，控制和接触模型较简单 | 较低，对重心和地面要求高 |
| Gazebo 调试成本 | 低 | 高，需处理滚子接触和侧向滑移 |
| 里程计与 EKF | 成熟直接 | 需要全向运动学与额外标定 |
| Nav2 参数 | 标准非全向配置 | 需要全向局部控制器配置 |
| 适用场景 | 搬运、巡检、工位间移动 | 需横移贴近货架/工位 |

当项目明确需要在狭窄工位前横向微调时，再将底盘替换为四麦克纳姆轮，并增加 `vy` 控制、全向运动学和全向 Nav2 参数。

### 2.2 初始机械尺寸建议

下表是仿真初始参数，实际设计必须以 AUBO i5 的官方尺寸、质量、额定载荷、末端工具质量和工作范围为准重新核算。

| 参数 | 建议初值 |
| --- | --- |
| 底盘长 × 宽 | 0.90–1.10 m × 0.65–0.80 m |
| 离地间隙 | 0.05–0.08 m |
| 轮径 | 0.15–0.20 m |
| 左右轮距 | 0.55–0.70 m |
| 机械臂底座高度 | 尽可能低，并保证安装刚性 |
| AUBO 安装位置 | 底盘中心略靠后，保留前伸空间 |
| 雷达高度 | 0.25–0.45 m，避开机械臂底座遮挡 |
| IMU 位置 | 靠近整机质心、刚性固定 |

机械臂最大前伸、最大侧伸和携带最大负载时，整机质心投影应位于车轮支撑多边形内。Gazebo 中必须为底盘、轮子、机械臂和负载填写合理的质量、惯量、摩擦和阻尼参数。

## 3. 现有仓库可复用基础

当前仓库已有两条可复用链路：

| 现有位置 | 可复用能力 |
| --- | --- |
| `robot_move_control/src/fishbot_gazebo` | Gazebo Sim、差速控制、激光雷达、SLAM Toolbox、Nav2 启动入口 |
| `aubo_i5_ros2_control/src` | AUBO i5 URDF/Xacro、Gazebo、`ros2_control`、MoveIt2 配置 |

最终项目不能把两套独立 Gazebo 仿真直接同时启动。正确结构是：**一个复合 URDF/Xacro、一个 Gazebo 世界、一个 `controller_manager`、多个 ROS 2 控制器。**

## 4. 总体软件架构

```text
任务层：行为树 / 状态机
  └─ 定位检查、收臂、导航、抓取、放置、故障恢复

规划层：
  ├─ Nav2：全局路径、局部避障、恢复行为
  └─ MoveIt2：机械臂轨迹规划、碰撞检测、抓取

控制层：
  ├─ diff_drive_controller：四轮差速底盘
  ├─ joint_trajectory_controller：AUBO i5 六轴
  └─ gripper_controller：夹爪

状态估计层：
  ├─ 轮式里程计
  ├─ IMU
  ├─ robot_localization EKF
  ├─ slam_toolbox：建图
  └─ AMCL：已知地图定位

仿真层：Gazebo Sim + gz_ros2_control + ros_gz_bridge
```

### 4.1 TF 树

TF 必须稳定保持为：

```text
map → odom → base_footprint → base_link
                              ├─ lidar_link
                              ├─ imu_link
                              └─ arm_base_link → AUBO 六轴 → tool0
```

- `slam_toolbox` 建图阶段或 `amcl` 定位阶段发布 `map → odom`。
- `robot_localization` EKF 发布 `odom → base_footprint`。
- `robot_state_publisher` 发布机器人本体固定关节和机械臂关节 TF。
- 禁止多个节点发布同一条 TF。例如启用 EKF 后，底盘控制器不能再发布 `odom → base_footprint`。

## 5. 新功能包与目录规划

建议保留原有功能包不动，在仓库根目录新建 `mobile_manipulator_ros2/src`：

```text
mobile_manipulator_ros2/src/
├─ mobile_manipulator_description/
│  ├─ urdf/
│  │  ├─ mobile_base.xacro
│  │  ├─ sensors.xacro
│  │  ├─ aubo_mount.xacro
│  │  └─ mobile_manipulator.urdf.xacro
│  ├─ meshes/
│  └─ rviz/
├─ mobile_manipulator_gazebo/
│  ├─ worlds/
│  ├─ config/controllers.yaml
│  └─ launch/simulation.launch.py
├─ mobile_manipulator_navigation/
│  ├─ config/ekf.yaml
│  ├─ config/slam.yaml
│  ├─ config/amcl.yaml
│  ├─ config/nav2.yaml
│  └─ launch/
├─ mobile_manipulator_moveit_config/
│  ├─ config/
│  └─ launch/
├─ mobile_manipulator_task/
│  ├─ src/
│  ├─ behavior_trees/
│  └─ config/
└─ mobile_manipulator_bringup/
   └─ launch/
```

统一构建命令：

```bash
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install --base-paths \
  aubo_i5_ros2_control/src \
  robot_move_control/src \
  mobile_manipulator_ros2/src
source install/setup.bash
```

## 6. 详细实施步骤

### 阶段 0：环境复现与基线确认

1. 固定 Ubuntu 24.04、ROS 2 Jazzy、Gazebo Sim、MoveIt2、Nav2 的版本组合。
2. 单独启动当前 Fishbot 仿真，检查 `/scan`、`/odom`、`/tf` 和底盘命令。
3. 单独启动当前 AUBO i5 Gazebo + MoveIt2，检查六轴关节状态、控制器和轨迹执行。
4. 每次启动保存以下基线信息：

```bash
ros2 node list
ros2 topic list -t
ros2 control list_controllers
ros2 run tf2_tools view_frames
```

5. 所有仿真节点统一使用 `use_sim_time:=true`。
6. 只使用 Gazebo Sim 与 `gz_ros2_control`，不要混用 Gazebo Classic 插件。

验收标准：两个已有项目均能独立运行；无重复 `/joint_states`、重复 TF 或重复 controller manager。

### 阶段 1：移动底盘与传感器建模

1. 创建参数化的底盘 Xacro，定义 `base_footprint`、`base_link`、四个轮子及其连续转动关节。
2. 使用简化碰撞体，并为底盘、轮子、机械臂安装位填写可信的质量、重心与惯量。
3. 建立 `lidar_front_link`、`lidar_rear_link`、`imu_link` 与 `arm_base_link`，固定坐标系命名和安装位姿。
4. 在 Gazebo 中添加 IMU 和前后两颗 2D 激光雷达；前雷达作为一期 SLAM 主传感器，前后雷达共同用于 Nav2 局部避障。
5. 在收纳姿态和典型抓取姿态下检查激光遮挡范围，确认前后行驶方向没有不可接受的盲区。
6. 在 RViz 和空 Gazebo 世界验证模型、TF、传感器话题、接触稳定性和激光扫描方向。

详细建模流程、Xacro 示例、双雷达决策与桥接配置见：[[01-移动底盘与传感器建模]]。

验收标准：Xacro/URDF 检查通过；Gazebo 模型稳定；传感器均有有效 TF 和持续输出；收纳姿态下不存在影响前后行驶的严重盲区。

### 阶段 2：AUBO i5 与底盘集成

1. 复用 `robot_description` 中已验证的 AUBO i5 Xacro 和网格。
2. 在 `aubo_mount.xacro` 中定义固定安装链：

```text
base_link → arm_base_link → AUBO base
```

3. 根据 CAD 或实际测量值填写机械臂安装位姿。
4. 将底盘顶板、雷达、电池箱、支撑立柱、夹爪和工作台加入碰撞模型。
5. 定义 `stowed` 收纳姿态和 `ready_to_pick` 预抓取姿态。
6. 在 MoveIt2 检查机械臂典型姿态是否与底盘、地面或工作台发生碰撞。

验收标准：MoveIt2 可以从收纳姿态规划到预抓取姿态，且不会穿过底盘或工作台。

### 阶段 3：统一 Gazebo 与 ros2_control

复合机器人只保留一个 `gz_ros2_control` 插件和一个 `/controller_manager`。推荐控制器：

```yaml
joint_state_broadcaster:
  type: joint_state_broadcaster/JointStateBroadcaster

base_controller:
  type: diff_drive_controller/DiffDriveController

arm_controller:
  type: joint_trajectory_controller/JointTrajectoryController

gripper_controller:
  type: position_controllers/GripperActionController
```

实施顺序：

1. 在复合 URDF 的 `ros2_control` 块中声明四个轮子、六个机械臂关节和夹爪的命令/状态接口。
2. 新建统一的 `controllers.yaml`。
3. 启动 `joint_state_broadcaster`。
4. 启动 `base_controller` 并完成遥控行驶。
5. 启动 `arm_controller` 并通过 `FollowJointTrajectory` 执行关节轨迹。
6. 启动夹爪控制器。
7. Gazebo 物理步长初始设为 0.001–0.01 s，控制器更新频率初始设为 100 Hz。
8. 调整轮胎摩擦、阻尼和转动惯量，解决抖动、打滑或穿模。

验收标准：所有控制器均为 `active`；底盘和机械臂同时存在但不抢控制器；执行机械臂轨迹时底盘不漂移或翻倒。

### 阶段 4：轮式里程计、IMU 与 EKF

推荐数据链：

```text
/wheel/odom + /imu/data
        ↓
robot_localization EKF
        ↓
/odometry/filtered + odom → base_footprint
```

1. 配置 `diff_drive_controller` 的四个轮名、轮半径、轮距、速度限制、加速度限制和命令超时。
2. 让控制器发布原始轮式里程计，但关闭其 `odom → base_footprint` TF 发布。
3. 配置 EKF：输入轮式里程计的速度信息与 IMU 角速度/朝向，设置 `world_frame=odom`。
4. 由 EKF 独占发布 `odom → base_footprint`。
5. 比较 `/wheel/odom` 和 `/odometry/filtered`：滤波输出应更平滑，静止时速度接近零。

验收标准：机器人绕场地行驶一圈时 TF 不断链，姿态不明显振荡。

### 阶段 5：SLAM 建图

1. 建立建图启动文件，包含 Gazebo、控制器、EKF、`slam_toolbox` 和 RViz。
2. `slam_toolbox` 初始参数：

```text
base_frame = base_footprint
odom_frame = odom
map_frame = map
scan_topic = /scan
resolution = 0.05 m
```

3. 遥控机器人覆盖外墙、内部障碍物、走廊、抓取工位和起点。
4. 保存地图：

```bash
ros2 run nav2_map_server map_saver_cli -f maps/lab_map
```

5. 检查地图墙体是否闭合、障碍物是否重影，以及 `map → odom` 是否只由 SLAM 发布。

验收标准：形成稳定的 `lab_map.yaml` 与 `lab_map.pgm`，重复建图时主结构一致。

### 阶段 6：AMCL 与 Nav2

1. 已知地图导航时停止 SLAM，启动 `map_server`、AMCL、Nav2、EKF 和 Gazebo。
2. Nav2 的 footprint 使用**机械臂收纳后的整机外轮廓**，不要只按轮距配置圆形半径。
3. global costmap 使用静态地图和膨胀层；local costmap 使用激光障碍层和膨胀层。
4. 差速底盘优先使用 Regulated Pure Pursuit；初始最大线速度设为 0.2–0.35 m/s，最大角速度设为 0.6–1.0 rad/s。
5. 速度链路固定为：

```text
Nav2 /cmd_vel_nav
  → twist_mux
  → /base_controller/cmd_vel_unstamped
  → diff_drive_controller
```

6. 在 `twist_mux` 中设置急停最高优先级、遥控高优先级、Nav2 中优先级、测试节点低优先级。
7. 设置起点、货架前、抓取工位、放置工位、回零点五个目标。

验收标准：连续 20 次导航至少成功 18 次，无碰撞、无持续震荡、无 TF 超时。

### 阶段 7：MoveIt2 复合规划场景

1. 基于现有 `aubo_i5_moveit_config` 创建复合机器人 MoveIt2 配置。
2. 一期只定义 `arm` 和 `gripper` Planning Group，不将底盘纳入 MoveIt 全身规划。
3. 将底盘、雷达、电池箱和工作台加入 Planning Scene 的碰撞对象。
4. 配置关节限位、速度/加速度缩放、`arm_controller/follow_joint_trajectory`、夹爪 Action 和末端执行器坐标系。
5. 在规划场景中加入台面、货架、待抓物体和放置箱。
6. 实现姿态流程：

```text
stowed → ready_to_pick → pre_grasp → grasp → lift
       → place → retreat → stowed
```

验收标准：抓取、抬升、放置均可规划；碰撞检测能阻止机械臂穿过底盘和工位。

### 阶段 8：任务编排与互锁

建立状态机或 Behavior Tree：

```text
INIT
 → CHECK_LOCALIZATION
 → STOW_ARM
 → NAVIGATE_TO_PICK
 → WAIT_BASE_STOPPED
 → PLAN_PREGRASP
 → GRASP
 → RETREAT_AND_STOW
 → NAVIGATE_TO_PLACE
 → PLACE
 → RETURN_HOME
```

每个状态必须具有成功条件、超时、取消逻辑、重试次数、失败恢复和日志记录。

必须实现以下互锁：

1. 机械臂不在 `stowed` 时，禁止 Nav2 驱动底盘。
2. 底盘线速度或角速度高于阈值时，禁止执行机械臂轨迹。
3. AMCL 定位质量低、TF 不完整或激光失效时，禁止导航。
4. 轨迹规划失败后，禁止执行上一条缓存轨迹。
5. 任务取消后，底盘速度归零，机械臂停止并保持安全状态。

验收标准：完整取送放任务能自动完成；故意制造定位丢失或轨迹失败时，系统安全停止或进入恢复状态。

## 7. 启动入口规划

```bash
# 复合机器人、Gazebo 与控制器
ros2 launch mobile_manipulator_gazebo simulation.launch.py

# 建图
ros2 launch mobile_manipulator_navigation slam.launch.py

# 已知地图定位与导航
ros2 launch mobile_manipulator_navigation navigation.launch.py \
  map:=/absolute/path/lab_map.yaml

# Gazebo + MoveIt2
ros2 launch mobile_manipulator_moveit_config demo_gazebo.launch.py

# 完整任务演示
ros2 launch mobile_manipulator_bringup full_demo.launch.py \
  map:=/absolute/path/lab_map.yaml
```

仿真、建图、导航、MoveIt2 与任务层必须可独立启动和验证，不应全部强耦合在一个 launch 文件中。

## 8. 六周实施排期

| 周期 | 目标 | 交付物 |
| --- | --- | --- |
| 第 1 周 | 复现已有 Fishbot 和 AUBO 仿真；冻结尺寸和坐标系 | 架构图、参数表、基线记录 |
| 第 2 周 | 四轮差速、传感器、复合 URDF | RViz 完整模型 |
| 第 3 周 | 统一 Gazebo 与 `ros2_control` | 可遥控复合机器人 |
| 第 4 周 | EKF、SLAM、地图保存 | 可复现地图 |
| 第 5 周 | AMCL、Nav2、多目标导航 | 连续导航演示 |
| 第 6 周 | MoveIt2、抓取流程、任务状态机 | 取送放完整演示 |

## 9. 最终验收清单

- [ ] 复合 URDF/Xacro 可展开，坐标系和惯量正确。
- [ ] Gazebo 中仅有一个世界、一个机器人实体、一个 controller manager。
- [ ] 四轮底盘、六轴机械臂和夹爪控制器均为 `active`。
- [ ] `/joint_states`、激光、IMU、里程计与 TF 持续发布。
- [ ] EKF 独占发布 `odom → base_footprint`。
- [ ] SLAM 可保存稳定地图。
- [ ] AMCL + Nav2 可稳定导航至多个工位。
- [ ] MoveIt2 可进行含底盘碰撞体的机械臂规划。
- [ ] 任务状态机具备收臂—导航—抓取—放置闭环和失败互锁。

## 10. 主要风险与规避

| 风险 | 规避措施 |
| --- | --- |
| 底盘和机械臂有多个 controller manager | 复合模型仅保留一个 `gz_ros2_control` 插件 |
| TF 重复发布 | 明确 `map→odom`、`odom→base_footprint` 的唯一发布者 |
| Nav2 低估机器人尺寸 | footprint 覆盖收纳后的机械臂、夹爪和外壳 |
| 模型抖动或翻倒 | 补全惯量、摩擦、阻尼与碰撞体，降低初始速度 |
| 机械臂碰撞底盘 | 在 MoveIt Planning Scene 中加入底盘、雷达和工位 |
| 移动中机械臂导致风险扩大 | 一期执行“底盘静止、机械臂操作”的强互锁 |
| 麦克纳姆仿真误差过大 | 一期使用差速，二期单独验证全向运动学和接触模型 |
