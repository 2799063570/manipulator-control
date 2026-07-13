---
title: AUBO i5 从 ROS 1 风格迁移到 Jazzy 实施方案
tags: [AUBO, ROS2, Jazzy, Xacro, ros2_control, Gazebo]
---

# AUBO i5 从 ROS 1 风格迁移到 Jazzy 实施方案

> [!abstract] 结论
> 当前 `aubo_i5_ros2_control/src/robot_description` 是“ROS 2 的 ament 包 + ROS 1 风格的控制 Xacro”。模型、网格和大部分惯量数据可复用；但 `<transmission>`、`gazebo_ros_control`、`$(find ...)`、包名和固定坐标关系必须系统迁移，才能稳定进入 ROS 2 Jazzy 的 Fake、Gazebo Harmonic 与真机链路。

## 1. 当前包审计结果

| 位置               | 当前内容                                                         | 判断                                    | Jazzy 处理                                                  |
| ---------------- | ------------------------------------------------------------ | ------------------------------------- | --------------------------------------------------------- |
| `urdf/arm.xacro` | 六轴 link/joint、mesh、inertial、limit                            | 大部分可复用                                | 保留 joint 名/轴向，逐轴验证                                        |
| `arm.xacro` 尾部   | `<transmission>`、`hardware_interface/PositionJointInterface` | ROS 1 `ros_control` 风格                | 删除，改 `<ros2_control>` command/state interface             |
| `arm.xacro` 尾部   | `libgazebo_ros_control.so`、`DefaultRobotHWSim`               | Gazebo Classic / ROS 1                | 删除，改 `gz_ros2_control`                                    |
| `aubo_i5.xacro`  | `$(find robot_description)` include                          | ROS 1 substitution 风格                 | 改 `$(find-pkg-share aubo_i5_description)` 或在 launch 传绝对路径 |
| `aubo_i5.xacro`  | `world2base_link`                                            | 与 arm 中 `world_joint` 都连到 `base_link` | 只能保留一条；否则 TF/URDF 多父节点                                    |
| `package.xml`    | 包名 `robot_description`、只依赖 rclcpp                            | 过于通用、依赖不完整                            | 改名/补 xacro、RSP、资源依赖                                       |
| display launch   | 仅 RSP + GUI + RViz                                           | 可作为显示基线                               | 加 `use_sim_time`、RViz config，分离控制 launch                  |
|                  |                                                              |                                       |                                                           |

> [!danger] 不要先改关节名
> 当前关节名为 `shoulder_joint`、`upperArm_joint`、`foreArm_joint`、`wrist1_joint`、`wrist2_joint`、`wrist3_joint`。在 URDF、SRDF、controller YAML、MoveIt、厂商驱动之间必须完全一致。迁移第一阶段保留这些名称，验证通过后若要改为统一命名再全局迁移。

## 2. 目标目录结构

建议不要继续让一个 `robot_description` 包同时承担模型、Gazebo、MoveIt 和真机通信。重构为：

```text
aubo_i5_ros2_control/src/
  aubo_i5_description/          # 纯模型：URDF/Xacro、mesh、RViz
    urdf/
      aubo_i5.urdf.xacro        # 总入口
      aubo_i5_arm.xacro         # link/joint/inertial
      aubo_i5.ros2_control.xacro# 硬件接口宏
      gripper.xacro
      sensors/camera.xacro
    meshes/
    launch/display.launch.py
  aubo_i5_moveit_config/        # SRDF、kinematics、OMPL、MoveIt controller 映射
  aubo_i5_gazebo/               # world、Gazebo spawn、bridge、spawner
  aubo_i5_hardware/             # 真机 SystemInterface 或厂商驱动适配
  aubo_i5_bringup/              # fake / gazebo / real 三套 launch
```

最小可行阶段可暂时保留现有 package 名，但长期应将 `robot_description` 重命名为 `aubo_i5_description`，避免与其他机器人模型包冲突。重命名时同步替换 `package://robot_description/...` mesh URI、launch 的 `FindPackageShare` 与 Xacro include。

