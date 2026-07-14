#! /usr/bin/env python
# coding=utf-8

"""
运行脚本

步骤:
第一步: 连接到 RPC 服务、机械臂登录
第二步: 连接到 SCRIPT 服务、机械臂登录
第三步: 运行脚本
第四步: 当规划期停止运行时，停止脚本运行
"""
import time

import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()
robot_script_client = pyaubo_sdk.ScriptClient()


def exampleScript():
    robot_script_client.sendFile("../aubo_script/example_movej.lua")
    pass


if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, 30004)  # 接口调用: 连接 RPC 服务
    robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
    robot_script_client.connect(robot_ip,30002)  # 接口调用： 连接 SCRIPT
    robot_script_client.login("aubo", "123456")  # 接口调用： 机械臂登录

    exampleScript()  # 运行脚本

    while True:
        time.sleep(1)
        runtime_state = robot_rpc_client.getRuntimeMachine().getStatus()
        if runtime_state != pyaubo_sdk.RuntimeState.Running:  # 当规划期停止运行时，停止脚本运行
            print("规划器的状态：", runtime_state)
            break
        print("规划器状态：", runtime_state)