#! /usr/bin/env python
# coding=utf-8

"""
拖动示教

步骤:
第一步: 连接RPC服务器、登录
第二步: 进入拖动示教模式，经过 25s 后，退出模式
"""
import pyaubo_sdk
import time

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()


def exampleFreeDrive(cli):
    robot_name = cli.getRobotNames()[0]
    cli.getRuntimeMachine().start()
    # 接口调用: 发起机器人自由驱动请求
    cli.getRobotInterface(robot_name).getRobotManage().freedrive(True)
    time.sleep(25)
    # 接口调用: 结束机器人自由驱动请求
    cli.getRobotInterface(robot_name).getRobotManage().freedrive(False)
    pass


if __name__ == '__main__':
    robot_rpc_client.setRequestTimeout(1000)
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接到 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            exampleFreeDrive(robot_rpc_client)  # 进入拖动示教模式，经过 25s 后，退出模式
