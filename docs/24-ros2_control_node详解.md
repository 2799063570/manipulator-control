---
title: ros2_control_node 详解
tags: [ros2_control, controller_manager, Fake, 真机]
---

# ros2_control_node 详解

`ros2_control_node` 是 `controller_manager` 包提供的通用宿主进程：它创建 ResourceManager 和 ControllerManager、加载 hardware plugin、管理 controller 生命周期，并在固定周期执行 read-update-write。它主要用于 Fake 与真机；Gazebo 中相同职责通常由 `gz_ros2_control` 插件承担。

## 1. 它不是什么

- 它不是具体控制器；`JointTrajectoryController`、`DiffDriveController`、`JointStateBroadcaster` 才是 controller plugin。
- 它不是 MoveIt；MoveIt 只规划并发送 Action。
- 它不是 robot_state_publisher；RSP 消费 `/joint_states` 并发布 TF。
- 它不是厂商驱动；厂商驱动或自写 `SystemInterface` 是它加载的 hardware plugin。

## 2. 内部对象

```text
ros2_control_node 进程
  └─ ControllerManager
      ├─ ResourceManager
      │   └─ 一个 hardware plugin
      │       ├─ Fake：mock_components/GenericSystem
      │       └─ 真机：aubo_i5_hardware/AuboI5System
      ├─ JointStateBroadcaster plugin
      └─ JointTrajectoryController plugin
```

ResourceManager 保存硬件导出的资源，例如 `joint1/position` command interface 和 `joint1/velocity` state interface。ControllerManager 负责实例化、配置和激活控制器。硬件插件负责把这些 interface 接到内存或控制柜。

## 3. 两类必须输入

| 输入       | 来自哪里                          | 用途                                                 |
| -------- | ----------------------------- | -------------------------------------------------- |
| 最终 URDF  | RSP 重发布的 `/robot_description` | 找 ros2_control 标签、hardware plugin、joint、接口与硬件参数    |
| 控制器 YAML | `ros2_controllers.yaml`       | 注册 controller 名、plugin type、update rate、joints 与接口 |

Jazzy 推荐链路是：launch 执行 Xacro，随后把结果设为 RSP 的 `robot_description` 参数；RSP 使用 transient-local QoS 重发布 `/robot_description`；`ros2_control_node` 订阅该 topic。控制器 YAML 则作为 `ros2_control_node` 的参数文件传入。

```python
rsp = Node(package="robot_state_publisher", executable="robot_state_publisher",
           parameters=[robot_description])
cm = Node(package="controller_manager", executable="ros2_control_node",
          parameters=[ros2_controllers_yaml])
```

> [!warning] Jazzy
> 不要将 controller manager 的 `robot_description` 参数当作主要机制。当前学习时以 RSP 发布 description topic、manager 订阅该 topic 为准；使用 `ros2 topic info /robot_description -v` 验证连接。

## 4. 启动后发生什么

1. 创建 ControllerManager，读取 YAML 中的 `update_rate` 和 `arm_controller.type` 等参数。
2. 接收 `/robot_description`，解析 `<ros2_control>`，得到 hardware 信息、joint 和 interface。
3. ResourceManager 用 pluginlib 创建 `<hardware><plugin>` 指定的 hardware plugin。
4. hardware 执行 `on_init`，导出 command/state interface。
5. manager 提供控制器生命周期服务；此时 YAML 中的 `arm_controller` 尚未被实例化。
6. `spawner` 请求 manager 加载 `arm_controller`；manager 再依据 YAML 的 type 用 pluginlib 创建 JTC。
7. configure 时 JTC 检查 joints/interfaces 并读取自身参数；activate 时 claim command interface，Action server 才会出现。

因此 YAML 不会自动创建控制器：它是参数和注册表，spawner/service 才触发创建和激活。

## 5. 控制周期

若 `update_rate: 100`，目标周期约 10 ms：

```text
hardware.read(time, period)
  Fake：更新内存 q/dq
  真机：读编码器、速度、模式、急停、故障、时间戳

controller_manager.update(time, period)
  JSB：从 state interface 发布 /joint_states
  JTC：采样轨迹、比较 desired/actual、写 command interface

hardware.write(time, period)
  Fake：消费内存 command 并模拟状态
  真机：安全检查后将命令发送给控制柜或 MCU
```

interface 是 manager 内共享资源，不是 topic。JTC 写 `joint1/position`，hardware 的 write 同周期读这个值。interface 被 claim 后，其他 controller 不能同时控制同一关节。

## 6. Fake 与真机

### Fake

