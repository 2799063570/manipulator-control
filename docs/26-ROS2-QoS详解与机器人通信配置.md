---
title: ROS 2 QoS 详解与机器人通信配置
tags: [ROS2, QoS, DDS, 通信, TF, ros2_control]
---

# ROS 2 QoS 详解与机器人通信配置

> [!abstract] 一句话
> QoS（Quality of Service）定义 ROS 2/DDS 的消息“要多可靠、保留多久、缓存多少、迟到订阅者是否拿到历史”。Publisher 与 Subscriber 不仅要 topic 名和消息类型相同，QoS 也必须**兼容**，否则会出现“topic 看得到、但就是收不到消息”。

## 1. QoS 在 ROS 2 的哪一层

```text
你的 Node
  -> rclcpp Publisher / Subscription（指定 QoS）
  -> RMW 实现（Fast DDS / Cyclone DDS 等）
  -> DDS 网络发现与传输
  -> 远端或本机 Node
```

QoS 是 publisher 和 subscription 创建时的契约，不是消息字段。它通常不能在运行中随意改；要改变 QoS，通常需要重建 publisher/subscription 或重启节点。

## 2. 五个最重要策略

### 2.1 Reliability：可靠性

| 策略 | 含义 | 适合 | 代价 |
|---|---|---|---|
| `RELIABLE` | DDS 尝试确保每条消息送达，可能重传 | 控制器状态、关键事件、地图、配置 | 网络差时可能排队/增加延迟 |
| `BEST_EFFORT` | 不重传，丢了就丢 | 高频传感器、相机、激光 | 允许丢帧，但延迟更小 |

“Reliable 一定更好”是错误的。控制机器人时，过期的速度/图像通常比丢一帧更危险；连续传感器更看重最新数据而非补发旧数据。

### 2.2 Durability：持久性

| 策略 | 含义 | 典型用途 |
|---|---|---|
| `VOLATILE` | 只给当前在线订阅者；迟到者不拿历史 | joint states、scan、cmd_vel |
| `TRANSIENT_LOCAL` | publisher 保存最后若干条；迟到订阅者连接后可获得 | `/robot_description`、`/tf_static`、地图/配置 |

本项目最重要实例：RSP 将 `robot_description` 以 transient-local 发布，所以 `ros2_control_node` 或 `ros_gz_sim create` 即使稍晚启动，仍能拿到 URDF；fixed TF 也要以 transient-local `/tf_static` 发布，否则新开的 RViz 会缺静态坐标系。

### 2.3 History 与 Depth：缓存策略和深度

| 策略 | 含义 |
|---|---|
| `KEEP_LAST(depth)` | 保留最近 depth 条；最常用 |
| `KEEP_ALL` | 尽量保留全部；可能耗尽资源，不适合连续数据 |

depth 不是发布频率，也不是“可靠程度”。例如 `/scan` 10 Hz、depth=5，只表示订阅者来不及处理时最多缓存约 5 帧，旧帧会被淘汰。控制命令通常应小 depth，避免旧命令排队后才执行。

### 2.4 Deadline：截止期

Deadline 表示“期望在多长时间内至少收到一次消息”。若超过期限，DDS 产生 deadline missed 事件。它适合监控关键状态/传感器是否失联，但**不会自动让机器人停止**；你仍需在 callback 或诊断逻辑中根据超时进入 hold/安全停机。

### 2.5 Lifespan：寿命

Lifespan 表示消息超过给定时长后不应再交付。它可防止网络阻塞后把过期数据交给订阅者。对速度命令更常见的工程做法仍是节点层 timeout：保存最后一次 `cmd_vel` 时间戳，超时发布零速度。

## 3. QoS 兼容性：为什么“看见 topic 但收不到”

通信建立需要对端提供的 QoS 至少满足订阅端请求。最常见规则：

