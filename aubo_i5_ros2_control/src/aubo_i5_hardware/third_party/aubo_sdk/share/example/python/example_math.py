#! /usr/bin/env python
# coding=utf-8

"""
机械臂欧拉角与四元数转换

步骤:
第一步: 连接到 RPC 服务
第二步: 机械臂登录
第三步: 欧拉角转四元数
第四步: 四元数转欧拉角
"""
import time

import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()


# 欧拉角转四元数
def exampleRpyToQuat():
    print("欧拉角转四元数")
    # 欧拉角
    rpy = [0.611, 0.785, 0.960]
    # 接口调用: 欧拉角转四元数
    quat = robot_rpc_client.getMath().rpyToQuaternion(rpy)
    print("四元数:", quat)


# 四元数转欧拉角
def exampleQuatToRpy():
    print("四元数转欧拉角")
    # 四元数
    quat = [0.834721517970497, 0.07804256900772265, 0.4518931575790371, 0.3048637712043723]
    # 接口调用: 四元数转欧拉角
    rpy = robot_rpc_client.getMath().quaternionToRpy(quat)
    print("欧拉角:", rpy)
    pass


# 已知法兰中心在Base下的位姿，求得TCP在Base下的位姿
def example_flange_to_tcp():
    # 接口调用: 获取机器人的名字
    robot_name = robot_rpc_client.getRobotNames()[0]
    robot_interface = robot_rpc_client.getRobotInterface(robot_name)
    # 接口调用: 设置TCP偏移
    tcp_offset = [0.01, 0.02, 0.03, 0.1, 0.2, 0.0]
    robot_interface.getRobotConfig().setTcpOffset(tcp_offset)
    time.sleep(0.1)
    # 接口调用: 获取法兰中心在Base下的当前位置姿态
    actual_flange_pose_on_base = robot_interface.getRobotState().getToolPose()
    print(f"法兰中心在Base下的当前位置姿态:{actual_flange_pose_on_base}")
    # 接口调用: 根据法兰中心在Base下的位置姿态，计算获得TCP在Base下的位置姿态
    actual_tcp_pose_on_base = robot_rpc_client.getMath().poseTrans(actual_flange_pose_on_base, tcp_offset)
    print(f"TCP在Base下的位置姿态:{actual_tcp_pose_on_base}")


# 已知TCP在Base下的位姿，求得法兰中心在Base下的位姿
def example_tcp_to_flange():
    # 接口调用: 获取机器人的名字
    robot_name = robot_rpc_client.getRobotNames()[0]
    robot_interface = robot_rpc_client.getRobotInterface(robot_name)
    # 接口调用: 设置TCP偏移
    tcp_offset = [0.01, 0.02, 0.03, 0.1, 0.2, 0.0]
    robot_interface.getRobotConfig().setTcpOffset(tcp_offset)
    time.sleep(0.1)
    # 接口调用: 获取TCP在Base下的当前位置姿态
    actual_tcp_pose_on_base = robot_interface.getRobotState().getTcpPose()
    print(f"TCP在Base下的当前位置姿态:{actual_tcp_pose_on_base}")
    # 接口调用: 根据TCP在Base下的位置姿态，计算获得法兰中心在Base下的位置姿态
    actual_flange_pose_on_base = robot_rpc_client.getMath().poseTrans(actual_tcp_pose_on_base, robot_rpc_client.getMath().poseInverse(tcp_offset))
    print(f"法兰中心在Base下的位置姿态:{actual_flange_pose_on_base}")


if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            exampleRpyToQuat()  # 欧拉角转四元数
            exampleQuatToRpy()  # 四元数转欧拉角
            example_flange_to_tcp()  # 已知法兰中心在Base下的位姿，求得TCP在Base下的位姿
            example_tcp_to_flange()  # 已知TCP在Base下的位姿，求得法兰中心在Base下的位姿
