#include "example_5.h"
#include "example_1.h"

#include <stdlib.h>
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

void Example_5::demo()
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

    /** Business block **/
    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Interface call: Set the maximum acceleration of the articulated motion
     * ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] =
        50.0 / 180.0 * M_PI; ////The interface requires the unit to be radians
    robotService.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    /** Interface call: set the maximum speed of articulated motion ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] =
        50.0 / 180.0 * M_PI; ////The interface requires the unit to be radians
    robotService.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Robot arm movement to zero posture **/
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2; // Units m/s2
    robotService.robotServiceSetGlobalMoveEndMaxLineAcc(endMoveMaxAcc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleAcc(endMoveMaxAcc);

    /** Interface call: Set the maximum speed of the end type motion Linear
     * motion belongs to the end type motion***/
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2; // Units m/s
    robotService.robotServiceSetGlobalMoveEndMaxLineVelc(endMoveMaxVelc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleVelc(endMoveMaxVelc);

    double jointAngle[aubo_robot_namespace::ARM_DOF];

    for (int i = 0; i < 1; i++) {
        // MoveP
        robotService.robotServiceInitGlobalMoveProfile();

        robotService.robotServiceSetGlobalMoveEndMaxLineAcc(endMoveMaxAcc);
        robotService.robotServiceSetGlobalMoveEndMaxAngleAcc(endMoveMaxAcc);
        robotService.robotServiceSetGlobalMoveEndMaxLineVelc(endMoveMaxVelc);
        robotService.robotServiceSetGlobalMoveEndMaxAngleVelc(endMoveMaxVelc);

        robotService.robotServiceClearGlobalWayPointVector();
        for (int i = 0; i < 1; i++) {
            Util::initJointAngleArray(jointAngle, -1.63013, -0.727936, 0.558376,
                                      -0.279454, 1.66626, -1.86472);
            robotService.robotServiceAddGlobalWayPoint(jointAngle);
            Util::initJointAngleArray(jointAngle, -2.42089, -0.529301, 0.867566,
                                      -0.238376, 1.64145, -2.658);
            robotService.robotServiceAddGlobalWayPoint(jointAngle);
            Util::initJointAngleArray(jointAngle, -0.515177, -0.402876, 1.09792,
                                      0.0179699, 1.60827, -0.748361);
            robotService.robotServiceAddGlobalWayPoint(jointAngle);
        }
        robotService.robotServiceSetGlobalBlendRadius(0); // Blending radius
        ret = robotService.robotServiceTrackMove(
            aubo_robot_namespace::CARTESIAN_MOVEP, true);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "TrackMove failed.　ret:" << ret << std::endl;
        }
    }

    /** Robotic arm shutdown**/
    robotService.robotServiceRobotShutdown();

    /** Interface call: logout　**/
    robotService.robotServiceLogout();
}

