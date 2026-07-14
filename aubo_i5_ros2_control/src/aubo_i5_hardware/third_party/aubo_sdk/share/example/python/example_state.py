#! /usr/bin/env python
# coding=utf-8

"""
获取机械臂状态信息

步骤:
第一步: 连接到 RPC 服务
第二步: 机械臂登录
第三步: 获取机械臂状态信息
"""

import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()


# 获取机械臂状态信息
def exampleState(robot_name):
    # 接口调用: 获取机器人的模式状态
    robot_mode_type = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getRobotModeType()
    print("机器人的模式状态:", robot_mode_type)
    # 接口调用: 获取安全模式
    safety_mode_type = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getSafetyModeType()
    print("安全模式:", safety_mode_type)
    # 接口调用: 机器人是否已经停止下来
    is_steady = robot_rpc_client.getRobotInterface(robot_name).getRobotState().isSteady()
    print("机器人是否已经停止下来:", is_steady)
    # 接口调用: 机器人是否已经在安全限制之内
    is_within_safety_limits = robot_rpc_client.getRobotInterface(robot_name).getRobotState().isWithinSafetyLimits()
    print("机器人是否已经在安全限制之内:", is_within_safety_limits)
    # 接口调用: 获取TCP的位姿
    tcp_pose = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getTcpPose()
    print("TCP的位姿:", tcp_pose)
    # 接口调用: 获取当前目标位姿
    target_tcp_pose = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getTargetTcpPose()
    print("当前目标位姿:", target_tcp_pose)
    # 接口调用: 获取工具端的位姿（不带TCP偏移）
    tool_pose = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getToolPose()
    print("工具端的位姿（不带TCP偏移）:", tool_pose)
    # 接口调用: 获取TCP速度
    tcp_speed = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getTcpSpeed()
    print("TCP速度:", tcp_speed)
    # 接口调用: 获取TCP的力 / 力矩
    tcp_force = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getTcpForce()
    print("TCP的力 / 力矩:", tcp_force)
    # 接口调用: 获取肘部的位置
    elbow_postion = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getElbowPosistion()
    print("肘部的位置:", elbow_postion)
    # 接口调用: 获取肘部速度
    elbow_velocity = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getElbowVelocity()
    print("肘部速度:", elbow_velocity)
    # 接口调用: 获取基座力 / 力矩
    base_force = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getBaseForce()
    print("基座力 / 力矩:", base_force)
    # 接口调用: 获取TCP目标位姿
    tcp_target_pose = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getTcpTargetPose()
    print("TCP目标位姿:", tcp_target_pose)
    # 接口调用: 获取TCP目标速度
    tcp_target_speed = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getTcpTargetSpeed()
    print("TCP目标速度:", tcp_target_speed)
    # 接口调用: 获取TCP目标力 / 力矩
    tcp_target_force = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getTcpTargetForce()
    print("TCP目标力 / 力矩:", tcp_target_force)
    # 接口调用: 获取机械臂关节标志
    joint_state = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointState()
    print("机械臂关节标志:", joint_state)
    # 接口调用: 获取关节的伺服状态
    joint_servo_mode = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointServoMode()
    print("关节的伺服状态:", joint_servo_mode)
    # 接口调用: 获取机械臂关节角度
    joint_positions = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointPositions()
    print("机械臂关节角度:", joint_positions)
    # 接口调用: 获取机械臂关节速度
    joint_speeds = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointSpeeds()
    print("机械臂关节速度:", joint_speeds)
    # 接口调用: 获取机械臂关节加速度
    joint_acc = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointAccelerations()
    print("机械臂关节加速度:", joint_acc)
    # 接口调用: 获取机械臂关节力矩
    joint_torque_sensors = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointTorqueSensors()
    print("机械臂关节力矩:", joint_torque_sensors)
    # 接口调用: 获取底座力传感器读数
    base_force_sensors = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getBaseForceSensor()
    print("底座力传感器读数:", base_force_sensors)
    # 接口调用: 获取TCP力传感器读数
    tcp_force_sensors = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getTcpForceSensors()
    print("TCP力传感器读数:", tcp_force_sensors)
    # 接口调用: 获取机械臂关节电流
    joint_currents = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointCurrents()
    print("机械臂关节电流:", joint_currents)
    # 接口调用: 获取机械臂关节电压
    joint_voltages = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointVoltages()
    print("机械臂关节电压:", joint_voltages)
    # 接口调用: 获取机械臂关节温度
    joint_temperatures = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointTemperatures()
    print("械臂关节温度:", joint_temperatures)
    # 接口调用: 获取关节 UniqueId
    joint_unique_ids = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointUniqueIds()
    print("关节 UniqueId:", joint_unique_ids)
    # 接口调用: 获取关节固件版本
    joint_firmware_versions = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointFirmwareVersions()
    print("关节固件版本:", joint_firmware_versions)
    # 接口调用: 获取关节硬件版本
    joint_hardward_versions = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointHardwareVersions()
    print("关节硬件版本:", joint_hardward_versions)
    # 接口调用: 获取主板 UniqueId
    master_board_unique_id = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getMasterBoardUniqueId()
    print("主板 UniqueId:", master_board_unique_id)
    # 接口调用: 获取MasterBoard固件版本
    master_board_firmware_version = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getMasterBoardFirmwareVersion()
    print("MasterBoard固件版本:", master_board_firmware_version)
    # 接口调用: 获取MasterBoard硬件版本
    master_board_hardware_version = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getMasterBoardHardwareVersion()
    print("MasterBoard硬件版本:", master_board_hardware_version)
    # 接口调用: 获取从板 UniqueId
    slave_board_unique_id = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getSlaveBoardUniqueId()
    print("从板 UniqueId:", slave_board_unique_id)
    # 接口调用: 获取SlaveBoard固件版本
    slave_board_firmware_version = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getSlaveBoardFirmwareVersion()
    print("SlaveBoard固件版本:", slave_board_firmware_version)
    # 接口调用: 获取SlaveBoard硬件版本
    slave_board_hardware_version = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getSlaveBoardHardwareVersion()
    print("SlaveBoard硬件版本:", slave_board_hardware_version)
    # 接口调用: 获取工具端全球唯一ID
    tool_unique_id = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getToolUniqueId()
    print("工具端全球唯一ID:", tool_unique_id)
    # 接口调用: 获取工具端固件版本
    tool_firmware_version = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getToolFirmwareVersion()
    print("工具端固件版本:", tool_firmware_version)
    # 接口调用: 获取工具端硬件版本
    tool_hardware_version = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getToolHardwareVersion()
    print("工具端硬件版本:", tool_hardware_version)
    # 接口调用: 获取底座全球唯一ID
    pedestal_unique_id = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getPedestalUniqueId()
    print("底座全球唯一ID:", pedestal_unique_id)
    # 接口调用: 获取底座固件版本
    pedestal_firmware_version = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getPedestalFirmwareVersion()
    print("底座固件版本:", pedestal_firmware_version)
    # 接口调用: 获取底座硬件版本
    pedestal_hardware_version = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getPedestalHardwareVersion()
    print("底座硬件版本:", pedestal_hardware_version)
    # 接口调用: 获取机械臂关节目标位置角度
    joint_target_positions = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointTargetPositions()
    print("械臂关节目标位置角度:", joint_target_positions)
    # 接口调用: 获取机械臂关节目标速度
    joint_target_speeds = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointTargetSpeeds()
    print("机械臂关节目标速度:", joint_target_speeds)
    # 接口调用: 获取机械臂关节目标加速度
    joint_target_acc = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointTargetAccelerations()
    print("机械臂关节目标加速度:", joint_target_acc)
    # 接口调用: 获取机械臂关节目标力矩
    joint_target_torques = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointTargetTorques()
    print("机械臂关节目标力矩:", joint_target_torques)
    # 接口调用: 获取机械臂关节目标电流
    joint_target_currents = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointTargetCurrents()
    print("机械臂关节目标电流:", joint_target_currents)
    # 接口调用: 获取控制柜温度
    control_box_temperature = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getControlBoxTemperature()
    print("控制柜温度:", control_box_temperature)
    # 接口调用: 获取母线电压
    main_voltage = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getMainVoltage()
    print("母线电压:", main_voltage)
    # 接口调用: 获取母线电流
    main_current = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getMainCurrent()
    print("母线电流:", main_current)
    # 接口调用: 获取机器人电压
    robot_voltage = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getRobotVoltage()
    print("机器人电压:", robot_voltage)
    # 接口调用: 获取机器人电流
    robot_current = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getRobotCurrent()
    print("机器人电流:", robot_current)


if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
            exampleState(robot_name)  # 获取机械臂状态信息
