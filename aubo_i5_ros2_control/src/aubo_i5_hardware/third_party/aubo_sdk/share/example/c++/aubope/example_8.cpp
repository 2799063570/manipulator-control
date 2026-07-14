#include "example_8.h"

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

using namespace aubo_robot_namespace;

void Example_8::demo()
{
    ServiceInterface robotService;
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;
    /** Interface call: login ***/
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    FK(robotService);
    IK(robotService);
}

void Example_8::FK(ServiceInterface &robotService)
{
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }
    aubo_robot_namespace::wayPoint_S wayPoint;

    double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };

    Util::initJointAngleArray(
        jointAngle, 0.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI, 90.0 / 180.0 * M_PI,
        0.0 / 180.0 * M_PI, 90.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI);

    ret = robotService.robotServiceRobotFk(
        jointAngle, aubo_robot_namespace::ARM_DOF, wayPoint);

    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        Util::printWaypoint(wayPoint);
    } else {
        std::cerr << "Calling the positive solution failed" << std::endl;
    }
}

void Example_8::IK(ServiceInterface &robotService)
{
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    aubo_robot_namespace::wayPoint_S wayPoint;

    double startPointJointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };

    Util::initJointAngleArray(startPointJointAngle, 0.0 / 180.0 * M_PI,
                              0.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI,
                              0.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI,
                              0.0 / 180.0 * M_PI);

    aubo_robot_namespace::Pos targetPosition;
    targetPosition.x = -0.400;
    targetPosition.y = -0.1215;
    targetPosition.z = 0.5476;

    aubo_robot_namespace::Rpy rpy;
    aubo_robot_namespace::Ori targetOri;

    rpy.rx = 180.0 / 180.0 * M_PI;
    rpy.ry = 0.0 / 180.0 * M_PI;
    rpy.rz = -90.0 / 180.0 * M_PI;

    robotService.RPYToQuaternion(rpy, targetOri);

    //
    ret = robotService.robotServiceRobotIk(startPointJointAngle, targetPosition,
                                           targetOri, wayPoint);

    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        Util::printWaypoint(wayPoint);
    } else {
        std::cerr << "Calling the inverse solution failed" << std::endl;
    }
}

int Example_8::toolPositionOriIk()
{
    ServiceInterface robotService;
    int ret = InterfaceCallSuccCode;
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    //工具描述  单位米 弧度
    aubo_robot_namespace::ToolInEndDesc toolInEndDesc;
    toolInEndDesc.toolInEndPosition.x = 0;
    toolInEndDesc.toolInEndPosition.y = 0;
    toolInEndDesc.toolInEndPosition.z = 0;
    toolInEndDesc.toolInEndOrientation.w = 1.0;
    toolInEndDesc.toolInEndOrientation.x = 0;
    toolInEndDesc.toolInEndOrientation.y = 0;
    toolInEndDesc.toolInEndOrientation.z = 0;

    Pos toolEndPosition;
    toolEndPosition.x = 0.4785;
    toolEndPosition.y = -0.1215;
    toolEndPosition.z = 0.4125;
    Rpy toolEndRpy;
    toolEndRpy.rx = 180 / 180 * M_PI;
    toolEndRpy.ry = 0;
    toolEndRpy.rz = 0;
    Ori toolEndQuaternionOnBase;
    robotService.RPYToQuaternion(toolEndRpy, toolEndQuaternionOnBase);
    //将用户提供的工具末端点转成法兰中心点  基转基去工具:将工具末端点
    //转为　法兰中心点
    aubo_robot_namespace::CoordCalibrateByJointAngleAndTool userCoord;
    userCoord.coordType = aubo_robot_namespace::BaseCoordinate;
    Pos flangePositionOnBase;
    Ori flangeQuaternionOnBase;
    if (robotService.userToBaseCoordinate(
            toolEndPosition, toolEndQuaternionOnBase, userCoord, toolInEndDesc,
            flangePositionOnBase,
            flangeQuaternionOnBase) != InterfaceCallSuccCode) {
        ret = ErrCode_UserToBaseConvertFailed;
    }
    //参考位置
    double referPointJointAngle[6];
    referPointJointAngle[0] = -0.000172 / 180 * M_PI;
    referPointJointAngle[1] = -7.291862 / 180 * M_PI;
    referPointJointAngle[2] = -75.694718 / 180 * M_PI;
    referPointJointAngle[3] = 21.596727 / 180 * M_PI;
    referPointJointAngle[4] = -89.999982 / 180 * M_PI;
    referPointJointAngle[5] = -0.000458 / 180 * M_PI;
    wayPoint_S wayPoint;
    //逆解:根据法兰的位置和姿态　求解　关节角
    //但是逆解一般为多个解，所以需要提供一个参考位置求解得一个最优解，一般参考位置指的是轨迹起点
    ret = robotService.robotServiceRobotIk(referPointJointAngle,
                                           flangePositionOnBase,
                                           flangeQuaternionOnBase, wayPoint);
    if (ret != InterfaceCallSuccCode) {
        ret = ErrCode_IkFailed;
        return ret;
    } else {
        std::cout << "IK success\n";
    }
    std::cout << "joint1:" << wayPoint.jointpos[0] * 180 / M_PI << std::endl;
    std::cout << "joint2:" << wayPoint.jointpos[1] * 180 / M_PI << std::endl;
    std::cout << "joint3:" << wayPoint.jointpos[2] * 180 / M_PI << std::endl;
    std::cout << "joint4:" << wayPoint.jointpos[3] * 180 / M_PI << std::endl;
    std::cout << "joint5:" << wayPoint.jointpos[4] * 180 / M_PI << std::endl;
    std::cout << "joint6:" << wayPoint.jointpos[5] * 180 / M_PI << std::endl;
    ret = robotService.robotServiceJointMove(wayPoint.jointpos, true);
    return ret;
}

