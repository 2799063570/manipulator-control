#ifndef EXAMPLE_1_H
#define EXAMPLE_1_H

#include "AuboRobotMetaType.h"
#include "serviceinterface.h"

/**
 * @brief The Example_1 class
 *
 * About the acquisition of the relevant state of the robot arm
 *
 */

class Example_1
{
public:
    // Used to get real-time waypoint callback function
    static void RealTimeWaypointCallback(
        const aubo_robot_namespace::wayPoint_S *wayPointPtr, void *arg);

    // Get real-time end speed callback function
    static void RealTimeEndSpeedCallback(double speed, void *arg);

    // Get real-time robotic arm event callback function
    static void RealTimeEventInfoCallback(
        const aubo_robot_namespace::RobotEventInfo *pEventInfo, void *arg);

    static void RealTimeJointStatusCallback(
        const aubo_robot_namespace::JointStatus *jointStatus, int len,
        void *arg);

    /**
     * @brief demo
     * 回调函数的方式获取实时路点，末端速度，机械臂的事件
     */
    static void demo();

    static void getJointStatus();
};

#endif // EXAMPLE_1_H