| Publisher 提供 | Subscriber 请求 | 是否兼容 |
|---|---|---|
| RELIABLE | RELIABLE | 是 |
| RELIABLE | BEST_EFFORT | 是 |
| BEST_EFFORT | BEST_EFFORT | 是 |
| BEST_EFFORT | RELIABLE | 否 |
| TRANSIENT_LOCAL | TRANSIENT_LOCAL | 是 |
| VOLATILE | TRANSIENT_LOCAL | 否 |

可以把订阅者理解为“最低需求”：订阅者要求 Reliable，但发布者只给 Best Effort，无法满足；订阅者要求历史，而发布者不保留历史，也无法满足。

检查命令：

```bash
ros2 topic info /robot_description -v
ros2 topic info /scan -v
ros2 topic info /tf_static -v
```

输出中会分别列出每个 publisher/subscription 的 Reliability、Durability、History、Depth。排错时不要只执行 `ros2 topic list`。

## 4. ROS 2 常用预设 QoS

### `rclcpp::QoS(10)`

常见教学写法；通常是 keep-last 10、reliable、volatile。适合普通低频 topic，但不是所有机器人接口的最佳默认值。

### `rclcpp::SensorDataQoS()`

适合高频传感器，通常为 best-effort、volatile、较小队列。激光、相机、IMU 订阅时经常使用；必须与设备发布端兼容。

### `rclcpp::SystemDefaultsQoS()`

使用中间件/系统默认值。快速测试方便，但跨机器/跨 DDS 时可预测性较弱；关键接口建议显式配置。

### `rclcpp::QoS(...).transient_local()`

用于需要让迟到订阅者获得最后状态的描述、静态配置、地图等。RSP 的 `/robot_description` 与 `/tf_static` 是这一思路的典型。

## 5. 本项目 topic 的推荐 QoS 表

| topic / 接口 | 数据性质 | 推荐 QoS | 原因 |
|---|---|---|---|
| `/robot_description` | 启动配置，最后一条最重要 | reliable + transient-local + keep-last(1) | 后启动的 Gazebo/manager 必须拿到 URDF |
| `/tf_static` | 固定安装关系 | reliable + transient-local | RViz/新节点随时需要完整静态树 |
| `/tf` | 高频动态关节/定位变换 | 常用 TF 默认 QoS | 只需最新变换，不能堆积旧位姿 |
| `/joint_states` | 连续状态反馈 | reliable + volatile + 小队列 | 控制/MoveIt 需可信最新状态；真机别堆积 |
| `/scan` | 高频激光 | SensorDataQoS（通常 best-effort + volatile） | 丢旧帧比迟到旧帧好 |
| `/camera/*` | 高带宽图像 | SensorDataQoS | 降低网络/CPU 堵塞 |
| `/imu` | 高频姿态/加速度 | SensorDataQoS；EKF 端按驱动匹配 | 强调及时性 |
| `/cmd_vel` | 瞬时控制命令 | reliable + volatile + depth 1 或很小 | 只要最新命令；另做 timeout/零速保护 |
| `FollowJointTrajectory` Action | 有反馈、可取消轨迹 | action 默认 QoS，勿随意改 | 标准客户端/服务端需一致 |
| `/diagnostics` | 低频健康状态 | reliable + volatile | 不应轻易丢关键故障 |
| `/map` | 大而低频的全局状态 | reliable + transient-local | Nav2/RViz 晚启动仍需地图 |

> [!warning] `cmd_vel` 的安全性不依赖 QoS
> 即使 Reliable，也不能保证机器人安全停止：网络堵塞可能使旧速度延迟抵达。底盘 controller/MCU 必须有独立 command timeout，超时归零；`twist_mux` 还需让急停来源拥有最高优先级。

## 6. QoS 在当前代码中如何写

当前学习节点有类似：

```cpp
sub_ = this->create_subscription<JointVelocityCmd>(
  "joint_velocity_cmd", 10, callback);
pub_ = this->create_publisher<JointPositionCmd>(
  "joint_position_cmd", 10);
```