int Example_8::baseToUserCoordinate()
{
    ServiceInterface robotService;
    int ret = InterfaceCallSuccCode;
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    using namespace aubo_robot_namespace;
    aubo_robot_namespace::Pos
        flangeCenterPositionOnBase; //基于基座标系的法兰盘中心位置信息
    aubo_robot_namespace::Ori
        flangeCenterOrientationOnBase; //基于基座标系的姿态信息
    aubo_robot_namespace::CoordCalibrateByJointAngleAndTool
        userCoord;                                     //用户坐标系
    aubo_robot_namespace::ToolInEndDesc toolInEndDesc; //工具参数
    aubo_robot_namespace::Pos
        toolEndPositionOnUserCoord; //基于用户座标系的工具末端位置信息
    aubo_robot_namespace::Ori
        toolEndOrientationOnUserCoord; //基于用户座标系的工具末端姿态信息

    flangeCenterPositionOnBase.x = 0.548870;
    flangeCenterPositionOnBase.y = -0.1215;
    flangeCenterPositionOnBase.z = 0.26319;

    aubo_robot_namespace::Rpy rpy;

    rpy.rx = 180.0 / 180.0 * M_PI;
    rpy.ry = 0.0 / 180.0 * M_PI;
    rpy.rz = 89.9996 / 180.0 * M_PI;

    robotService.RPYToQuaternion(rpy, flangeCenterOrientationOnBase);

    toolInEndDesc.toolInEndPosition.x = 0;
    toolInEndDesc.toolInEndPosition.y = 0;
    toolInEndDesc.toolInEndPosition.z = 0;
    toolInEndDesc.toolInEndOrientation.w = 1.0;
    toolInEndDesc.toolInEndOrientation.x = 0;
    toolInEndDesc.toolInEndOrientation.y = 0;
    toolInEndDesc.toolInEndOrientation.z = 0;

    userCoord.coordType = WorldCoordinate;
    userCoord.methods = aubo_robot_namespace::
        Origin_AnyPointOnPositiveXAxis_AnyPointOnFirstQuadrantOfXOYPlane;
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
    /** Interface call: baseToUserCoordinate ***/
    ret = robotService.baseToUserCoordinate(
        flangeCenterPositionOnBase, flangeCenterOrientationOnBase, userCoord,
        toolInEndDesc, toolEndPositionOnUserCoord,
        toolEndOrientationOnUserCoord);

    std::cerr << toolEndPositionOnUserCoord.x << ","
              << toolEndPositionOnUserCoord.y << ","
              << toolEndPositionOnUserCoord.z << std::endl;
    /** Interface call: userToBaseCoordinate ***/
    ret = robotService.userToBaseCoordinate(
        toolEndPositionOnUserCoord, toolEndOrientationOnUserCoord, userCoord,
        toolInEndDesc, flangeCenterPositionOnBase,
        flangeCenterOrientationOnBase);

    std::cerr << flangeCenterPositionOnBase.x << ","
              << flangeCenterPositionOnBase.y << ","
              << flangeCenterPositionOnBase.z << std::endl;

    return ret;
}

