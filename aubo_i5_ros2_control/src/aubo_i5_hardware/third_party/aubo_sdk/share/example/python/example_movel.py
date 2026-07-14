#! /usr/bin/env python
# coding=utf-8

"""
机械臂直线运动

步骤:
第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
第二步: 先关节运动起始位置，然后以直线运动的方式经过3个路点
第三步: RPC 退出登录、断开连接
"""
import time
import math
import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
robot_rpc_client = pyaubo_sdk.RpcClient()


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


# 先关节运动起始位置，然后循环依次以直线运动的方式经过3个路点
def example_movel(rpc_client):
    # 路点，用关节角来表示，单位: 弧度
    q = [0.0 * (math.pi / 180), -15.0 * (math.pi / 180), 100.0 * (math.pi / 180), 25.0 * (math.pi / 180),
         90.0 * (math.pi / 180), 0.0 * (math.pi / 180)]
    # 路点，用位姿来表示，形式[x,y,z,rx,ry,rz], 位置单位：米, 姿态单位：弧度
    pose1 = [0.551, -0.124, 0.419, -3.134, 0.004, 1.567]
    pose2 = [0.552, 0.235, 0.419, -3.138, 0.007, 1.567]
    pose3 = [0.551, -0.124, 0.261, -3.134, 0.0042, 1.569]

    # 接口调用：获取机器人的名字
    robot_name = rpc_client.getRobotNames()[0]

    robot_interface = rpc_client.getRobotInterface(robot_name)

    # 接口调用: 设置机械臂的速度比率
    robot_interface.getMotionControl().setSpeedFraction(0.75)

    # 接口调用: 设置工具中心点（TCP相对于法兰盘中心的偏移）
    tcp_offset = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    robot_interface.getRobotConfig().setTcpOffset(tcp_offset)

    # 关节运动到初始位置
    robot_interface.getMotionControl() \
        .moveJoint(q, 80 * (math.pi / 180), 60 * (math.pi / 180), 0, 0)

    # 阻塞
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("关节运动到初始位置成功")
    else:
        print("关节运动到初始位置失败")

    # 以直线运动的方式依次走3个路点
    # 直线运动
    robot_interface.getMotionControl() \
        .moveLine(pose1, 1.2, 0.25, 0, 0)

    # 阻塞
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("直线运动到路点1成功")
    else:
        print("直线运动到路点1失败")

    # 直线运动
    robot_interface.getMotionControl() \
        .moveLine(pose2, 1.2, 0.25, 0, 0)

    # 阻塞
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("直线运动到路点2成功")
    else:
        print("直线运动到路点2失败")

    # 直线运动
    robot_interface.getMotionControl() \
        .moveLine(pose3, 1.2, 0.25, 0, 0)

    # 阻塞
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("直线运动到路点3成功")
    else:
        print("直线运动到路点3失败")


if __name__ == '__main__':
    robot_rpc_client = pyaubo_sdk.RpcClient()
    robot_rpc_client.setRequestTimeout(1000)  # 接口调用: 设置 RPC 超时
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("RPC客户端连接成功!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 登录
        example_movel(robot_rpc_client)  # 先关节运动起始位置，然后以直线运动的方式经过3个路点
        robot_rpc_client.logout()  # 退出登录
        robot_rpc_client.disconnect()  # 断开连接
    else:
        print("RPC客户端连接失败!")
