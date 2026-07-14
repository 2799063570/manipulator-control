#! /usr/bin/env python
# coding=utf-8

"""
servoj 运动

步骤:
第一步: 连接到 RPC 服务、机械臂登录、设置RPC请求超时时间
第二步: 读取 .offt 轨迹文件
第三步: 关节运动到轨迹中的第一个点
第四步: 开启 servo 模式
第五步: 做 servoj 运动
第六步: 关闭 servo 模式
第七步: 断开 RPC 连接
"""
import pyaubo_sdk
import time

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

def servoj():
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
    robot_rpc_client.getRuntimeMachine().start()
    mc = robot.getMotionControl()
    mc.moveJoint(traj[0], M_PI, M_PI, 0., 0.)
    waitArrival(robot)

    # 开启 servo 模式
    robot.getMotionControl().setServoMode(True)
    i = 0
    while not mc.isServoModeEnabled():
        i = i + 1
        if i > 5:
            print("Servo Mode is ", mc.isServoModeEnabled())
            return -1
        time.sleep(0.005)

    traj.remove(traj[0])
    for q in traj:
        mc.servoJoint(q, 0.1, 0.2, 0.005, 0.1, 200)
        time.sleep(0.005)

    # 关闭 servo 模式
    mc.setServoMode(False)
    print("Servoj end")
    return 0


if __name__ == '__main__':
    robot_rpc_client.setRequestTimeout(1000)  # 接口调用: 设置 RPC 请求超时时间
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接到 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")
            servoj()
            robot_rpc_client.disconnect()  # 接口调用: 断开RPC连接