int Example_8::toolToEnd_demo()
{
    ServiceInterface robotService;
    int ret = InterfaceCallSuccCode;
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    aubo_robot_namespace::Ori tcpOriInEnd;
    tcpOriInEnd.w = 0.8535534;
    tcpOriInEnd.x = -0.1464466;
    tcpOriInEnd.y = 0.3535534;
    tcpOriInEnd.z = 0.3535534;

    aubo_robot_namespace::Ori toolOri;
    aubo_robot_namespace::Rpy rpy;
    rpy.rx = 180.0 / 180.0 * M_PI;
    rpy.ry = -45 / 180.0 * M_PI;
    rpy.rz = -134.99 / 180.0 * M_PI;
    robotService.RPYToQuaternion(rpy, toolOri);

    aubo_robot_namespace::Ori endOri;
    robotService.toolOrientation2EndOrientation(tcpOriInEnd, toolOri, endOri);
    std::cerr << endOri.w << "," << endOri.x << "," << endOri.y << ","
              << endOri.z << std::endl;
    return ret;
}
int Example_8::CoordinateCalibration_demo()
{
    ServiceInterface robotService;
    int ret = InterfaceCallSuccCode;
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    using namespace aubo_robot_namespace;
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

    userCoord.coordType = WorldCoordinate;
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
    double bInWPos[3], bInWOri[9], wInBPos[3];
    ret = robotService.robotServiceCheckUserCoordinate(userCoord);
    if (ret) {
        std::cerr << "Unable to calibrate a coordinate system" << std::endl;
        return ret;
    }
    ret = robotService.robotServiceUserCoordinateCalibration(userCoord, bInWPos,
                                                             bInWOri, wInBPos);
    std::cerr << bInWPos[0] << "," << bInWPos[1] << "," << bInWPos[2]
              << std::endl;
    for (int i = 0; i < 3; i++) //行
    {
        for (int j = 0; j < 3; j++) //列
        {
            std::cerr << bInWOri[i * 3 + j] << ",";
        }
        std::cerr << std::endl;
    }
    std::cerr << wInBPos[0] << "   " << wInBPos[1] << "    " << wInBPos[2]
              << std::endl;
    Ori result;
    ret = robotService.robotServiceOriMatrixToQuaternion(bInWOri, result);
    std::cerr << result.w << "," << result.x << "," << result.y << ","
              << result.z << std::endl;
    return ret;
}

int Example_8::FK_IK()
{
    ServiceInterface robotService;
    int ret = InterfaceCallSuccCode;
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    aubo_robot_namespace::wayPoint_S wayPoint;

    double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };

    Util::initJointAngleArray(
        jointAngle, 0.0 / 180.0 * M_PI, -15 / 180.0 * M_PI, 100 / 180.0 * M_PI,
        25 / 180.0 * M_PI, 90.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI);
    ret = robotService.robotServiceRobotFk(
        jointAngle, aubo_robot_namespace::ARM_DOF, wayPoint);
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        Util::printWaypoint(wayPoint);
    } else {
        std::cerr << "Calling the positive solution failed" << std::endl;
    }
    aubo_robot_namespace::wayPoint_S wayPoint1;

    robotService.robotServiceRobotIk(jointAngle, wayPoint.cartPos.position,
                                     wayPoint.orientation, wayPoint1);
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        Util::printWaypoint(wayPoint1);
    } else {
        std::cerr << "Calling the positive solution failed" << std::endl;
    }
}
