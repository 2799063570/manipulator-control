#! /usr/bin/env python
# coding=utf-8

"""
功能：机械臂关节运动

步骤:
第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
第二步: 设置运动速度比率、以关节运动的方式依次经过3个路点
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


# 以关节运动的方式依次经过3个路点
def example_movej(rpc_client):
    # 关节角，单位: 弧度
    q1 = [0.0 * (math.pi / 180), -15.0 * (math.pi / 180), 100.0 * (math.pi / 180),
          25.0 * (math.pi / 180), 90.0 * (math.pi / 180), 0.0 * (math.pi / 180)
          ]
    q2 = [35.92 * (math.pi / 180), -11.28 * (math.pi / 180), 59.96 * (math.pi / 180),
          -18.76 * (math.pi / 180), 90.0 * (math.pi / 180), 35.92 * (math.pi / 180)
          ]
    q3 = [41.04 * (math.pi / 180), -7.65 * (math.pi / 180), 98.80 * (math.pi / 180),
          16.44 * (math.pi / 180), 90.0 * (math.pi / 180), 11.64 * (math.pi / 180)
          ]

    # 接口调用：获取机器人的名字
    robot_name = rpc_client.getRobotNames()[0]

    robot_interface = rpc_client.getRobotInterface(robot_name)

    # 接口调用: 设置机械臂的速度比率
    robot_interface.getMotionControl().setSpeedFraction(0.75)

    # 关节运动到路点 q1
    robot_interface.getMotionControl() \
        .moveJoint(q1, 80 * (math.pi / 180), 60 * (math.pi / 180), 0, 0)

    # 阻塞
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("关节运动到路点1成功")
    else:
        print("关节运动到路点1失败")

    # 关节运动到路点 q2
    robot_interface.getMotionControl() \
        .moveJoint(q2, 80 * (math.pi / 180), 60 * (math.pi / 180), 0, 0)

    # 阻塞
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("关节运动到路点2成功")
    else:
        print("关节运动到路点2失败")

    # 关节运动到路点 q3
    robot_interface.getMotionControl() \
        .moveJoint(q3, 80 * (math.pi / 180), 60 * (math.pi / 180), 0, 0)

    # 阻塞
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("关节运动到路点3成功")
    else:
        print("关节运动到路点3失败")


if __name__ == '__main__':
    robot_rpc_client = pyaubo_sdk.RpcClient()
    robot_rpc_client.setRequestTimeout(1000)  # 接口调用: 设置 RPC 超时
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("RPC客户端连接成功!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 登录
        if robot_rpc_client.hasLogined():
            print("RPC客户端登录成功!")
            example_movej(robot_rpc_client)  # 以关节运动的方式依次经过3个路点
            robot_rpc_client.logout()  # 退出登录
            robot_rpc_client.disconnect()  # 断开连接
