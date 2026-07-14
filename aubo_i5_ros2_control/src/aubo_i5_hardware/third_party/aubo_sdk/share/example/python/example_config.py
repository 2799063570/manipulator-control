#! /usr/bin/env python
# coding=utf-8

"""
获取机械臂配置信息

步骤:
第一步: 连接到 RPC 服务
第二步: 机械臂登录
第三步: 获取机械臂配置信息
"""

import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()


# 获取机械臂相关配置
def exampleConfig(robot_name):
    robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
    # 接口调用: 获取机器人的名字
    name = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getName()
    print("机器人的名字:", name)
    # 接口调用: 获取机器人的自由度
    dof = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getDof()
    print("机器人的自由度:", dof)
    # 接口调用: 获取机器人的伺服控制周期（从硬件抽象层读取）
    cycle_time = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getCycletime()
    print("伺服控制周期:", cycle_time)
    # 接口调用: 获取默认的工具端加速度，单位: m/s^2
    default_tool_acc = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getDefaultToolAcc()
    print("默认的工具端加速度:", default_tool_acc)
    # 接口调用: 获取默认的工具端速度，单位: m/s
    default_tool_speed = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getDefaultToolSpeed()
    print("默认的工具端速度:", default_tool_speed)
    # 接口调用: 获取默认的关节加速度，单位: rad/s
    default_joint_acc = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getDefaultJointAcc()
    print("默认的关节加速度", default_joint_acc)
    # 接口调用: 获取默认的关节速度，单位: rad/s
    default_joint_speed = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getDefaultJointSpeed()
    print("默认的关节加速度", default_joint_speed)
    # 接口调用: 获取机器人类型代码
    robot_type = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getRobotType()
    print("机器人类型代码", robot_type)
    # 接口调用: 获取机器人子类型代码
    sub_robot_type = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getRobotSubType()
    print("机器人子类型代码", sub_robot_type)
    # 接口调用: 获取控制柜类型代码
    control_box_type = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getControlBoxType()
    print("控制柜类型代码", control_box_type)
    # 接口调用: 获取安装位姿(机器人的基坐标系相对于世界坐标系)
    mounting_pose = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getMountingPose()
    print("安装位姿(机器人的基坐标系相对于世界坐标系)", mounting_pose)
    # 接口调用: 设置碰撞灵敏度等级
    level = 6
    robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().setCollisionLevel(level)
    print("设置碰撞灵敏度等级:", level)
    # 接口调用: 获取碰撞灵敏度等级
    level = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getCollisionLevel()
    print("碰撞灵敏度等级", level)
    # 接口调用: 设置碰撞停止类型
    collision_stop_type = 1
    robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().setCollisionStopType(collision_stop_type)
    print("设置碰撞停止类型", collision_stop_type)
    # 接口调用: 获取碰撞停止类型
    collision_stop_type = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getCollisionStopType()
    print("碰撞停止类型", collision_stop_type)
    # 接口调用: 获取机器人DH参数
    kin_param = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getKinematicsParam(True)
    print("机器人DH参数", kin_param)
    # 接口调用: 获取指定温度下的DH参数补偿值
    temperature = 20
    kin_compensate = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getKinematicsCompensate(
        temperature)
    print("指定温度下的DH参数补偿值", kin_compensate)
    # 接口调用: 获取可用的末端力矩传感器的名字
    tcp_force_sensor_name = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getTcpForceSensorNames()
    print("可用的末端力矩传感器的名字", tcp_force_sensor_name)
    # 接口调用: 获取末端力矩偏移
    tcp_force_offset = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getTcpForceOffset()
    print("末端力矩偏移", tcp_force_offset)
    # 接口调用: 获取可用的底座力矩传感器的名字
    base_force_sensor_names = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getBaseForceSensorNames()
    print("可用的底座力矩传感器的名字", base_force_sensor_names)
    # 接口调用: 获取底座力矩偏移
    base_force_offset = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getBaseForceOffset()
    print("底座力矩偏移", base_force_offset)
    # 接口调用: 获取安全参数校验码 CRC32
    check_sum = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getSafetyParametersCheckSum()
    print("安全参数校验码 CRC32", check_sum)
    # 接口调用: 获取关节最大位置（物理极限）
    joint_max_positions = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getJointMaxPositions()
    print("关节最大位置（物理极限）", joint_max_positions)
    # 接口调用: 获取关节最小位置（物理极限）
    joint_min_positions = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getJointMinPositions()
    print("关节最小位置（物理极限）", joint_min_positions)
    # 接口调用: 获取关节最大速度（物理极限）
    joint_max_speeds = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getJointMaxSpeeds()
    print("关节最大速度（物理极限）", joint_max_speeds)
    # 接口调用: 获取关节最大加速度（物理极限）
    joint_max_acc = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getJointMaxAccelerations()
    print("关节最大加速度（物理极限）", joint_max_acc)
    # 接口调用: 获取TCP最大速度（物理极限）
    tcp_max_speeds = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getTcpMaxSpeeds()
    print("TCP最大速度（物理极限）", tcp_max_speeds)
    # 接口调用: 获取TCP最大加速度（物理极限）
    tcp_max_acc = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getTcpMaxAccelerations()
    print("TCP最大加速度（物理极限）", tcp_max_acc)
    # 接口调用:获取机器人的安装姿态
    gravity = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getGravity()
    print("机器人的安装姿态", gravity)
    # 接口调用: 获取TCP偏移
    tcp_offset = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getTcpOffset()
    print("TCP偏移", tcp_offset)
    # 接口调用: 获取末端负载
    payload = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getPayload()
    print("末端负载如下:")
    print("mass:", payload[0])
    print("cog:", payload[1])
    print("aom:", payload[2])
    print("inertia:", payload[3])
    # 接口调用: 获取固件升级的进程
    firmware_update_process = robot_rpc_client.getRobotInterface(robot_name).getRobotConfig().getFirmwareUpdateProcess()
    print("固件升级的进程", firmware_update_process)


if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
            exampleConfig(robot_name)  # 获取机械臂相关配置
