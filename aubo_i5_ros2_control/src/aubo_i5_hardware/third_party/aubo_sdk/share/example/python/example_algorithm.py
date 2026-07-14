#! /usr/bin/env python
# coding=utf-8

"""
机械臂正逆解

步骤:
第一步: 连接到 RPC 服务
第二步: 机械臂登录
第三步: 机械臂逆解
第四步: 机械臂正解
"""

import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()

# 逆解
def exampleInverseK(robot_name):
    robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
    print("逆解")
    # 机械臂目标位姿
    waypoint_1_p = [0.549273, -0.12094,   0.464501,
                    3.13802,  0.00122674, 1.57095]
    # 机械臂参考关节角, 单位: 弧度
    waypoint_0_q = [0.00123059, -0.26063, 1.7441,
                    0.437503,   1.56957,  0.00107586]
    # 接口调用: 逆解
    res = robot_rpc_client.getRobotInterface(robot_name).getRobotAlgorithm().inverseKinematics(waypoint_0_q, waypoint_1_p)
    print("逆解函数返回值:", res[1])
    print("逆解得到的关节角:", res[0])
    pass

# 正解
def exampleForwardK(robot_name):
    robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
    # 机械臂目标关节角
    q = [0.0012302916520035012, -0.17709153384169424, 1.3017793878477586,
         -0.08835407161244224, 1.5695657489560777, 0.0010722296812234074]
    # 接口调用: 正解
    res = robot_rpc_client.getRobotInterface(robot_name).getRobotAlgorithm().forwardKinematics(q)
    print("正解函数返回值:", res[1])
    print("正解得到的位姿:", res[0])
    pass


if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
            exampleInverseK(robot_name)  # 机械臂逆解
            exampleForwardK(robot_name)  # 机械臂正解