void Example_5::speedl_demo()
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

    /** Business block **/
    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Interface call: Set the maximum acceleration of the articulated motion
     * ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] =
        50.0 / 180.0 * M_PI; ////The interface requires the unit to be radians
    robotService.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    /** Interface call: set the maximum speed of articulated motion ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] =
        50.0 / 180.0 * M_PI; ////The interface requires the unit to be radians
    robotService.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Robot arm movement to zero posture **/
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2; // Units m/s2
    robotService.robotServiceSetGlobalMoveEndMaxLineAcc(endMoveMaxAcc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleAcc(endMoveMaxAcc);

    /** Interface call: Set the maximum speed of the end type motion Linear
     * motion belongs to the end type motion***/
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2; // Units m/s
    robotService.robotServiceSetGlobalMoveEndMaxLineVelc(endMoveMaxVelc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleVelc(endMoveMaxVelc);

    double jointAngle[aubo_robot_namespace::ARM_DOF];

    std::cout << "Movement to waypoint 1" << std::endl;
    /** Waypoint 1 movement **/
    Util::initJointAngleArray(jointAngle, 0, -15.0 / 180.0 * M_PI,
                              100.0 / 180.0 * M_PI, 25.0 / 180.0 * M_PI,
                              90.0 / 180.0 * M_PI, 0);
    robotService.robotServiceJointMove(jointAngle, true);
    if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cerr << "Movement to waypoint 1 failed.　ret:" << ret << std::endl;
    }
    /** robotServiceSetTeachCoordinateSystem**/
    aubo_robot_namespace::CoordCalibrateByJointAngleAndTool
        userCoord;                                     //用户坐标系
    aubo_robot_namespace::ToolInEndDesc toolInEndDesc; //工具参数
    toolInEndDesc.toolInEndPosition.x = 0;
    toolInEndDesc.toolInEndPosition.y = 0;
    toolInEndDesc.toolInEndPosition.z = 0;
    toolInEndDesc.toolInEndOrientation.w = 1.0;
    toolInEndDesc.toolInEndOrientation.x = 0;
    toolInEndDesc.toolInEndOrientation.y = 0;
    toolInEndDesc.toolInEndOrientation.z = 0;

    userCoord.coordType = aubo_robot_namespace::WorldCoordinate;
    userCoord.methods = aubo_robot_namespace::
        Origin_AnyPointOnPositiveXAxis_AnyPointOnPositiveYAxis;
    // Origin_AnyPointOnPositiveXAxis_AnyPointOnFirstQuadrantOfXOYPlane;
    double radian_tem[6] = { 0,
                             -15 / 180.0 * M_PI,
                             100 / 180.0 * M_PI,
                             25 / 180.0 * M_PI,
                             90 / 180.0 * M_PI,
                             0 };
    memcpy(userCoord.wayPointArray[0].jointPos, radian_tem, sizeof(radian_tem));

    double radian_tem2[6] = { 9.578 / 180.0 * M_PI,   -13.35 / 180.0 * M_PI,
                              102.133 / 180.0 * M_PI, 25.484 / 180.0 * M_PI,
                              90 / 180.0 * M_PI,      9.578 / 180.0 * M_PI };
    memcpy(userCoord.wayPointArray[1].jointPos, radian_tem2,
           sizeof(radian_tem2));

    double radian_tem3[6] = { 0,
                              11.472 / 180.0 * M_PI,
                              127.443 / 180.0 * M_PI,
                              25.970 / 180.0 * M_PI,
                              90 / 180.0 * M_PI,
                              0 };
    memcpy(userCoord.wayPointArray[2].jointPos, radian_tem3,
           sizeof(radian_tem3));
    userCoord.toolDesc = toolInEndDesc;
    robotService.robotServiceSetTeachCoordinateSystem(userCoord);
    /** Robotic 开始示教**/
    std::cout << "TeachStart" << std::endl;
    robotService.robotServiceTeachStart(aubo_robot_namespace::MOV_X, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //    robotService.robotServiceRegisterRealTimeRoadPointCallback(
    //        Example_1::RealTimeWaypointCallback, NULL);
    robotService.robotServiceRegisterRobotEventInfoCallback(
        Example_1::RealTimeEventInfoCallback, NULL);
    //    robotService.robotServiceTeachStop();
    //    std::cout << "TeachStop" << std::endl;
}
void Example_5::circle_demo()
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

    /** Business block **/
    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Interface call: Set the maximum acceleration of the articulated motion
     * ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] = 50.0 / 180.0 * M_PI;
    robotService.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    /** Interface call: set the maximum speed of articulated motion ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] = 50.0 / 180.0 * M_PI;
    robotService.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Robot arm movement to zero posture **/
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2; // Units m/s2
    robotService.robotServiceSetGlobalMoveEndMaxLineAcc(endMoveMaxAcc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleAcc(endMoveMaxAcc);

    /** Interface call: Set the maximum speed of the end type motion Linear
     * motion belongs to the end type motion***/
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2; // Units m/s
    robotService.robotServiceSetGlobalMoveEndMaxLineVelc(endMoveMaxVelc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleVelc(endMoveMaxVelc);

    double jointAngle[aubo_robot_namespace::ARM_DOF];

    for (int i = 0; i < 1; i++) {
        robotService.robotServiceClearGlobalWayPointVector();

        Util::initJointAngleArray(jointAngle, 89.25 / 180.0 * M_PI,
                                  -35.33 / 180.0 * M_PI, 69.75 / 180.0 * M_PI,
                                  15.17 / 180.0 * M_PI, 90 / 180.0 * M_PI,
                                  -0.65 / 180.0 * M_PI);
        robotService.robotServiceJointMove(jointAngle, false);
        robotService.robotServiceAddGlobalWayPoint(jointAngle);
        Util::initJointAngleArray(jointAngle, 82 / 180.0 * M_PI,
                                  -18.04 / 180.0 * M_PI, 95.96 / 180.0 * M_PI,
                                  24.09 / 180.0 * M_PI, 90.01 / 180.0 * M_PI,
                                  -7.91 / 180.0 * M_PI);
        robotService.robotServiceAddGlobalWayPoint(jointAngle);
        Util::initJointAngleArray(jointAngle, 106.34 / 180.0 * M_PI,
                                  16.45 / 180.0 * M_PI, 131.01 / 180.0 * M_PI,
                                  24.64 / 180.0 * M_PI, 89.97 / 180.0 * M_PI,
                                  16.43 / 180.0 * M_PI);
        robotService.robotServiceAddGlobalWayPoint(jointAngle);

        robotService.robotServiceSetGlobalBlendRadius(0);
        ret =
            robotService.robotServiceTrackMove(aubo_robot_namespace::ARC, true);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "TrackMove failed.　ret:" << ret << std::endl;
        }
    }
}
void Example_5::spline_demo()
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

    /** Business block **/
    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Interface call: Set the maximum acceleration of the articulated motion
     * ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] = 50.0 / 180.0 * M_PI;
    robotService.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    /** Interface call: set the maximum speed of articulated motion ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 50.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] = 50.0 / 180.0 * M_PI;
    robotService.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    /** Interface call: Initialize motion properties ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** Robot arm movement to zero posture **/
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2; // Units m/s2
    robotService.robotServiceSetGlobalMoveEndMaxLineAcc(endMoveMaxAcc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleAcc(endMoveMaxAcc);

    /** Interface call: Set the maximum speed of the end type motion Linear
     * motion belongs to the end type motion***/
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2; // Units m/s
    robotService.robotServiceSetGlobalMoveEndMaxLineVelc(endMoveMaxVelc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleVelc(endMoveMaxVelc);

    double jointAngle[aubo_robot_namespace::ARM_DOF];

    for (int i = 0; i < 1; i++) {
        robotService.robotServiceClearGlobalWayPointVector();

        Util::initJointAngleArray(jointAngle, 89.25 / 180.0 * M_PI,
                                  -35.33 / 180.0 * M_PI, 69.75 / 180.0 * M_PI,
                                  15.17 / 180.0 * M_PI, 90 / 180.0 * M_PI,
                                  -0.65 / 180.0 * M_PI);
        robotService.robotServiceJointMove(jointAngle, true);
        robotService.robotServiceAddGlobalWayPoint(jointAngle);
        Util::initJointAngleArray(jointAngle, 82 / 180.0 * M_PI,
                                  -18.04 / 180.0 * M_PI, 95.96 / 180.0 * M_PI,
                                  24.09 / 180.0 * M_PI, 90.01 / 180.0 * M_PI,
                                  -7.91 / 180.0 * M_PI);
        robotService.robotServiceAddGlobalWayPoint(jointAngle);
        Util::initJointAngleArray(jointAngle, 106.34 / 180.0 * M_PI,
                                  16.45 / 180.0 * M_PI, 131.01 / 180.0 * M_PI,
                                  24.64 / 180.0 * M_PI, 89.97 / 180.0 * M_PI,
                                  16.43 / 180.0 * M_PI);
        robotService.robotServiceAddGlobalWayPoint(jointAngle);
        Util::initJointAngleArray(jointAngle, 107 / 180.0 * M_PI,
                                  21 / 180.0 * M_PI, 134 / 180.0 * M_PI,
                                  22 / 180.0 * M_PI, 90.01 / 180.0 * M_PI,
                                  -18 / 180.0 * M_PI);
        robotService.robotServiceAddGlobalWayPoint(jointAngle);
        Util::initJointAngleArray(
            jointAngle, 90 / 180.0 * M_PI, 2 / 180.0 * M_PI, 134 / 180.0 * M_PI,
            2 / 180.0 * M_PI, 90.01 / 180.0 * M_PI, -18 / 180.0 * M_PI);
        robotService.robotServiceAddGlobalWayPoint(jointAngle);

        robotService.robotServiceSetGlobalBlendRadius(0);
        ret = robotService.robotServiceTrackMove(
            aubo_robot_namespace::JOINT_GNUBSPLINEINTP, true);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "TrackMove failed.　ret:" << ret << std::endl;
        }
    }
}

void Example_5::OfflineTrack_demo()
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
    /** 清除路点 **/
    ret = robotService.robotServiceOfflineTrackWaypointClear();
    if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "robotServiceOfflineTrackWaypointClear failed"
                  << std::endl;
        return;
    }
    // char *path = get_current_dir_name(); //调用系统API获取当前路径
    /** 追加离线轨迹路点 **/
    ret = robotService.robotServiceOfflineTrackWaypointAppend(
        "/home/aubo/git/aubo_sdk/example/c++/trajs/record6.offt");

    /** 启动离线轨迹 **/
    if (ret == aubo_robot_namespace::ErrnoSucc) {
        std::cout << "upload waypoint success." << std::endl;

        ret = robotService.robotServiceOfflineTrackMoveStartup(true);

        if (ret != aubo_robot_namespace::ErrnoSucc) {
            std::cout << "failed." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        ret = robotService.robotServiceOfflineTrackMoveStop();
    } else {
        std::cout << "upload waypoint failed." << std::endl;
    }
}