## 3. 第一阶段：先把“纯模型”从 ROS 1 控制代码中剥离

### 3.1 保留的内容

- `<link>` 的 visual/collision/inertial。
- 六个 `<joint>` 的 origin、axis、limit（但最大 velocity/effort 必须与厂商手册复核）。
- 夹爪、相机、桌子几何。
- `package://` mesh URI 的形式（改包名后再替换包路径）。

### 3.2 必须删除的 ROS 1 块

从 `arm.xacro` 删除整段 transmission 宏、6 次 transmission 调用和 Gazebo Classic plugin：

```xml
<!-- 删除，不可用于 Jazzy -->
<transmission> ... hardware_interface/PositionJointInterface ... </transmission>
<plugin name="gazebo_ros_control" filename="libgazebo_ros_control.so"> ... </plugin>
```

Jazzy 不使用 Gazebo Classic 的 `gazebo_ros_control`；不要保留它“以防万一”，否则会在 Ubuntu 24.04/Gazebo Harmonic 中造成 plugin not found 或概念混乱。

### 3.3 修复根坐标树

当前 `arm.xacro` 有：

```xml
<link name="world"/>
<joint name="world_joint" type="fixed">
  <parent link="world"/><child link="base_link"/>
</joint>
```

而 `aubo_i5.xacro` 又加了：

```xml
<joint name="world2base_link" type="fixed">
  <parent link="world"/><child link="base_link"/>
</joint>
```

这会让 `base_link` 有两个 parent joint，不是合法树。选择一种方案：

- **方案 A（推荐）**：`aubo_i5_arm.xacro` 只定义机械臂本体，不定义 `world`；总入口用参数 `use_world_joint` 决定是否添加唯一的 `world -> base_link`。
- **方案 B**：保留 `world_joint`，删除外层 `world2base_link`。

方案 A 的总入口模板：

```xml
<xacro:arg name="use_world_joint" default="true"/>
<xacro:if value="$(arg use_world_joint)">
  <link name="world"/>
  <joint name="world_joint" type="fixed">
    <parent link="world"/>
    <child link="base_link"/>
  </joint>
</xacro:if>
```

## 4. 第二阶段：改 Xacro include 与总入口参数

ROS 1 形式：

```xml
<xacro:include filename="$(find robot_description)/urdf/arm.xacro"/>
```

Jazzy 推荐改为：

```xml
<xacro:include filename="$(find-pkg-share aubo_i5_description)/urdf/aubo_i5_arm.xacro"/>
```

同样修改夹爪、相机 include。总入口应暴露后端选择：

```xml
<?xml version="1.0"?>
<robot name="aubo_i5" xmlns:xacro="http://www.ros.org/wiki/xacro">
  <xacro:arg name="hardware_type" default="fake"/>
  <xacro:arg name="initial_positions_file" default=""/>
  <xacro:arg name="ros2_controllers_file" default=""/>
  <xacro:arg name="robot_ip" default=""/>
  <xacro:arg name="use_world_joint" default="true"/>

  <xacro:include filename="$(find-pkg-share aubo_i5_description)/urdf/aubo_i5_arm.xacro"/>
  <xacro:include filename="$(find-pkg-share aubo_i5_description)/urdf/aubo_i5.ros2_control.xacro"/>
  <xacro:aubo_i5_ros2_control
      name="AuboI5System"
      hardware_type="$(arg hardware_type)"
      initial_positions_file="$(arg initial_positions_file)"
      ros2_controllers_file="$(arg ros2_controllers_file)"
      robot_ip="$(arg robot_ip)"/>
</robot>
```

Xacro 变量语法要统一：`$(arg hardware_type)` 是 launch/Xacro argument substitution；`${变量}` 是宏/属性表达式。不要在同一处混用 `$(find ...)`、`$(find-pkg-share ...)` 和未定义 `${...}`。