其中 `10` 会构造默认的 keep-last(10) QoS。学习阶段可用；以后应按接口含义显式写出：

```cpp
auto command_qos = rclcpp::QoS(rclcpp::KeepLast(1))
  .reliable()
  .durability_volatile();

sub_ = create_subscription<geometry_msgs::msg::Twist>(
  "/cmd_vel", command_qos, callback);

auto sensor_qos = rclcpp::SensorDataQoS();
scan_sub_ = create_subscription<sensor_msgs::msg::LaserScan>(
  "/scan", sensor_qos, scan_callback);
```

显式 QoS 的好处是代码审查时能看到设计意图；但发布端和订阅端必须协同修改，不能只改一端。

## 7. 亲自做的 QoS 实验

### 实验 A：观察 RSP 的 transient-local

1. 启动 RSP/任意机器人 launch，等待几秒。
2. **之后**再开新终端执行：

```bash
ros2 topic echo /robot_description --once
ros2 topic info /robot_description -v
```

新订阅者仍应立即收到 URDF，这证明不是“RSP 正好又发布一次”，而是 transient-local 保存了最后样本。

### 实验 B：制造 QoS 不兼容

使用一个发布端为 best-effort、订阅端为 reliable 的简单测试节点（建议新建最小包，避免改动控制节点）。观察两端虽然发现彼此，但 subscription callback 不触发；再将订阅端改为 `SensorDataQoS`/best-effort，通信恢复。实验后用 `ros2 topic info -v` 解释结果。

### 实验 C：队列堆积与旧命令

让 publisher 以 100 Hz 发布序号，subscriber callback 故意以 10 Hz 处理。分别用 depth=1 和 depth=100 观察接收序号：depth=1 更接近最新值，depth=100 会积压大量历史。该实验解释为何控制命令不能用大队列。

## 8. 高频错误与排障

| 现象 | 最可能原因 | 处理 |
|---|---|---|
| topic list 有名字，但 callback 从不触发 | QoS 不兼容 | `ros2 topic info -v` 比较两端 Reliability/Durability |
| create 等不到 robot_description | RSP 未发布，或 subscriber QoS 不接受 transient-local | 先启动 RSP，检查 description QoS |
| RViz 新开后无静态 frame | `/tf_static` 被错误发布为 volatile，或没有 fixed joint | 检查 tf_static QoS/TF 图 |
| 激光/图像时有时无 | 订阅端强求 reliable，驱动只提供 best-effort | 使用 SensorDataQoS 或匹配驱动 QoS |
| 机器人收到陈旧 cmd_vel | depth 太大、没有 timeout | depth 1/小队列 + 独立 timeout |
| 多机通信异常 | ROS_DOMAIN_ID、DDS/RMW、网络 multicast 或 QoS 不一致 | 逐项检查环境变量和 `topic info -v` |

## 9. 面试背诵

**问：QoS 为什么会导致 ROS 2 看得到 topic 却收不到消息？**  
答：ROS 2 基于 DDS，连接建立除 topic 名和类型外还要求 QoS 兼容。比如订阅者要求 Reliable，而发布者只提供 Best Effort，就不能匹配；订阅者要求 transient-local 历史而发布者是 volatile 也不兼容。我会用 `ros2 topic info -v` 查看双方的 reliability、durability、history 和 depth。

**问：为什么 robot_description 和 tf_static 要 transient-local？**  
答：它们是低频但关键的启动配置。Gazebo、controller manager 或新开的 RViz 可能晚于 RSP 启动，transient-local 会保留最后消息，让迟到订阅者仍可得到 URDF 和固定坐标系。动态传感器/控制则更关注最新数据，通常不应堆积历史。

**问：控制命令应选 Reliable 还是 Best Effort？**  
答：要看控制系统需求，常见机器人内网会用 reliable、volatile、小 depth，但绝不依赖 QoS 代替安全机制。底层必须有 command timeout、速度/加速度限制和急停；过期命令即使可靠送达也不应继续执行。

