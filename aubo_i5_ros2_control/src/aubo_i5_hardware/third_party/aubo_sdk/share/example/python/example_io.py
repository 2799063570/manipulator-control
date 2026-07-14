#! /usr/bin/env python
# coding=utf-8

"""
获取机械臂配置信息

步骤:
第一步: 连接到 RPC 服务
第二步: 机械臂登录
第三步: 标准数字IO:
    1.获取标准数字输入和输出数量
    2.设置和获取输入触发动作
    3.设置和获取输出状态选择
    4.获取标准数字输入和输出值
第四步: 标准模拟IO:
    1.获取标准模拟输入和输出数量
    2.设置和获取标准模拟输入范围
    3.获取标准模拟输入
    4.设置和获取标准模拟输出范围
    5.设置和获取标准模拟输出状态选择
    6.设置和获取标准模拟输出
第五步: 工具端数字IO
    1.获取工具端数字输入和输出数量
    2.设置和获取工具端数字输入触发动作
    3.设置和获取工具端数字输出状态选择
第六步: 工具端模拟IO
    1.获取工具端模拟输入和输出数量
    2.获取工具端模拟输入范围
    3.获取工具端模拟输入
"""

import pyaubo_sdk

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()


def exampleStandardDigitalIO(robot_name):
    # 接口调用： 获取标准数字输入数量
    input_num = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardDigitalInputNum()
    print("标准数字输入数量：", input_num)
    # 接口调用： 获取标准数字输出数量
    output_num = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardDigitalOutputNum()
    print("标准数字输出数量：", output_num)

    # 接口调用： 设置所有的输入触发动作为 Default
    robot_rpc_client.getRobotInterface(robot_name).getIoControl().setDigitalInputActionDefault()
    # 接口调用： 获取输入触发动作
    input_action = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardDigitalInputAction(0x00000001)
    print("获取输入触发动作为：", input_action)
    # 接口调用： 获取输入触发动作
    input_action = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardDigitalInputAction(0x00000001)
    print("获取输入触发动作为：", input_action)

    # 接口调用： 设置所有的输出状态选择为 NONE
    robot_rpc_client.getRobotInterface(robot_name).getIoControl().setDigitalOutputRunstateDefault()
    # 接口调用： 获取输出状态选择
    output_runstate = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardDigitalOutputRunstate(0x00000001)
    print("获取输出状态选择为：", output_runstate)

    # 接口调用： 获取标准数字输出状态选择
    output_runstate = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardDigitalOutputRunstate(0x00000001)
    print("获取输出状态选择为：", output_runstate)

    # 打印所有的标准数字输入值
    input_value = []
    for i in range(input_num):
        value = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardDigitalInput(i)
        input_value.append(value)
    print("输入值:", input_value)

    # 打印所有的标准数字输出值
    output_value = []
    for i in range(output_num):
        value = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardDigitalOutput(i)
        output_value.append(value)
    print("输出值:", output_value)


def exampleStandardAnalogIO(robot_name):
    # 接口调用： 获取标准模拟输入数量
    input_num = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardAnalogInputNum()
    print("标准模拟输入数量：", input_num)
    # 接口调用： 获取标准模拟输出数量
    output_num = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardAnalogOutputNum()
    print("标准模拟输出数量：", output_num)

    # 接口调用： 设置标准模拟输入范围
    input_domain = 15
    robot_rpc_client.getRobotInterface(robot_name).getIoControl().setStandardAnalogInputDomain(0, input_domain)
    # 接口调用： 获取标准模拟输入范围
    input_domain = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardAnalogInputDomain(0)
    print("标准模拟输入范围：", input_domain)

    # 接口调用： 设置标准模拟输出范围
    output_domain = 15
    robot_rpc_client.getRobotInterface(robot_name).getIoControl().setStandardAnalogOutputDomain(0, output_domain)
    # 接口调用： 获取标准模拟输出范围
    output_domain = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardAnalogOutputDomain(0)
    print("标准模拟输出范围：", output_domain)

    # 接口调用： 获取标准模拟输出状态选择
    output_runstate = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getStandardAnalogOutputRunstate(3)
    print("输出状态选择：", output_runstate)


def exampleToolDigitalIO(robot_name):
    # 接口调用： 获取工具端数字输入数量
    input_num = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolDigitalInputNum()
    print("工具端数字输入数量：", input_num)
    # 接口调用： 获取工具端数字输出数量
    output_num = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolDigitalOutputNum()
    print("工具端数字输出数量：", output_num)

    # 接口调用： 设置指定的工具端IO为输入
    robot_rpc_client.getRobotInterface(robot_name).getIoControl().setToolIoInput(1, True)
    # 接口调用： 判断指定的工具端IO是否为输入
    isInput = robot_rpc_client.getRobotInterface(robot_name).getIoControl().isToolIoInput(1)
    print("指定的工具端IO是否为输入：", isInput)
    # 接口调用： 获取工具端数字输入
    input = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolDigitalInput(1)
    print("工具端数字输入：", input)

    # 接口调用： 设置所有的工具端数字输入触发动作为 Default
    input_action = pyaubo_sdk.StandardInputAction.Default
    robot_rpc_client.getRobotInterface(robot_name).getIoControl().setToolDigitalInputAction(0, input_action)
    # 接口调用： 获取工具端数字输入触发动作
    input_action = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolDigitalInputAction(0)
    print("输入触发动作：", input_action)

    # 接口调用： 获取工具端数字输出状态选择
    output_runstate = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolDigitalOutputRunstate(3)
    print("输出状态选择：", output_runstate)

    # 接口调用： 设置工具端数字输出
    output = True
    robot_rpc_client.getRobotInterface(robot_name).getIoControl().setToolDigitalOutput(3, output)
    # 接口调用： 获取工具端数字输出
    output = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolDigitalOutput(3)
    print("工具端数字输出：", output)


def exampleToolAnalogIO(robot_name):
    # 接口调用： 获取标准模拟输入数量
    input_num = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolAnalogInputNum()
    print("标准模拟输入数量：", input_num)
    # 接口调用： 获取标准模拟输出数量
    output_num = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolAnalogOutputNum()
    print("标准模拟输出数量：", output_num)

    # 接口调用： 获取工具端模拟输入范围
    input_domain = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolAnalogInputDomain(1)
    print("工具端模拟输入范围：", input_domain)
    # 接口调用： 获取工具端模拟输入
    input_value = robot_rpc_client.getRobotInterface(robot_name).getIoControl().getToolAnalogInput(1)
    print("工具端模拟输入：", input_value)


if __name__ == '__main__':
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
            exampleStandardDigitalIO(robot_name)  # 获取标准数字IO
            exampleStandardAnalogIO(robot_name)  # 获取标准模拟IO
            exampleToolDigitalIO(robot_name)  # 获取工具端数字IO
            exampleToolAnalogIO(robot_name)  # 获取工具端模拟IO