Xacro 选择 `<plugin>mock_components/GenericSystem</plugin>`。RSP 发布 description，node 创建 GenericSystem，spawner 激活 JSB/JTC，所有 read/write 均在内存，通常 `use_sim_time:=false`。可验证模型、MoveIt、Action、接口和 controller 配置；不能验证动力学、传感器、控制柜或安全停机。

### 真机

Xacro 选择厂商/自写 plugin，例如 `<plugin>aubo_i5_hardware/AuboI5System</plugin>`。拓扑与 Fake 相同，但 `on_configure` 建立控制柜通信，read 读真实状态，write 发真实命令。先只读核对 q/dq、单位、零位、方向、急停和超时；未确认前不要激活轨迹控制器。

## 7. hardware 与 controller 的生命周期

| 对象 | 关键回调/状态 | 目的 |
|---|---|---|
| hardware | `on_init`、`on_configure`、`on_activate`、read、write | 是否能安全连接并导出接口 |
| controller | unconfigured、inactive、active | 是否能 claim interface 并执行算法 |

真机 hardware 可以 ready 但 JTC 仍 inactive。首次 activate 时要用当前真实 q 初始化 command 并 hold；不能默认向零位发送命令，否则会突跳。

## 8. spawner 的角色

spawner 是短命命令行客户端，不是 controller。它向已经运行的 manager 请求加载、配置、激活名为 `arm_controller` 的 plugin：

```bash
ros2 run controller_manager spawner arm_controller \
  --controller-manager /controller_manager
```

spawner 退出后，JTC 仍在 manager 内运行。检查：

```bash
ros2 node list | grep controller_manager
ros2 control list_hardware_components
ros2 control list_hardware_interfaces
ros2 control list_controllers
ros2 action list -t
```

## 9. 分层实验

1. 启动 RSP 与 control node，不启动 spawner：有 manager/hardware interface，无 active JTC/action。
2. 执行 `ros2 control list_hardware_interfaces`：确认 GenericSystem/真机 plugin 导出接口。
3. 只启动 joint_state_broadcaster：确认 `/joint_states` 出现。
4. 启动 arm_controller：确认 FollowJointTrajectory action 出现。
5. 最后才发送小幅 trajectory。

真机只可将前 3 步作为只读诊断。激活 JTC 前必须验证实体急停、控制柜模式、通信超时和 hold 行为。

## 10. 面试背诵

**问：ros2_control_node 做什么？**  
答：它是 ControllerManager 的通用宿主进程，从 robot_description 解析 ros2_control 硬件描述，使用 ResourceManager 通过 pluginlib 加载 hardware interface，读取控制器 YAML 并提供控制器生命周期服务。spawner 请求其加载/激活具体 controller；运行时它循环执行 hardware read、controller update、hardware write，让同一控制器能复用于 Fake、Gazebo 与真机。

## 12. 逐行解析当前 `planning_execution.launch.py` 的 control node

当前代码：

```python
ros2_control_node = Node(
    package="controller_manager",
    executable="ros2_control_node",
    parameters=[
        moveit_config.robot_description,
        str(ros2_controllers_path),
        {"use_sim_time": use_sim_time},
    ],
    output="screen",
    condition=IfCondition(use_ros2_control),
)
```

它等价于“若 `use_ros2_control` 为 true，则启动一个名为 `ros2_control_node` 的 ROS 2 进程，并给它模型、控制器 YAML 和时钟参数”。逐项解释：

| 代码 | 含义 | 对系统产生的结果 |
|---|---|---|
| `Node(...)` | launch_ros 的一个启动动作 | 启动一个独立的 ROS 2 进程 |
| `package="controller_manager"` | 从哪个 ROS package 查可执行文件 | 等价于查 `ros2 pkg prefix controller_manager` |
| `executable="ros2_control_node"` | 要执行哪个程序 | 创建 ResourceManager 与 ControllerManager，运行控制循环 |
| `parameters=[...]` | 给该节点加载的参数源，按列表顺序合并 | 后面的同名参数覆盖前面 |
| `output="screen"` | stdout/stderr 显示在 launch 终端 | 能看到硬件、controller、pluginlib 报错 |
| `condition=IfCondition(...)` | launch 条件 | `use_ros2_control:=false` 时该进程根本不启动 |

### 12.1 `moveit_config.robot_description` 是什么

`MoveItConfigsBuilder(...).to_moveit_configs()` 生成一个配置对象。`moveit_config.robot_description` 是 Python 字典，概念上类似：

```python
{
  "robot_description": "<robot name='simple_2dof_arm'> ... 完整 URDF ... </robot>"
}
```

