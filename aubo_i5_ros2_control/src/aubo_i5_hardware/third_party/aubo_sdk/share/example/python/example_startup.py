#! /usr/bin/env python
# coding=utf-8

"""
机械臂上电与断电

步骤:
第一步: 连接到 RPC 服务
第二步: 机械臂登录
第三步: 机械臂上电
第四步: 机械臂断电
"""

import pyaubo_sdk
import time

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()

# 机械臂上电
def exampleStartup():
    robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
    if 0 == robot_rpc_client.getRobotInterface(
            robot_name).getRobotManage().poweron():  # 接口调用: 发起机器人上电请求
        print("The robot is requesting power-on!")
        if 0 == robot_rpc_client.getRobotInterface(
                robot_name).getRobotManage().startup():  # 接口调用: 发起机器人启动请求
            print("The robot is requesting startup!")
            # 循环直至机械臂松刹车成功
            while 1:
                robot_mode = robot_rpc_client.getRobotInterface(robot_name) \
                    .getRobotState().getRobotModeType()  # 接口调用: 获取机器人的模式类型
                print("Robot current mode: %s" % (robot_mode.name))
                if robot_mode == pyaubo_sdk.RobotModeType.Running:
                    break
                time.sleep(1)

# 机械臂断电
def examplePoweroff():
    robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
    if 0 == robot_rpc_client.getRobotInterface(
            robot_name).getRobotManage().poweroff(): # 接口调用: 机械臂断电
        print("The robot is requesting power-off!")

if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接到 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            exampleStartup()  # 机械臂上电
            examplePoweroff()  # 机械臂断电
