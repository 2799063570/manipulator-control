#! /usr/bin/env python
# coding=utf-8

"""
trackJoint 运动

步骤:
第一步: 连接到 RPC 服务、机械臂登录、设置 RPC 请求超时时间
第二步: 读取 .offt 轨迹文件
第三步: 关节运动到轨迹中的第一个点
第五步: 做 trackJoint 运动
第六步: 停止 trackJoint 运动
第七步: 断开 RPC 连接
"""
import pyaubo_sdk
import time

robot_ip = "127.0.0.1"  # 服务器 IP 地址
robot_port = 30004  # 端口号
M_PI = 3.14159265358979323846
robot_rpc_client = pyaubo_sdk.RpcClient()

# 阻塞
def wait_arrival(robot_interface):
    max_retry_count = 5
    cnt = 0

    # 接口调用: 获取当前的运动指令 ID
    exec_id = robot_interface.getMotionControl().getExecId()

    # 等待机械臂开始运动
    while exec_id == -1:
        if cnt > max_retry_count:
            return -1
        time.sleep(0.05)
        cnt += 1
        exec_id = robot_interface.getMotionControl().getExecId()

    # 等待机械臂运动完成
    while robot_interface.getMotionControl().getExecId() != -1:
        time.sleep(0.05)

    return 0

def trackJ():
    robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
    robot = robot_rpc_client.getRobotInterface(robot_name)

    # 读取轨迹文件并加载轨迹点
    file = open('../c++/trajs/record6.offt')
    traj = []
    for line in file:
        str_list = line.split(",")
        float_list = []
        for strs in str_list:
            float_list.append(float(strs))
        traj.append(float_list)

    traj_sz = len(traj)
    if traj_sz == 0:
        print("没有轨迹点")
    else:
        print("加载的轨迹点数量为: ", traj_sz)

    # 关节运动到第一个点
    # 当前位置要与轨迹中的第一个点一致，否则容易引起较大超调
    print("goto p1")
    mc = robot.getMotionControl()
    mc.setSpeedFraction(0.8)
    mc.moveJoint(traj[0], M_PI, M_PI, 0., 0.)
    wait_arrival(robot)

    # 做 trackJoint 运动
    traj.remove(traj[0])
    for q in traj:
        traj_queue_size = mc.getTrajectoryQueueSize()
        print("traj_queue_size: ", traj_queue_size)
        while traj_queue_size > 8:
             traj_queue_size = mc.getTrajectoryQueueSize()
             time.sleep(0.001)
        mc.trackJoint(q, 0.02, 0.5, 1)

    # 停止 trackJoint 运行
    mc.stopJoint(1)

    # 等待运动结束
    is_steady = robot.getRobotState().isSteady()
    while is_steady is False:
        is_steady = robot.getRobotState().isSteady()
        time.sleep(0.005)
    
    print("trackJoint end")
    return 0


if __name__ == '__main__':
    robot_rpc_client.setRequestTimeout(1000)  # 接口调用: 设置 RPC 请求超时时间
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接到 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            trackJ()
            robot_rpc_client.disconnect()  # 接口调用: 断开RPC连接