## 5. 第三阶段：新增 `aubo_i5.ros2_control.xacro`

这是替代 transmission 的核心。先只实现 Fake 和 Gazebo；真机 plugin 名在确认 AUBO 官方 Jazzy driver 后再填。

```xml
<?xml version="1.0"?>
<robot xmlns:xacro="http://www.ros.org/wiki/xacro">
  <xacro:macro name="aubo_i5_ros2_control"
    params="name hardware_type initial_positions_file ros2_controllers_file robot_ip">

    <ros2_control name="${name}" type="system">
      <hardware>
        <xacro:if value="${hardware_type == 'fake'}">
          <plugin>mock_components/GenericSystem</plugin>
        </xacro:if>
        <xacro:if value="${hardware_type == 'gazebo'}">
          <plugin>gz_ros2_control/GazeboSimSystem</plugin>
        </xacro:if>
        <xacro:if value="${hardware_type == 'real'}">
          <plugin>aubo_i5_hardware/AuboI5System</plugin>
          <param name="robot_ip">${robot_ip}</param>
        </xacro:if>
      </hardware>

      <joint name="shoulder_joint">
        <command_interface name="position"/>
        <state_interface name="position"/>
        <state_interface name="velocity"/>
      </joint>
      <!-- 对 upperArm_joint、foreArm_joint、wrist1/2/3_joint 完全重复 -->
    </ros2_control>

    <xacro:if value="${hardware_type == 'gazebo'}">
      <gazebo>
        <plugin filename="libgz_ros2_control-system.so"
                name="gz_ros2_control::GazeboSimROS2ControlPlugin">
          <parameters>${ros2_controllers_file}</parameters>
        </plugin>
      </gazebo>
    </xacro:if>
  </xacro:macro>
</robot>
```

约束：六个 joint 名必须与主臂 URDF 完全相同；position command 与 position/velocity state 必须同 controller YAML 一致。不要先添加 effort/velocity command interface，除非 Gazebo/真机硬件和控制算法确实要使用它们。

## 6. 第四阶段：控制器 YAML 与启动方式

创建 `aubo_i5_moveit_config/config/ros2_controllers.yaml`：

```yaml
controller_manager:
  ros__parameters:
    update_rate: 100
    joint_state_broadcaster:
      type: joint_state_broadcaster/JointStateBroadcaster
    arm_controller:
      type: joint_trajectory_controller/JointTrajectoryController

arm_controller:
  ros__parameters:
    joints:
      - shoulder_joint
      - upperArm_joint
      - foreArm_joint
      - wrist1_joint
      - wrist2_joint
      - wrist3_joint
    command_interfaces: [position]
    state_interfaces: [position, velocity]
```

启动规则：

| 后端 | 谁启动 manager | YAML 如何加载 | 谁 spawn controller |
|---|---|---|---|
| Fake | 外部 `ros2_control_node` | `parameters=[controllers_yaml]` | 外部 ROS 2 spawner |
| Gazebo Harmonic | `gz_ros2_control` 模型插件 | URDF plugin 的 `<parameters>` 路径 | 外部 ROS 2 spawner |
| 真机 | 外部 `ros2_control_node` | `parameters=[controllers_yaml]` | 外部 ROS 2 spawner，ready 后才启动 |

Fake/真机模式中 Jazzy 应由 RSP 统一持有模型，并重发布 transient-local `/robot_description`；`ros2_control_node` 订阅这个 topic。Gazebo 模式由 `create -topic robot_description` 把同一 URDF 送进 Gazebo。

## 7. 第五阶段：按顺序验证，不能跳步

### A. 纯 URDF / RSP

```bash
ros2 run xacro xacro \
  $(ros2 pkg prefix aubo_i5_description)/share/aubo_i5_description/urdf/aubo_i5.urdf.xacro \
  hardware_type:=fake > /tmp/aubo_fake.urdf
check_urdf /tmp/aubo_fake.urdf
```

