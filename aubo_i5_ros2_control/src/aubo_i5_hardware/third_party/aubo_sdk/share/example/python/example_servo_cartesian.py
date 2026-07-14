#! /usr/bin/env python
# coding=utf-8

"""
servoCartesian 运动

步骤:
第一步: 连接到 RPC 服务、机械臂登录、设置RPC请求超时时间
第二步: 读取 .txt 轨迹文件
第三步: 关节运动到轨迹中的第一个点
第四步: 开启 servo 模式
第五步: 做 servoCartesian 运动
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


def servo_cartesian():
    robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字
    robot_interface = robot_rpc_client.getRobotInterface(robot_name)

    # 读取轨迹文件并加载轨迹点
    # 注意: cartesian-heart-L.txt是适用于S3系列的笛卡尔轨迹文件，
    # 且该文件中姿态单位是角度
    file = open('../c++/trajs/cartesian-heart-L.txt')
    traj = []
    for line in file:
        str_list = line.split(",")
        float_list = []
        for strs in str_list:
            float_list.append(float(strs))
        # 将轨迹文件中姿态的单位由角度转换成弧度
        for i in range(3, 6):
            float_list[i] = float_list[i] / 180 * M_PI
        traj.append(float_list)

    traj_sz = len(traj)
    if traj_sz == 0:
        print("没有轨迹点")
        return 0
    else:
        print("加载的轨迹点数量为: ", traj_sz)

    robot_name = robot_rpc_client.getRobotNames()[0]  # 接口调用: 获取机器人的名字

    # 接口调用：获取当前位置的关节角，单位：rad
    current_q = robot_rpc_client.getRobotInterface(robot_name).getRobotState().getJointPositions()
    # 接口调用: 逆解
    res = robot_rpc_client.getRobotInterface(robot_name).getRobotAlgorithm().inverseKinematics(current_q, traj[0])
    if res[1] != 0:
        print("轨迹文件中的第一个路点逆解失败，返回值：", res[1])
        return -1

    # q0 = [elem * 180 / 3.14 for elem in res[0]]
    # print("逆解得到的轨迹文件中的第一个路点关节角(单位为角度):", q0)

    # 关节运动到轨迹文件中的第一个点
    # 当前位置要与轨迹中的第一个点一致，否则容易引起较大超调
    mc = robot_interface.getMotionControl()
    mc.moveJoint(res[0], 80/180*M_PI, 60/180*M_PI, 0., 0.)
    ret = wait_arrival(robot_interface)
    if ret == 0:
        print("关节运动到路点1成功")
    else:
        print("关节运动到路点1失败")
        return -1

    # 开启 servo 模式
    robot_interface.getMotionControl().setServoMode(True)
    i = 0
    while not mc.isServoModeEnabled():
        i = i + 1
        if i > 5:
            print("开启Servo模式失败！当前的Servo模式是： ", mc.isServoModeEnabled())
            return -1
        time.sleep(0.005)

    traj.remove(traj[0])
    for p in traj:
        # 笛卡尔空间伺服运动
        mc.servoCartesian(p, 0.0, 0.0, 0.1, 0.0, 0.0)
        time.sleep(0.05)

    # 关闭 servo 模式
    mc.setServoMode(False)
    i = 0
    while mc.isServoModeEnabled():
        i = i + 1
        if i > 5:
            print("关闭Servo模式失败！当前的Servo模式是： ", mc.isServoModeEnabled())
            return -1
        time.sleep(0.005)
    print("servoCartesian运动结束")
    return 0


if __name__ == '__main__':
    robot_rpc_client.setRequestTimeout(1000)  # 接口调用: 设置 RPC 请求超时时间
    robot_rpc_client.connect(robot_ip, robot_port)  # 接口调用: 连接到 RPC 服务
    if robot_rpc_client.hasConnected():
        print("RPC连接成功！")
        robot_rpc_client.login("aubo", "123456")  # 接口调用: 机械臂登录
        servo_cartesian()  # 笛卡尔空间Servo运动示例
        robot_rpc_client.disconnect()  # 接口调用: 断开RPC连接
    else:
        print("RPC连接失败！")
