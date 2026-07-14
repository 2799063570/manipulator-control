import threading
import pyaubo_sdk

robot_ip = "127.0.0.1"
robot_port = 30010
M_PI = 3.14159265358979323846

robot_rtde_client = pyaubo_sdk.RtdeClient()
mutex = threading.Lock()

def subscribe_callback1(parser):
    mutex.acquire()
    actual_q_ = parser.popVectorDouble()
    print('@actual_q_: {}'.format(actual_q_))
    actual_current_ = parser.popVectorDouble()
    print('@actual_current_: {}'.format(actual_current_))
    robot_mode_ = parser.popRobotModeType()
    print('@robot_mode_: {}'.format(robot_mode_))
    safety_mode_ = parser.popSafetyModeType()
    print('@safety_mode_: {}'.format(safety_mode_))
    runtime_state_ = parser.popRuntimeState()
    print('@runtime_state_: {}'.format(runtime_state_))
    line_ = parser.popInt32()
    print('@line_: {}'.format(line_))
    actual_TCP_pose_ = parser.popVectorDouble()
    print('@actual_TCP_pose_: {}'.format(actual_TCP_pose_))
    mutex.release()

def subscribe_callback2(parser):
    mutex.acquire()
    joint_temperatures_ = parser.popVectorDouble()
    print('@joint_temperatures_: {}'.format(joint_temperatures_))
    joint_mode_ = parser.popVectorJointStateType()
    print('@joint_mode_: {}'.format(joint_mode_))
    actual_main_voltage_ = parser.popDouble()
    print('@actual_main_voltage_: {}'.format(actual_main_voltage_))
    actual_robot_voltage_ = parser.popDouble()
    print('@actual_robot_voltage_: {}'.format(actual_robot_voltage_))
    mutex.release()

def example_subscribe():
    names_list1 = ["R1_actual_q", "R1_actual_current", "R1_robot_mode",
                   "R1_safety_mode",
                   "runtime_state", "line_number", "R1_actual_TCP_pose"]
    topic1 = robot_rtde_client.setTopic(False, names_list1, 50, 0)
    robot_rtde_client.subscribe(topic1, subscribe_callback1)

    names_list2 = ["R1_joint_temperatures", "R1_joint_mode",
                   "R1_actual_main_voltage", "R1_actual_robot_voltage"]
    topic2 = robot_rtde_client.setTopic(False, names_list2, 1, 1)
    robot_rtde_client.subscribe(topic2, subscribe_callback2)

    isstop = input("Input 'stop' to end ")
    if isstop == 'stop':
        robot_rtde_client.removeTopic(False, topic1)
        robot_rtde_client.removeTopic(False, topic2)

if __name__ == '__main__':
    robot_rtde_client.connect(robot_ip, robot_port)
    if robot_rtde_client.hasConnected():
        print("Robot rtde_client connected successfully!")
        robot_rtde_client.login("aubo", "123456")
        if robot_rtde_client.hasLogined():
            print("Robot rtde_client logined successfully!")
            # write our examples here
            example_subscribe()