验收：没有多 parent link；没有未展开的 `${...}`；mesh URI 有效；6 轴关节与工具/夹爪坐标树完整。

### B. Fake + ros2_control

启动 RSP、外部 `ros2_control_node`、spawner。验收：

```bash
ros2 control list_hardware_components
ros2 control list_hardware_interfaces
ros2 control list_controllers
ros2 action list -t
```

应有 GenericSystem、6 轴 position command/state interface、active JSB/JTC、`/arm_controller/follow_joint_trajectory`。

### C. MoveIt Fake

用 Setup Assistant 生成 SRDF 与 MoveIt config；先验证 home/ready 的 joint 目标，再验证 pose IK。要用展开后的 URDF，且确认 base 是 `base_link`、tip 是实际 TCP/工具 link。

### D. Gazebo Harmonic

使用 `hardware_type:=gazebo` 展开同一 Xacro；确认最终 URDF 含 `GazeboSimSystem` 和 `GazeboSimROS2ControlPlugin`。先 CLI 发 0.05 rad 小轨迹，再由 MoveIt 执行。Gazebo 下不得同时启动外部 `ros2_control_node`。

### E. 真机只读，最后才运动

`hardware_type:=real` 先只让 hardware plugin read。逐轴比对 q/dq、rad/deg、零位、方向、急停/保护停。只有硬件 ready、command 初始化为当前 q、实体急停可达时，才启动 JTC 的 spawner。

## 8. 使用 MoveIt Setup Assistant 创建 AUBO MoveIt Config

> [!important] 使用时机
> Setup Assistant 不是修复 URDF 的工具。必须先完成第 7 节 A、B：Xacro 能展开、`check_urdf` 通过、只有一条根到 `base_link` 的树、6 个关节命名/轴向/limit 已确认，然后才创建 MoveIt config。第一轮使用 `hardware_type:=fake`，不要将未经验证的 Gazebo/真机插件直接带入 Setup Assistant。

### 8.1 准备输入 URDF

先把总 Xacro 展开成一个确定的 Fake URDF：

```bash
cd ~/manipulator-control/aubo_i5_ros2_control
source /opt/ros/jazzy/setup.bash
source install/setup.bash

ros2 run xacro xacro \
  $(ros2 pkg prefix aubo_i5_description)/share/aubo_i5_description/urdf/aubo_i5.urdf.xacro \
  hardware_type:=fake \
  initial_positions_file:=/tmp/aubo_initial_positions.yaml \
  ros2_controllers_file:=/tmp/aubo_ros2_controllers.yaml \
  > /tmp/aubo_i5_fake.urdf

check_urdf /tmp/aubo_i5_fake.urdf
```

如果你的总 Xacro 在迁移初期还没有这些参数，可先只传 `hardware_type:=fake`；但最终启动文件应补齐统一参数。输入文件应包含本体、夹爪固定关系和你希望作为末端的工具 link；视觉/碰撞 mesh 必须能解析。

### 8.2 启动 Setup Assistant

```bash
source /opt/ros/jazzy/setup.bash
QT_QPA_PLATFORM=xcb ros2 run moveit_setup_assistant moveit_setup_assistant
```

Ubuntu 24.04 Wayland 下若报 `Invalid parentWindowHandle`，加：

```bash
QT_QPA_PLATFORM=xcb LIBGL_ALWAYS_SOFTWARE=1 \
  ros2 run moveit_setup_assistant moveit_setup_assistant
```

在 GUI 中选择 **Create New MoveIt Configuration Package**，再选择 `/tmp/aubo_i5_fake.urdf`。不要直接选旧 `arm.xacro`：它不是完整机器人入口，而且仍可能携带 ROS 1 transmission/plugin 内容。

### 8.3 Assistant 中每一步如何填写

