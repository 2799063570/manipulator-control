#! /usr/bin/env python
# coding=utf-8

"""
功能：机械臂ServoJ运动

步骤:
第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
第二步: ServoJ运动
第三步: RPC 退出登录、断开连接
"""

import time
import math
import pyaubo_sdk

robot_ip = "127.0.0.1"  # 机械臂 IP 地址
robot_port = 30004  # 端口号


# 阻塞
def wait_arrival(robot_interface):
    max_retry_count = 5
    cnt = 0

    # 接口调用: 获取当前的运动指令 ID
    exec_id = robot_interface.getMotionControl().getExecId()

    # 等待机械臂开始运动
    while exec_id == -1:
        if cnt > max_retry_count:
            return -1
        time.sleep(0.05)
        cnt += 1
        exec_id = robot_interface.getMotionControl().getExecId()

    # 等待机械臂运动完成
    while robot_interface.getMotionControl().getExecId() != -1:
        time.sleep(0.05)

    return 0


"""
测试: 让1关节旋转120度，规划时间为10秒，
在第5秒时下发第二个点，让第三关节旋转90度
结论: 在机器人没到达目标点前，下发新目标点，
机器人会放弃原目标点，直接向新的目标点运动
"""


def example_servoj(rpc_client):
    # 关节角，单位: 弧度
    q = [0.0 * (math.pi / 180), -15.0 * (math.pi / 180), 100.0 * (math.pi / 180),
         25.0 * (math.pi / 180), 90.0 * (math.pi / 180), 0.0 * (math.pi / 180)
         ]

    # 接口调用：获取机器人的名字
    robot_name = rpc_client.getRobotNames()[0]

    robot_interface = rpc_client.getRobotInterface(robot_name)

    # 接口调用: 设置机械臂的速度比率
    robot_interface.getMotionControl().setSpeedFraction(0.75)

    # 关节运动到路点 q
    robot_interface.getMotionControl() \
        .moveJoint(q, 80 * (math.pi / 180), 60 * (math.pi / 180), 0, 0)

    # 阻塞
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("关节运动到初始位置成功")
    else:
        print("关节运动到初始位置失败")

    # 接口调用: 开启Servo 模式
    robot_interface.getMotionControl().setServoMode(True)

    # 等待进入 Servo 模式
    i = 0
    while not robot_interface.getMotionControl().isServoModeEnabled():
        if i > 5:
            print("Servo 模式使能失败! 当前Servo 状态为 ", robot_interface
                  .getMotionControl()
                  .isServoModeEnabled())
            return -1
        time.sleep(0.005)
        i += 1

    q1 = [-120.0 * (math.pi / 180), -15.0 * (math.pi / 180),
          100.0 * (math.pi / 180), 25.0 * (math.pi / 180),
          90.0 * (math.pi / 180), 0.0 * (math.pi / 180)]
    print("向第一个目标点运动")
    # 接口调用: 关节伺服运动
    robot_interface.getMotionControl().servoJoint(q1, 0.0, 0.0, 10, 0.0, 0.0)

    time.sleep(5)

    q2 = [0, 0, 90.0 / 180 * math.pi, 0, 0, 0]
    print("向第二个目标点运动")
    # 接口调用: 关节伺服运动
    robot_interface.getMotionControl().servoJoint(q2, 0.0, 0.0, 10, 0.0, 0.0)

    # 等待运动结束
    while not robot_interface.getRobotState().isSteady():
        time.sleep(0.005)

    print("ServoJ 运动结束")

    # 关闭Servo 模式
    robot_interface.getMotionControl().setServoMode(False)

    # 等待结束 Servo 模式
    i = 0
    while robot_interface.getMotionControl().isServoModeEnabled():
        if i > 5:
            print("Servo 模式失能失败! 当前的 Servo 模式是 ",
                  robot_interface.getMotionControl().isServoModeEnabled())
            return -1
        time.sleep(0.005)
        i += 1

    return 0


if __name__ == '__main__':
    robot_rpc_client = pyaubo_sdk.RpcClient()
    robot_rpc_client.setRequestTimeout(1000)  # 接口调用: 设置 RPC 超时
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("RPC客户端连接成功!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 登录
        if robot_rpc_client.hasLogined():
            print("RPC客户端登录成功!")
            example_servoj(robot_rpc_client)  # ServoJ示例
            robot_rpc_client.logout()  # 退出登录
            robot_rpc_client.disconnect()  # 断开连接
