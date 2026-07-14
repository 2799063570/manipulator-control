#include "example_4.h"

#include <thread>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

#include "util.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 30000
aubo_robot_namespace::wayPoint_S Example_4::s_currentWayPoing;

void Example_4::RealTimeWaypointCallback(
    const aubo_robot_namespace::wayPoint_S *wayPointPtr, void *arg)
{
    (void)arg;
    s_currentWayPoing = *wayPointPtr;
    // Util::printWaypoint(s_currentWayPoing);
}

void Example_4::demo()
{
    ServiceInterface robotService;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }

    // Tool dynamics parameter
    aubo_robot_namespace::ToolDynamicsParam toolDynamicsParam;
    memset(&toolDynamicsParam, 0, sizeof(toolDynamicsParam));

    /** Interface call: Set the maximum acceleration of the articulated motion
     * ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] =
        50.0 / 180.0 * M_PI; // The interface requires the unit to be radians
    robotService.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    /** Interface call: set the maximum speed of articulated motion ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] =
        50.0 / 180.0 * M_PI; // The interface requires the unit to be radians
    robotService.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    /** Move to initial pose **/
    double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };
    Util::initJointAngleArray(
        jointAngle, 0.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI, 90.0 / 180.0 * M_PI,
        0.0 / 180.0 * M_PI, 90.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI);
    ret = robotService.robotServiceJointMove(jointAngle, true);
    if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cerr << "Movement to zero posture failure.　ret:" << ret
                  << std::endl;
    }

    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Interface call: Set the maximum acceleration of the end type motion
     * Linear motion belongs to the end type motion***/
    double lineMoveMaxAcc;
    lineMoveMaxAcc = 2; // Units m/s2
    robotService.robotServiceSetGlobalMoveEndMaxLineAcc(lineMoveMaxAcc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleAcc(lineMoveMaxAcc);

    /** Interface call: Set the maximum speed of the end type motion Linear
     * motion belongs to the end type motion***/
    double lineMoveMaxVelc;
    lineMoveMaxVelc = 2; // Units m/s
    robotService.robotServiceSetGlobalMoveEndMaxLineVelc(lineMoveMaxVelc);
    robotService.robotServiceGetGlobalMoveEndMaxAngleVelc(lineMoveMaxVelc);

    for (int i = 0; i > -1; i++) {
        double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };
        Util::initJointAngleArray(jointAngle, -1.63013, -0.727936, 0.558376,
                                  -0.279454, 1.66626, -1.86472);
        std::cout << "Moving to waypoint 0." << std::endl;
        ret = robotService.robotServiceLineMove(jointAngle, true);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "Movement to waypoint 0 failed.　ret:" << ret
                      << std::endl;
        }

        Util::initJointAngleArray(jointAngle, -2.42089, -0.529301, 0.867566,
                                  -0.238376, 1.64145, -2.658);
        std::cout << "Moving to waypoint 1." << std::endl;
        robotService.robotServiceLineMove(jointAngle, true);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "Movement to waypoint 1 failed.　ret:" << ret
                      << std::endl;
        }

        Util::initJointAngleArray(jointAngle, -0.515177, -0.402876, 1.09792,
                                  0.0179699, 1.60827, -0.748361);
        std::cout << "Moving to waypoint 2." << std::endl;
        robotService.robotServiceLineMove(jointAngle, true);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "Movement to waypoint 2 failed.　ret:" << ret
                      << std::endl;
        }
    }
}
