#! /usr/bin/env python
# coding=utf-8

"""
机械臂圆弧运动

步骤:
第一步: 连接到 RPC 服务
第二步: 机械臂登录
第三步: 圆弧运动
"""

import time
import math
import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()

waypoint1 = []  # 圆弧轨迹的起始路点（位姿）
waypoint2 = []  # 圆弧轨迹的中间路点（位姿）
waypoint3 = []  # 圆弧轨迹的终止路点（位姿）

# 获取圆弧轨迹的路点
# 依据 (x-0.12294)^2 + (y-0.54855)^2 + (z-0.26319)^2 = 0.2^2 来计算
def get_circle_waypoint():
    global waypoint1
    global waypoint2
    global waypoint3

    z1 = 0.26319
    y1 = 0.7
    x1 = math.sqrt(pow(0.2, 2) - pow(y1 - 0.54855, 2) - pow(z1 - 0.26319, 2))
    waypoint1 = [x1, y1, z1, 3.14, 0.00, 3.14]

    z2 = 0.26319
    y2 = 0.55
    x2 = math.sqrt(pow(0.2, 2) - pow(y2 - 0.54855, 2) - pow(z2 - 0.26319, 2))
    waypoint2 = [x2, y2, z2, 3.14, 0.00, 3.14]

    z3 = 0.26319
    y3 = 0.35
    x3 = math.sqrt(pow(0.2, 2) - pow(y3 - 0.54855, 2) - pow(z3 - 0.26319, 2))
    waypoint3 = [x3, y3, z3, 3.14, 0.00, 3.14]

# 圆弧运动
def example_movec(robot_name):
    get_circle_waypoint()
    robot_rpc_client.getRobotInterface(robot_name).getMotionControl()\
        .moveLine(waypoint1, 180 * (M_PI / 180), 1000000 * (M_PI / 180), 0, 0)  # 接口调用: 直线运动
    time.sleep(1)

    ret = robot_rpc_client.getRobotInterface(robot_name).getMotionControl()\
        .moveCircle(waypoint2, waypoint3, 180 * (M_PI / 180), 1000000 * (M_PI / 180), 0, 0) # 接口调用: 圆弧运动

    if ret == 0:
        print("圆弧运动成功!")
    else:
        print("圆弧运动失败！错误码:%d", ret)


if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
            example_movec(robot_name)  # 圆弧运动