| Assistant 页面 | AUBO i5 应做什么 | 验收/易错点 |
|---|---|---|
| Self-Collisions | 生成默认碰撞禁用矩阵，抽查相邻 link 是否被合理禁用 | 不要把桌子、夹爪或真实可能碰撞的链对随意禁用 |
| Virtual Joints | 固定机械臂创建 `world -> base_link` 的 fixed virtual joint；若 URDF 已含唯一 `world_joint`，不要重复创建 | URDF 与 SRDF 只能有一套根连接语义 |
| Planning Groups | 新建 `arm`，选择 **Chain**：base=`base_link`，tip=`tool0` 或实际工具末端 | 不要将 `world`、固定夹爪 link 混入 6 轴关节 group |
| Kinematics | 先使用 KDL；设置较小 timeout，如 0.05–0.1 s | KDL 可作为基线；若后续 IK 成功率/速度不够，再评估 TRAC-IK 等插件 |
| Robot Poses | 创建 `home`、`ready`、`inspect` | 关节值来自已验证的安全姿态，远离限位与奇异位形 |
| End Effectors | 若夹爪已是可控关节，建 `gripper` group 并指定 parent link；若目前只是固定几何，先不建 action end effector | 固定夹爪不能伪装成可开合 controller |
| Passive Joints | 通常 6 轴主臂无 passive joint | 被动/从动关节才填写，例如某些夹爪 mimic 机构 |
| Author Information | 填真实维护者、版本、许可证 | 不保留 Setup Assistant 的占位信息 |
| ROS 2 Controllers | 创建 `arm_controller`，类型为 FollowJointTrajectory，填 6 个关节 | 关节顺序必须同 `ros2_controllers.yaml`、真实驱动一致 |
| MoveIt Controllers | 生成/确认 `moveit_controllers.yaml`，action namespace 为 `follow_joint_trajectory` | 完整 action 应是 `/arm_controller/follow_joint_trajectory` |
| Configuration Package | 包名设为 `aubo_i5_moveit_config`，路径放当前 workspace 的 `src/` 下 | 不输出到 description 包内，避免模型与规划配置耦合 |

### 8.4 生成后的目录与必须审查的文件

Assistant 会生成类似：

```text
aubo_i5_moveit_config/
  config/
    aubo_i5.srdf
    kinematics.yaml
    joint_limits.yaml
    ompl_planning.yaml
    moveit_controllers.yaml
    ros2_controllers.yaml
    initial_positions.yaml
  launch/
    demo.launch.py
    move_group.launch.py
    rsp.launch.py
  package.xml
  CMakeLists.txt
```

生成不代表正确，必须逐项审查：

1. **SRDF**：`arm` group 恰好含六轴；base/tip 正确；virtual joint 未与 URDF 重复；自碰撞禁用项合理。
2. **kinematics.yaml**：group 名必须叫 `arm`（或与你 SRDF 一致）；先确认 KDL 能解。
3. **joint_limits.yaml**：与厂商手册和 URDF 双重核对；MoveIt 限制应不高于真实硬件限制，首次测试建议额外收紧至 10%–20%。
4. **ros2_controllers.yaml**：controller joints 的名称、顺序、interface 与 `aubo_i5.ros2_control.xacro` 一字不差。
5. **moveit_controllers.yaml**：`arm_controller`、joints、`action_ns` 与运行中的 JTC action 相匹配。
6. **launch**：默认 demo 常是 Fake/RViz；将 Gazebo 和 real launch 分开，不要在生成文件中直接塞 IP/真机凭据。

### 8.5 生成后先运行 Fake，再接 Gazebo

先构建所有描述与 MoveIt 包：

```bash
cd ~/manipulator-control/aubo_i5_ros2_control
rosdep install --from-paths src --ignore-src -r -y
colcon build --symlink-install
source install/setup.bash
```

推荐启动顺序：

