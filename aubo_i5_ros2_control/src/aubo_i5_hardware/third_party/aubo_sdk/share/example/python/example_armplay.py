#! /usr/bin/env python
# coding=utf-8

"""
PathBuffer 运动

步骤:
第一步: 连接到 RPC 服务、机械臂登录、设置RPC请求超时时间
第二步: 读取 JSON 文件
第三步: 关节运动到JSON文件中的第一个路点
第四步: 释放路径缓存
第五步: 新建一个路径点缓存 “rec”
第六步: 将 JSON 文件中的路点添加到 “rec” 缓存中
第七步: 计算、优化等耗时操作，传入的参数相同时不会重新计算
第八步: 判断"rec"这个buffer是否有效，
    有效则执行下一步；反之，则一直阻塞
第九步: 执行缓存的路径
第十步: 断开RPC连接

"""

import pyaubo_sdk
import time
import json
import sys

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()


# 阻塞
def waitArrival(impl):
    cnt = 0
    while impl.getMotionControl().getExecId() == -1:
        cnt += 1
        if cnt > 5:
            print("Motion fail!")
            return -1
        time.sleep(0.05)
        print("getExecId: ", impl.getMotionControl().getExecId())
    id = impl.getMotionControl().getExecId()
    while True:
        id1 = impl.getMotionControl().getExecId()
        if id != id1:
            break
        time.sleep(0.05)

# 等待 MovePathBuffer 完成
def waitMovePathBufferFinished(impl):
    while impl.getMotionControl().getExecId() == -1:
        time.sleep(0.05)
    while True:
        id = impl.getMotionControl().getExecId()
        if id == -1:
            break
        time.sleep(0.05)


if __name__ == '__main__':
    robot_rpc_client.setRequestTimeout(1000)  # 接口调用: 设置 RPC 超时
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接到 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")

            robot_rpc_client.setRequestTimeout(40)  # 接口调用: 设置RPC请求超时时间

            # 读取 JSON 文件
            f = open('../c++/trajs/aubo-joint-test-0929-10.armplay')
            input = json.load(f)
            traj = input['jointlist']
            interval = input['interval']
            print("采样时间间隔: ", interval)

            traj_sz = len(traj)
            if traj_sz == 0:
                print("没有路点")
                sys.exit()
            else:
                print("加载点的数量: ", len(traj))

            robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
            mc = robot_rpc_client.getRobotInterface(robot_name).getMotionControl()

            # 关节运动
            q1 = traj[0]
            print("q1: ", q1)
            print("goto p1")
            mc.moveJoint(q1, M_PI, M_PI, 0., 0.)
            waitArrival(robot_rpc_client.getRobotInterface(robot_name))

            print("pathBufferAlloc")
            mc.pathBufferFree("rec")  # 接口调用: 释放路径缓存
            mc.pathBufferAlloc("rec", 2, traj_sz)  # 接口调用: 新建一个路径点缓存

            # 将 traj 中的路点添加到 “rec” 缓存中
            # 以每10个路点为一个列表来添加
            # 当未添加的路点数量小于或者等于10时，则作为最后一组列表来添加
            offset = 10
            it = 0
            while True:
                print("pathBufferAppend ", offset)
                mc.pathBufferAppend("rec", traj[it:it + 10:1])  # 接口调用: 向路径缓存添加路点
                it += 10
                if offset + 10 >= traj_sz:
                    print("pathBufferAppend ", traj_sz)
                    mc.pathBufferAppend("rec", traj[it:traj_sz:1])
                    break
                offset += 10

            print("pathBufferEval ", mc.pathBufferEval("rec", [], [], interval))  # 接口调用: 计算、优化等耗时操作，传入的参数相同时不会重新计算
            while not mc.pathBufferValid("rec"):
                print("pathBufferValid: ", mc.pathBufferValid("rec"))  # 接口调用: 指定名字的buffer是否有效
                time.sleep(0.0005)
            mc.movePathBuffer("rec")  # 接口调用: 执行缓存的路径
            waitMovePathBufferFinished(robot_rpc_client.getRobotInterface(robot_name))
            print("end movePathBuffer")
            robot_rpc_client.disconnect()  # 接口调用: 断开RPC连接

