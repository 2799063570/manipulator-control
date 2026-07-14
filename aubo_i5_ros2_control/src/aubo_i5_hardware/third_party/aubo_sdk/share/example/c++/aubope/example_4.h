#ifndef EXAMPLE_4_H
#define EXAMPLE_4_H

#include "AuboRobotMetaType.h"
#include "serviceinterface.h"

class Example_4
{
public:
    /**
     * @brief demo
     *
     * Linear motion
     */
    static void demo();

    static void demo_relativeOri();
    static void example_MoveLtoPosition();
    static void example_quaternion_RPY_Conversion();
    static void RealTimeWaypointCallback(
        const aubo_robot_namespace::wayPoint_S *wayPointPtr, void *arg);

    static aubo_robot_namespace::wayPoint_S s_currentWayPoing;
};

#endif // EXAMPLE_4_H