```text
1. RSP（Fake URDF）
2. ros2_control_node（GenericSystem，订阅 description）
3. joint_state_broadcaster spawner
4. arm_controller spawner
5. move_group
6. RViz MotionPlanning
```

验收：先用 RViz 的 Joint 目标执行 `home -> ready -> home`；再尝试 Pose 目标。只有 Fake 中 controller/action/IK/碰撞模型全部通过后，才把总 Xacro 的 `hardware_type` 改为 `gazebo` 并接入 `aubo_i5_gazebo`。

### 8.6 Setup Assistant 无法替你完成的内容

| 内容 | 为什么不能依赖 Assistant | 应在哪里完成 |
|---|---|---|
| ROS 1 transmission 转 ros2_control | Assistant 不理解旧 Gazebo Classic plugin 的真实语义 | `aubo_i5.ros2_control.xacro` |
| Gazebo world/spawn/bridge | Assistant 不创建 Gazebo Harmonic 仿真场景 | `aubo_i5_gazebo` |
| 真机通信/IP/急停 | Assistant 不应掌握硬件凭据和安全协议 | `aubo_i5_hardware`、bringup |
| TCP/负载/手眼标定 | 依赖实际工具和测量 | 真机标定流程 |
| 高质量 collision/inertial | 自动生成无法替代 CAD/手册 | description 包、CAD 数据 |

### 8.7 生成配置后的最小排障命令

```bash
ros2 param get /move_group robot_description_semantic
ros2 param get /move_group robot_description_kinematics
ros2 control list_controllers
ros2 action list -t | grep follow_joint_trajectory
ros2 topic echo /joint_states --once
ros2 run tf2_tools view_frames
```

若 Joint 目标能执行但 Pose 目标无解，优先检查 SRDF group 的 base/tip、kinematics group 名与 URDF 关节轴；不要一开始就修改 OMPL。若 Plan 成功但 Execute 失败，优先检查 `moveit_controllers.yaml` 与 JTC action 名/关节顺序。

## 9. package.xml、CMake 与依赖改动

description 包最小依赖应反映实际使用的 launch/资源：

```xml
<buildtool_depend>ament_cmake</buildtool_depend>
<exec_depend>robot_state_publisher</exec_depend>
<exec_depend>joint_state_publisher_gui</exec_depend>
<exec_depend>rviz2</exec_depend>
<exec_depend>xacro</exec_depend>
```

如果该包内包含 ros2_control Xacro，运行 Gazebo launch 的包再声明 `gz_ros2_control`、`ros_gz_sim`、`controller_manager`、`joint_trajectory_controller` 等依赖；不要仅因 XML 中写了 plugin 就把所有 Gazebo/MoveIt 依赖都堆到纯 description 包。

`CMakeLists.txt` 保留资源安装即可：

```cmake
install(DIRECTORY urdf launch meshes resources
  DESTINATION share/${PROJECT_NAME})
```

要先确保 `resources` 真实存在，否则不要写进 install 列表。真正的 Gazebo world、controller YAML、MoveIt SRDF 应分别安装到对应 package。

## 10. 建议的实际改造顺序

1. 复制当前包为 `aubo_i5_description`，保留原包作为 ROS 1 参考，不要直接破坏唯一模型副本。
2. 删除 Classic transmission/plugin，修复重复 `world -> base_link`，仅做 RSP/RViz 验证。
3. 新增 `aubo_i5.ros2_control.xacro`，先跑 Fake。
4. 新建 MoveIt config，先验证 joint-space。
5. 使用 Setup Assistant 生成并审查 `aubo_i5_moveit_config`，完成 Fake MoveIt 验证。
6. 新建 Gazebo package，接 `gz_ros2_control` 和 controller YAML。
7. 最后接厂商 Jazzy driver 或实现 SystemInterface；真机只读优先。

每阶段单独 commit、保存展开后的 URDF 与 `view_frames` 结果。任何阶段失败，都回到该阶段排查，不要同时修改 MoveIt、Gazebo 和硬件通信。
