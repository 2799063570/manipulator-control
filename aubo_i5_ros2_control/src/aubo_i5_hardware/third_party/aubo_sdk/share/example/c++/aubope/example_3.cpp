#include "example_3.h"

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
void Example_3::demo()
{
    ServiceInterface robotservice;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotservice.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }

    std::cout << "Robot arm initialization....." << std::endl;

    /** If the real robot arm is connected, the arm needs to be initialized.**/
    aubo_robot_namespace::ROBOT_SERVICE_STATE result;

    /** Business block **/
    /** Interface call: Initialize motion properties ***/
    robotservice.robotServiceInitGlobalMoveProfile();

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
    robotservice.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    /** Interface call: set the maximum speed of articulated motion ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 80.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 80.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 80.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 80.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 80.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] =
        80.0 / 180.0 * M_PI; // The interface requires the unit to be radians
    robotservice.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    /** Robot arm movement to zero posture **/
    double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };
    Util::initJointAngleArray(jointAngle, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    std::cout << "Calling the motion function" << std::endl;
    ret = robotservice.robotServiceJointMove(jointAngle, true);
    if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cerr << "Movement to zero posture failure.　ret:" << ret
                  << std::endl;
    }

    for (int i = 0; i > -1; i++) {
        //        /** Interface call: set offset**/
        //        aubo_robot_namespace::MoveRelative relativeMoveOnBase;
        //        relativeMoveOnBase.ena = true;
        //        relativeMoveOnBase.relativePosition[0] = 0;
        //        relativeMoveOnBase.relativePosition[1] = 0;
        //        relativeMoveOnBase.relativePosition[2] = 0.05*(i%4);   //Unit:
        //        m

        //        relativeMoveOnBase.relativeOri.w=1;
        //        relativeMoveOnBase.relativeOri.x=0;
        //        relativeMoveOnBase.relativeOri.y=0;
        //        relativeMoveOnBase.relativeOri.z=0;
        //        robotservice.robotServiceSetMoveRelativeParam(relativeMoveOnBase);
        std::cout << "Movement to waypoint 1" << std::endl;
        /** Waypoint 1 movement **/
        Util::initJointAngleArray(jointAngle, 0.0 / 180.0 * M_PI,
                                  0.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI,
                                  0.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI,
                                  0.0 / 180.0 * M_PI);
        ret = robotservice.robotServiceJointMove(jointAngle, true);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "Movement to waypoint 1 failed.　ret:" << ret
                      << std::endl;
            break;
        }
        std::cout << "Movement to waypoint 2" << std::endl;
        /** Waypoint 2 movement **/
        Util::initJointAngleArray(jointAngle, 90.0 / 180.0 * M_PI,
                                  0.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI,
                                  0.0 / 180.0 * M_PI, 0.0 / 180.0 * M_PI,
                                  0.0 / 180.0 * M_PI);
        ret = robotservice.robotServiceJointMove(jointAngle, true);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "Movement to waypoint 2 failed.　ret:" << ret
                      << std::endl;
            break;
        }
        aubo_robot_namespace::ForceSensorData data;
        ret = robotservice.robotServerGetToolForceSensorData(data);
        double tmp[6];
        tmp[0] = data.data[0];
        tmp[1] = data.data[1];
        tmp[2] = data.data[2];
        tmp[3] = data.data[3];
        tmp[4] = data.data[4];
        tmp[5] = data.data[5];
        std::cout << "forcesenser data : " << ret << std::endl;

        // for(int j = 0; j < 6; j++) {
        //    std::cout<<"data"<<j<<":"<<data.data[j]<<std::endl;
        //}
        std::cout << "data" << 0 << ":" << tmp[0] << std::endl;
        std::cout << "data" << 1 << ":" << tmp[1] << std::endl;
        std::cout << "data" << 2 << ":" << tmp[2] << std::endl;
        std::cout << "data" << 3 << ":" << tmp[3] << std::endl;
        std::cout << "data" << 4 << ":" << tmp[4] << std::endl;
        std::cout << "data" << 5 << ":" << tmp[5] << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    /** Robotic arm shutdown **/
    robotservice.robotServiceRobotShutdown();

    /** Interface call: logout　**/
    robotservice.robotServiceLogout();
}
void Example_3::Relative_demo()
{
    ServiceInterface robotservice;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotservice.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }

    std::cout << "Robot arm initialization....." << std::endl;

    /** Interface call: Initialize motion properties ***/
    robotservice.robotServiceInitGlobalMoveProfile();

    /** Interface call: Set the maximum acceleration of the articulated motion
     * ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] =
        10.0 / 180.0 * M_PI; // The interface requires the unit to be radians
    robotservice.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    /** Interface call: set the maximum speed of articulated motion ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] =
        10.0 / 180.0 * M_PI; // The interface requires the unit to be radians
    robotservice.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    /** Robot arm movement to zero posture **/
    double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };

    Util::initJointAngleArray(jointAngle, 0.0, -15 / 180.0 * M_PI,
                              100 / 180.0 * M_PI, 25 / 180.0 * M_PI,
                              90 / 180.0 * M_PI, 0.0);

    std::cout << "Calling the motion function" << std::endl;
    ret = robotservice.robotServiceJointMove(jointAngle, true);
    if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cerr << "Movement to zero posture failure.　ret:" << ret
                  << std::endl;
    }

    /** Interface call: login ***/
    aubo_robot_namespace::MoveRelative relativeMoveOnBase;
    relativeMoveOnBase.ena = true;
    relativeMoveOnBase.relativePosition[0] = 0; // -0.04887;
    relativeMoveOnBase.relativePosition[1] = 0.0215;
    relativeMoveOnBase.relativePosition[2] = 0; // Unit:m

    relativeMoveOnBase.relativeOri.w = 1;
    relativeMoveOnBase.relativeOri.x = 0;
    relativeMoveOnBase.relativeOri.y = 0;
    relativeMoveOnBase.relativeOri.z = 0;
    //    relativeMoveOnBase.relativeOri.w = 1;
    //    relativeMoveOnBase.relativeOri.x = 0;
    //    relativeMoveOnBase.relativeOri.y = 0;
    //    relativeMoveOnBase.relativeOri.z = 0;
    //    ret =
    //    robotservice.robotServiceSetMoveRelativeParam(relativeMoveOnBase);

    aubo_robot_namespace::MoveProfile_t moveProfile;
    moveProfile.relative = relativeMoveOnBase;
    memcpy(moveProfile.jointMaxAcc, jointMaxAcc.jointPara,
           sizeof(jointMaxAcc.jointPara));
    memcpy(moveProfile.jointMaxVelc, jointMaxVelc.jointPara,
           sizeof(jointMaxAcc.jointPara));

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
        Origin_AnyPointOnPositiveXAxis_AnyPointOnFirstQuadrantOfXOYPlane;
    double radian_tem[6] = { 0.0,
                             -15 / 180.0 * M_PI,
                             100 / 180.0 * M_PI,
                             25 / 180.0 * M_PI,
                             90 / 180.0 * M_PI,
                             0.0 };
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
    moveProfile.relativeOnCoord = userCoord;

    std::cout << "RelativeParam" << std::endl;
    ret = robotservice.robotServiceJointMove(moveProfile, jointAngle, true);
    if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cerr << "Movement to zero posture failure.　ret:" << ret
                  << std::endl;
    }
}
void Example_3::Relative_base_demo()
{
    ServiceInterface robotService;
    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** 接口调用: 登录 ***/
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "登录成功" << std::endl;
    } else {
        std::cerr << "登录失败" << std::endl;
    }

    /** 如果是连接真实机械臂，需要对机械臂进行初始化　**/
    aubo_robot_namespace::ROBOT_SERVICE_STATE result;

    //工具动力学参数
    aubo_robot_namespace::ToolDynamicsParam toolDynamicsParam;
    memset(&toolDynamicsParam, 0, sizeof(toolDynamicsParam));
    ret = robotService.rootServiceRobotStartup(
        toolDynamicsParam /**工具动力学参数**/, 6 /*碰撞等级*/,
        true /*是否允许读取位姿　默认为true*/, true, /*保留默认为true */
        1000,                                        /*保留默认为1000 */
        result);                                     /*机械臂初始化*/
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cerr << "机械臂初始化成功." << std::endl;
    } else {
        std::cerr << "机械臂初始化失败." << std::endl;
    }

    //初始化运动属性
    robotService.robotServiceInitGlobalMoveProfile();

    //设置关节运动最大加速度
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 30 * M_PI / 180;
    jointMaxAcc.jointPara[1] = 30 * M_PI / 180;
    jointMaxAcc.jointPara[2] = 30 * M_PI / 180;
    jointMaxAcc.jointPara[3] = 30 * M_PI / 180;
    jointMaxAcc.jointPara[4] = 30 * M_PI / 180;
    jointMaxAcc.jointPara[5] = 30 * M_PI / 180;
    robotService.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    //设置关节运动最大速度
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 30 * M_PI / 180;
    jointMaxVelc.jointPara[1] = 30 * M_PI / 180;
    jointMaxVelc.jointPara[2] = 30 * M_PI / 180;
    jointMaxVelc.jointPara[3] = 30 * M_PI / 180;
    jointMaxVelc.jointPara[4] = 30 * M_PI / 180;
    jointMaxVelc.jointPara[5] = 30 * M_PI / 180;
    robotService.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);

    //设置偏移
    aubo_robot_namespace::MoveRelative relativeOnBase;
    relativeOnBase.ena = true;
    relativeOnBase.relativePosition[0] = 0.03;
    relativeOnBase.relativePosition[1] = 0.0;
    relativeOnBase.relativePosition[2] = 0.0;
    aubo_robot_namespace::Rpy rpyRelative;
    aubo_robot_namespace::Ori oriRelative;
    rpyRelative.rx = 0.0 / 180 * M_PI; //要加一位小数点
    rpyRelative.ry = 0.0 / 180 * M_PI;
    rpyRelative.rz = 0.0 / 180 * M_PI;
    robotService.RPYToQuaternion(rpyRelative, oriRelative);
    relativeOnBase.relativeOri = oriRelative;

    //偏移：法兰盘中心在基坐标系下偏移
    robotService.robotServiceSetMoveRelativeParam(relativeOnBase);
    //偏移：TCP在末端坐标系或工具坐标系下偏移
    //    aubo_robot_namespace::CoordCalibrateByJointAngleAndTool
    //        userCoord;                                     //用户坐标系
    //    aubo_robot_namespace::ToolInEndDesc toolInEndDesc; //工具参数
    //    toolInEndDesc.toolInEndPosition.x = 0;
    //    toolInEndDesc.toolInEndPosition.y = 0;
    //    toolInEndDesc.toolInEndPosition.z = 0;
    //    toolInEndDesc.toolInEndOrientation.w = 1.0;
    //    toolInEndDesc.toolInEndOrientation.x = 0;
    //    toolInEndDesc.toolInEndOrientation.y = 0;
    //    toolInEndDesc.toolInEndOrientation.z = 0;

    //    userCoord.coordType = aubo_robot_namespace::EndCoordinate;
    //    userCoord.toolDesc = toolInEndDesc;
    // robotService.robotServiceSetMoveRelativeParam(relativeOnBase, userCoord);

    //路点
    double jointAngle[6] = {};
    jointAngle[0] = 173.108713 * M_PI / 180;
    jointAngle[1] = -12.075005 * M_PI / 180;
    jointAngle[2] = -83.663342 * M_PI / 180;
    jointAngle[3] = -15.641249 * M_PI / 180;
    jointAngle[4] = -89.140000 * M_PI / 180;
    jointAngle[5] = -28.328713 * M_PI / 180;

    //关节运动
    // robotService.robotServiceJointMove(jointAngle, true);
    robotService.robotServiceLineMove(jointAngle, true);
}
