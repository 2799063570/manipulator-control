#ifndef UTIL_H
#define UTIL_H

#include "AuboRobotMetaType.h"
#include "serviceinterface.h"

class Util
{
public:
    /** Print waypoint information **/
    static void printWaypoint(aubo_robot_namespace::wayPoint_S &wayPoint);

    /** Print joint status information **/
    static void printJointStatus(
        const aubo_robot_namespace::JointStatus *jointStatus, int len);

    /** Print event information **/
    static void printEventInfo(
        const aubo_robot_namespace::RobotEventInfo &eventInfo);

    /** Print diagnostic information **/
    static void printRobotDiagnosis(
        const aubo_robot_namespace::RobotDiagnosis &robotDiagnosis);

    static void initJointAngleArray(double *array, double joint0, double joint1,
                                    double joint2, double joint3, double joint4,
                                    double joint5);
};

#endif // UTIL_H
