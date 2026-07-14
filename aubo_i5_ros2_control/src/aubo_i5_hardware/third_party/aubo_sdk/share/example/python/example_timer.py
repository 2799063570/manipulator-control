#! /usr/bin/env python
# coding=utf-8

"""
测试定时器功能: 开始、结束、重置、删除

步骤:
第一步: 连接RPC服务、机械臂登录
第二步: 例1
    1. 定时器 timer1 开始计时，经过 7.5 s，定时器 timer1 结束，获取时长约 7.5s
    2. 重复上面步骤，获取时长约 15.0 s
    3. 重置定时器 timer1，获取时长约 0.0 s，删除定时器 timer1
第三步: 例2
    1. 定时器 timer1 开始计时，经过 7.5 s，获取时长约 7.5s，重置定时器 timer1
    2. 定时器 timer1 开始计时，经过 7.5 s，获取时长约 7.5s，重置并删除定时器 timer1
"""

import time
import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()

if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
            impl = robot_rpc_client.getRobotInterface(robot_name)

            print("例1---------------")
            # 定时器 timer1 开始计时，经过 7.5 s，定时器 timer1 结束，获取时长约 7.5s
            robot_rpc_client.getRuntimeMachine().timerStart("timer1")
            time.sleep(7.5)
            robot_rpc_client.getRuntimeMachine().timerStop("timer1")
            timer1 = robot_rpc_client.getRuntimeMachine().getTimer("timer1")
            print("时长 = ", timer1)

            # 定时器 timer1 开始计时，经过 7.5 s，定时器 timer1 结束，获取时长约 15.0s
            robot_rpc_client.getRuntimeMachine().timerStart("timer1")
            time.sleep(7.5)
            robot_rpc_client.getRuntimeMachine().timerStop("timer1")
            timer1 = robot_rpc_client.getRuntimeMachine().getTimer("timer1")
            print("时长 = ", timer1)

            # 重置定时器 timer1，获取时长约 0.0 s，删除定时器 timer1
            robot_rpc_client.getRuntimeMachine().timerReset("timer1")
            timer1 = robot_rpc_client.getRuntimeMachine().getTimer("timer1")
            print("时长 = ", timer1)
            robot_rpc_client.getRuntimeMachine().timerDelete("timer1")

            print("例2---------------")
            # 定时器 timer1 开始计时，经过 7.5 s，获取时长约 7.5s，重置定时器 timer1
            robot_rpc_client.getRuntimeMachine().timerStart("timer1")
            time.sleep(7.5)
            timer1 = robot_rpc_client.getRuntimeMachine().getTimer("timer1")
            print("时长 = ", timer1)
            robot_rpc_client.getRuntimeMachine().timerReset("timer1")

            # 定时器 timer1 开始计时，经过 7.5 s，获取时长约 7.5s，重置并删除定时器 timer1
            robot_rpc_client.getRuntimeMachine().timerStart("timer1")
            time.sleep(7.5)
            timer1 = robot_rpc_client.getRuntimeMachine().getTimer("timer1")
            print("时长 = ", timer1)
            robot_rpc_client.getRuntimeMachine().timerReset("timer1")
            robot_rpc_client.getRuntimeMachine().timerDelete("timer1")
