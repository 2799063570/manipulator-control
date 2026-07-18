# AUBO i5 SDK 与真机验收程序说明

本功能包提供三套彼此独立的验收程序，用于按“SDK → 只读连接 → 单轴低速动作”的顺序接入真机。

> [!danger] 真机安全边界
> 单轴测试会向控制柜发送 Servo 命令。执行前必须确认现场监护人员在场、实体急停可达、机械臂空载、工作区已清空，并已在示教器/控制柜中完成厂家要求的安全设置。测试程序不能代替实体安全措施。

运行这些独立测试程序时，不要同时启动 `aubo_i5_real.launch.py`、MoveIt 或其他连接、控制同一台机械臂的程序，避免多个客户端竞争控制权。

## 1. 构建前提

在部署 Ubuntu 主机中，将与控制柜版本和 CPU 架构匹配的 AUBO Linux SDK 放入：

```text
aubo_i5_hardware/third_party/aubo_sdk/
```

其中必须包含 `include/`、`lib/libaubo_sdk.so` 与 `lib/cmake/aubo_sdk/aubo_sdkConfig.cmake`。

随后构建并加载工作空间：

```bash
source /opt/ros/jazzy/setup.bash
cd ~/aubo_i5_ros2_control
colcon build --packages-select aubo_msgs aubo_dashboard_msgs aubo_i5_hardware
source install/setup.bash
```

## 2. 本地测试配置

[`config/robot_test.example.yaml`](config/robot_test.example.yaml) 是可提交的示例，不保存真实 IP 或凭据。先复制成仅本机使用的配置文件：

```bash
cp $(ros2 pkg prefix aubo_i5_hardware)/share/aubo_i5_hardware/config/robot_test.example.yaml \
  ~/robot_test.local.yaml
```

编辑 `~/robot_test.local.yaml`，填写并复核以下内容：

| 配置项 | 用途 | 注意事项 |
| --- | --- | --- |
| `connection.robot_ip` | 控制柜 IP 地址 | 必填；不提交到 Git。 |
| `rpc_port` / `rtde_port` | AUBO RPC/RTDE 端口 | 必须与现场控制柜一致。 |
| `feedback_timeout_ms` | 等待实时反馈的超时 | 超时即视为连接失败。 |
| `joint_name` | 单轴测试关节名 | 必须是六轴之一。 |
| `delta_rad` | 单次测试位移 | 程序限制为绝对值不超过 `0.05 rad`。 |
| `duration_s` | 测试时长 | 程序限制为 2–10 秒。 |
| `speed_fraction` | 控制柜速度比例 | 程序限制为 `(0, 0.05]`。 |
| `lower_limit_rad` / `upper_limit_rad` | 软件限位 | 必须根据厂家批准的真机限位确认。 |

凭据不写入 YAML。所有需要连接控制柜的程序都从环境变量读取：

```bash
export AUBO_ROBOT_USERNAME='你的用户名'
export AUBO_ROBOT_PASSWORD='你的密码'
```

## 3. 程序一：SDK 自检

```bash
ros2 run aubo_i5_hardware aubo_sdk_smoke_test
```

用途：验证程序能加载 AUBO SDK，并能构造 RPC/RTDE 客户端。

安全性：不建立网络连接，不读取控制柜，也不会发送任何运动命令。

通过条件：输出 `AUBO SDK smoke test passed`。

## 4. 程序二：只读连接与反馈检查

```bash
ros2 run aubo_i5_hardware aubo_i5_connection_test \
  --config ~/robot_test.local.yaml
```

用途：通过 RPC 与 RTDE 登录控制柜，等待一帧有效的六轴实时反馈，打印关节位置（rad）、关节速度（rad/s）、机器人模式与安全模式，然后主动断开连接。

安全性：不进入 Servo 模式，不会发送任何运动命令。

通过后必须人工核对：

1. 六个关节的顺序是否与示教器一致；
2. 位置单位是否为 rad、速度单位是否为 rad/s；
3. 手动小幅点动时，RViz/反馈中的正方向是否正确；
4. 零位与当前姿态是否符合预期。

## 5. 程序三：单轴低速小位移测试

仅在前两个程序通过后执行。首次建议使用 `shoulder_joint`、`delta_rad: 0.02`、`duration_s: 5.0`，一次只测一个关节。

```bash
ros2 run aubo_i5_hardware aubo_i5_single_joint_test \
  --config ~/robot_test.local.yaml \
  --enable-motion \
  --acknowledge-safety
```

两个动作确认参数缺一不可：

- `--enable-motion`：明确允许程序发送 Servo 命令；
- `--acknowledge-safety`：确认操作者已完成现场安全检查。

程序在每个 8 ms 控制周期都会检查实时反馈是否新鲜，以及控制柜是否处于 `Running` 且安全状态为 `Normal` 或 `ReducedMode`。任一检查失败、SDK 返回错误或目标超出 YAML 中的限位时，程序立即停止继续发送轨迹；结束时会退出 Servo 模式并尝试恢复原有速度比例。

通过条件：实际反馈的单轴位移与 YAML 中 `delta_rad` 的误差不超过 `0.03 rad`，且无保护停机、急停或其他告警。

## 6. 推荐验收顺序

1. SDK 自检通过；
2. 只读连接通过，完成关节顺序、单位、零位和方向核对；
3. 空载单轴低速测试，逐轴进行；
4. 再启动 `aubo_i5_real.launch.py` 的只读模式，验证 `/joint_states`；
5. 最后才在现场安全验收通过后，使用 `enable_motion:=true` 接入 MoveIt 短轨迹。

任何一级失败，都应停止在当前级别，先排查 SDK 版本、网络、控制柜模式、关节映射或标定数据，不能通过提高速度或绕过安全检查继续测试。
