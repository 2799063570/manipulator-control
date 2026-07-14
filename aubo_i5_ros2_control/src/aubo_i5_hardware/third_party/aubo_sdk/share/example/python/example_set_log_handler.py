#! /usr/bin/env python
# coding=utf-8

"""
自定义日志的格式

步骤:
第一步: 自定义日志的格式
注意: 需要在连接服务器之前，调用 setLogHandler 函数。
否则，自定义的日志格式是不会生效的。
第二步: 连接到 RPC 服务
第三步: 机械臂登录
"""

import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()


# 打印日志内容
def print_log(level: int, filename: str, line: int, message: str):
    print("level:", level)
    print("filename:", filename)
    print("line:", line)
    print("message:", message)


# 自定义日志格式
def example_read_log():
    # 接口调用: 重写日志系统的格式
    robot_rpc_client.setLogHandler(print_log)


if __name__ == '__main__':
    example_read_log()  # 自定义日志的格式
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
