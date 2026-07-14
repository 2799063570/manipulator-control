#! /usr/bin/env python
# coding=utf-8

"""
PathBuffer 运动
注: 离线轨迹文件里的轨迹是相对于用户坐标系的轨迹

步骤:
第一步: 连接到 RPC 服务、机械臂登录、设置RPC请求超时时间
第二步: 读取.csv轨迹文件并加载轨迹点
第三步: 将轨迹文件中的轨迹位姿转换成相对于基坐标系的位姿，
       逆解求出关节角，
       添加到 “rec” 缓存中
第四步: 关节运动到第一个点，
       执行缓存的路径
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


# 等待 MovePathBuffer 完成
def waitMovePathBufferFinished(impl):
    while impl.getMotionControl().getExecId() == -1:
        time.sleep(0.05)
    while True:
        ids = impl.getMotionControl().getExecId()
        if ids == -1:
            break
        time.sleep(0.05)


if __name__ == '__main__':
    robot_rpc_client.setRequestTimeout(1000)  # 接口调用: 设置 RPC 请求超时时间
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接到 RPC 服务
    if robot_rpc_client.hasConnected():
        print("Robot rcp_client connected successfully!")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        if robot_rpc_client.hasLogined():
            print("Robot rcp_client logined successfully!")

            robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
            robot = robot_rpc_client.getRobotInterface(robot_name)

            # 读取轨迹文件并加载轨迹点
            file = open('../c++/trajs/CoffeCappuccino-heart-R_filtered.csv')
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

            tcp_offset = [-0.093, 0.0007249, 0.152, 90.66 / 180.0 * M_PI, -0.7635 / 180.0 * M_PI, -91.49 / 180.0 * M_PI]

            coord_q0 = [-1.627214, -0.360182, 1.783065, -1.063912, -1.642473, -1.510366]
            coord_q1 = [-0.806772, -0.344142, 1.806783, -1.079834, -0.824188, -1.444983]
            coord_q2 = [-0.702519, -0.105247, 2.113576, -1.022056, -0.720357, -1.430912]

            robot.getRobotConfig().setTcpOffset(tcp_offset)  # 接口调用: 设置 TCP 偏移值
            time.sleep(1)

            coord_p0 = robot.getRobotAlgorithm().forwardKinematics(coord_q0)[0]
            coord_p1 = robot.getRobotAlgorithm().forwardKinematics(coord_q1)[0]
            coord_p2 = robot.getRobotAlgorithm().forwardKinematics(coord_q2)[0]

            coord_p = [coord_p0, coord_p1, coord_p2]
            # 求用户坐标系相对于基坐标系的位姿
            coord = robot_rpc_client.getMath().calibrateCoordinate(coord_p, 0)[0]

            print("TCP 偏移值: ", tcp_offset)
            print("用户坐标系: ", coord)

            current_q = robot.getRobotState().getJointPositions()
            traj_q = []
            for p in traj:
                # 已知相对于用户坐标系的轨迹和用户坐标系相对于基坐标系的位姿，
                # 求轨迹相对于基坐标系的位姿
                rp = robot_rpc_client.getMath().poseTrans(coord, p)
                # 逆解求关节角
                q = robot.getRobotAlgorithm().inverseKinematics(current_q, rp)[0]
                current_q = q
                traj_q.append(q)
                print("q: ", q)

            # 关节运动到第一个点
            print("goto p1")
            mc = robot.getMotionControl()
            mc.moveJoint(traj_q[0], M_PI, M_PI, 0., 0.)
            waitArrival(robot)

            print("pathBufferAlloc")
            # mc.pathBufferFree("rec")  # 接口调用: 释放路径缓存
            mc.pathBufferAlloc("rec", 3, traj_sz)  # 接口调用: 新建一个路径点缓存

            # 将 traj_q 中的路点添加到 “rec” 缓存中
            # 以每10个路点为一个列表来添加
            # 当未添加的路点数量小于或者等于10时，则作为最后一组列表来添加
            offset = 10
            it = 0
            while True:
                print("pathBufferAppend ", offset)
                mc.pathBufferAppend("rec", traj_q[it:it + 10:1])  # 接口调用: 向路径缓存添加路点
                it += 10
                if offset + 10 >= traj_sz:
                    print("pathBufferAppend ", traj_sz)
                    mc.pathBufferAppend("rec", traj_q[it:traj_sz:1])
                    break
                offset += 10

            interval = 0
            print("pathBufferEval ", mc.pathBufferEval("rec", [1, 1, 1, 1, 1, 1], [1, 1, 1, 1, 1, 1],
                                                       interval))  # 接口调用: 计算、优化等耗时操作，传入的参数相同时不会重新计算
            while not mc.pathBufferValid("rec"):
                print("pathBufferValid: ", mc.pathBufferValid("rec"))  # 接口调用: 指定名字的buffer是否有效
                time.sleep(0.005)
            mc.movePathBuffer("rec")  # 接口调用: 执行缓存的路径
            waitMovePathBufferFinished(robot_rpc_client.getRobotInterface(robot_name))
            print("end movePathBuffer")
            robot_rpc_client.disconnect()  # 接口调用: 断开RPC连接