这段 URDF 由 Xacro 展开而来，包含普通 link/joint，也包含 `<ros2_control>` 标签。在 Fake 模式，该标签最终含：

```xml
<hardware>
  <plugin>mock_components/GenericSystem</plugin>
</hardware>
```

历史上/部分版本中，将这项直接作为 `ros2_control_node` 参数，会让 manager 直接使用 URDF 来创建 ResourceManager 和 GenericSystem。

> [!warning] Jazzy 的重要适配
> Jazzy 的 controller manager 推荐通过订阅 `/robot_description` 获取 URDF；`robot_state_publisher` 持有 `robot_description` 参数并将其重发布为 transient-local topic。因此当前代码中把 `moveit_config.robot_description` 同时传给 control node 是旧式/冗余写法，不能作为你的主要理解或唯一依赖。
>
> 正确的 Jazzy 心智模型是：同一 `moveit_config.robot_description` 传给 **RSP**；RSP 发布 `/robot_description`；`ros2_control_node` 订阅该 topic。若当前 launch 在你的 Jazzy 环境可运行，仍建议用 `ros2 topic info /robot_description -v`、节点日志和 `ros2 control list_hardware_components` 验证 manager 实际已收到模型。

### 12.2 `str(ros2_controllers_path)` 是什么

`ros2_controllers_path` 指向本包的：

```text
simple_arm_moveit_config/config/ros2_controllers.yaml
```

`str(...)` 将 pathlib Path 转为普通字符串路径；`parameters` 发现字符串且它是 YAML 文件时，将其作为 ROS 参数文件加载。它不是让 `ros2_control_node` “打开并直接运行 controller”的命令，而是设置参数，例如：

```text
/controller_manager 的 update_rate = 100
/controller_manager 的 arm_controller.type
  = joint_trajectory_controller/JointTrajectoryController
/arm_controller 的 joints = [joint1, joint2]
/arm_controller 的 command_interfaces = [position]
```

此时 `arm_controller` 只是参数中的名称和类型，尚未有 JTC 对象。后面的 spawner 才会使用这些参数调用 manager 的服务，实际 load/configure/activate 它。

### 12.3 `use_sim_time` 为什么也传给它

`use_sim_time` 是 ROS 节点通用参数：

- Fake：通常为 `false`，node 使用电脑墙钟计算 control period。
- Gazebo：应为 `true`，node/manager 应以 `/clock` 为时间源。
- 真机：通常为 `false`，使用本机稳定时钟或厂商同步时钟。

当前这个 launch 是 Fake hardware launch，默认 `use_sim_time:=false`。如果把它误设为 true 而又没有启动 Gazebo `/clock` publisher，定时器/时间相关行为会停滞或异常。

### 12.4 `condition=IfCondition(use_ros2_control)` 的用途

该 launch 把 `use_ros2_control` 声明为 launch argument。运行：

```bash
ros2 launch simple_arm_moveit_config planning_execution.launch.py \
  use_ros2_control:=false
```

时，这个 Node 不启动；同时应关闭 controller spawner，否则 spawner 会因找不到 `/controller_manager` 失败。这个开关适合单独调 MoveIt 规划或使用外部已有的 controller manager。

### 12.5 它之后还必须发生什么

这段 Node 只完成“manager/hardware 宿主已启动”，还没有真正的控制器。后续两个 Node：

```python
spawner joint_state_broadcaster --controller-manager /controller_manager
spawner arm_controller --controller-manager /controller_manager
```

才使系统出现以下接口：

```text
JointStateBroadcaster active
  -> /joint_states
JointTrajectoryController active
  -> /arm_controller/follow_joint_trajectory
```

然后 MoveIt 才能通过 `moveit_controllers.yaml` 中的 action 映射执行轨迹。

### 12.6 推荐你做的验证

启动该 launch 后，按此顺序检查：

```bash
# 1. 条件为 true 时，进程/节点存在
ros2 node list | grep controller_manager

# 2. URDF 链路存在，RSP 是 description 发布者
ros2 topic info /robot_description -v

# 3. GenericSystem 已由 manager 创建并导出接口
ros2 control list_hardware_components
ros2 control list_hardware_interfaces

# 4. spawner 已激活 controller
ros2 control list_controllers

# 5. JTC action 已出现
ros2 action list -t | grep follow_joint_trajectory
```

若第 1 步失败，检查 `use_ros2_control`；第 2 步失败，检查 RSP/Xacro；第 3 步失败，检查 `<ros2_control>` 和 GenericSystem plugin；第 4 步失败，检查 YAML/spawner/joint name；第 5 步失败，检查 JTC plugin 与 controller 是否 active。
