#! /usr/bin/env python
# coding=utf-8

"""
机械臂直线运动到奇异位置，RTDE订阅的示教器弹窗错误信息输出到控制台中

步骤:
第一步: 连接和登录RPC服务
第二步: 连接和登录RTDE服务
第三步: 设置RTDE订阅话题
第四步: 订阅话题，获取示教器弹窗的日志等级、错误码、错误信息
第五步: 直线运动到一个奇异点
"""
import os
import time
import sys
import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
rpc_port = 30004  # RPC端口号
rtde_port = 30010  # RTDE 端口号
M_PI = 3.14159265358979323846


# 阻塞
def waitArrival(impl):
    cnt = 0
    while impl.getMotionControl().getExecId() == -1:
        cnt += 1
        if cnt > 5:
            print("Motion fail!")
            return -1
        time.sleep(0.05)
        print("getExecId: ", impl.getMotionControl().getExecId())
    id = impl.getMotionControl().getExecId()
    while True:
        id1 = impl.getMotionControl().getExecId()
        if id != id1:
            break
        time.sleep(0.05)


# 直线运动到一个奇异点
def movel_singularity():
    robot_name = robot_rpc_client.getRobotNames()[0]
    # 关节角，单位: 弧度
    waypoint_1_q = [-2.40194, 0.103747, 1.7804, 0.108636, 1.57129, -2.6379]
    # 奇异位置，形式[x,y,z,rx,ry,rz]
    waypoint_2_p = [1000, 1000, 1000, 3.05165, 0.0324355, 1.80417]

    # 先关节运动到路点 waypoint_1，然后再直线运动到奇异点 waypoint_2
    print("Moving to waypoint_1...")
    robot_rpc_client.getRobotInterface(robot_name).getMotionControl() \
        .moveJoint(waypoint_1_q, 180 * (M_PI / 180), 1000000 * (M_PI / 180), 0, 0)
    waitArrival(robot_rpc_client.getRobotInterface(robot_name))
    print("Moving to waypoint_2...")
    ret = robot_rpc_client.getRobotInterface(robot_name).getMotionControl() \
        .moveLine(waypoint_2_p, 250 * (M_PI / 180), 1000 * (M_PI / 180), 0, 0)
    waitArrival(robot_rpc_client.getRobotInterface(robot_name))


# 在控制台中打印日志等级、错误码等信息
def printlog(level, source, code, content):
    level_names = ["Critical", "Error", "Warning", "Info", "Debug", "BackTrace"]
    sys.stderr.write(f"[{level_names[level.value]}] {source} - {code} {content}\n")


# 订阅回调函数，获取日志等级、错误码、错误信息
def subscribe_callback(parser):
    msgs = parser.popRobotMsgVector()
    for msg in msgs:
        # 获取错误码相对应的错误信息
        error_content = pyaubo_sdk.errorCode2Str(msg.code)
        for arg in msg.args:
            pos = error_content.find("{}")
            if pos != -1:
                error_content = error_content[:pos] + arg + error_content[pos + 2:]
            else:
                break
        printlog(msg.level, msg.source, msg.code, error_content)


if __name__ == '__main__':
    # 连接和登录RPC服务
    robot_rpc_client = pyaubo_sdk.RpcClient()
    robot_rpc_client.connect(robot_ip, rpc_port)
    robot_rpc_client.login("aubo", "123456")

    # 连接和登录RTDE服务
    robot_rtde_client = pyaubo_sdk.RtdeClient()
    robot_rtde_client.connect(robot_ip, rtde_port)
    robot_rtde_client.login("aubo", "123456")

    # 设置RTDE订阅话题
    topic = robot_rtde_client.setTopic(False, ["R1_message"], 200, 0)
    if topic < 0:
        print("Set topic fail!")
        os.exit()

    # 订阅话题，获取示教器弹窗的日志等级、错误码、错误信息
    robot_rtde_client.subscribe(topic, subscribe_callback)

    # 直线运动到一个奇异点
    movel_singularity()

    while True:
        time.sleep(1)
