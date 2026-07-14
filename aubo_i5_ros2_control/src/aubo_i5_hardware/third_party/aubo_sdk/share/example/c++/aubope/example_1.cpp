#include "example_1.h"
#include "thread"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include "util.h"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 30000

void Example_1::RealTimeJointStatusCallback(
    const aubo_robot_namespace::JointStatus *jointStatus, int len, void *arg)
{
    (void)arg;
    Util::Util::printJointStatus(jointStatus, len);
}

void Example_1::RealTimeWaypointCallback(
    const aubo_robot_namespace::wayPoint_S *wayPointPtr, void *arg)
{
    (void)arg;
    aubo_robot_namespace::wayPoint_S waypoint = *wayPointPtr;
    Util::printWaypoint(waypoint);
}

void Example_1::RealTimeEndSpeedCallback(double speed, void *arg)
{
    (void)arg;
    std::cout << "Real-time end speed:" << speed << std::endl;
}

void Example_1::RealTimeEventInfoCallback(
    const aubo_robot_namespace::RobotEventInfo *pEventInfo, void *arg)
{
    (void)arg;
    Util::printEventInfo(*pEventInfo);
}

void Example_1::demo()
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

    /** If the real robot arm is connected, the arm needs to be initialized.**/
    aubo_robot_namespace::ROBOT_SERVICE_STATE result;

    // Tool dynamics parameter
    aubo_robot_namespace::ToolDynamicsParam toolDynamicsParam;
    memset(&toolDynamicsParam, 0, sizeof(toolDynamicsParam));

    ret = robotService.rootServiceRobotStartup(
        toolDynamicsParam /**Tool dynamics parameter**/, 6 /*Collision level*/,
        true /*Whether to allow reading poses defaults to true*/,
        true,    /*Leave the default to true */
        1000,    /*Leave the default to 1000 */
        result); /*Robot arm initialization*/
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "Robot arm initialization succeeded." << std::endl;
    } else {
        std::cerr << "Robot arm initialization failed." << std::endl;
    }

    robotService.robotServiceRegisterRealTimeRoadPointCallback(
        Example_1::RealTimeWaypointCallback, NULL);

    //    robotService.robotServiceRegisterRealTimeEndSpeedCallback(
    //        Example_1::RealTimeEndSpeedCallback, NULL);

    //    robotService.robotServiceRegisterRobotEventInfoCallback(
    //        Example_1::RealTimeEventInfoCallback, NULL);

    //    robotService.robotServiceRegisterRealTimeJointStatusCallback(
    //        Example_1::RealTimeJointStatusCallback, NULL);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    /** Robotic arm shutdown **/
    robotService.robotServiceRobotShutdown();

    /** Interface call: logout　**/
    robotService.robotServiceLogout();
}

void Example_1::getJointStatus()
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

    /** If the real robot arm is connected, the arm needs to be initialized.**/
    aubo_robot_namespace::ROBOT_SERVICE_STATE result;

    // Tool dynamics parameter
    aubo_robot_namespace::ToolDynamicsParam toolDynamicsParam;
    memset(&toolDynamicsParam, 0, sizeof(toolDynamicsParam));

    ret = robotService.rootServiceRobotStartup(
        toolDynamicsParam /**Tool dynamics parameter**/, 6 /*Collision level*/,
        true /*Whether to allow reading poses defaults to true*/,
        true,    /*Leave the default to true */
        1000,    /*Leave the default to 1000 */
        result); /*Robot arm initialization*/
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "Robot arm initialization succeeded." << std::endl;
    } else {
        std::cerr << "Robot arm initialization failed." << std::endl;
    }

    aubo_robot_namespace::JointStatus jointStatus[6];
    ret = robotService.robotServiceGetRobotJointStatus(jointStatus, 6);
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "Get joint status successfully." << std::endl;

        Util::printJointStatus(jointStatus, 6);

    } else {
        std::cerr << "Failed to get joint state." << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}
