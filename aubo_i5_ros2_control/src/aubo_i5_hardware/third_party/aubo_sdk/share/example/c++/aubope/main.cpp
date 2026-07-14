#include <iostream>
#include <string>
#include <stdio.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "example_0.h"
#include "example_1.h"
#include "example_3.h"
#include "example_4.h"
#include "example_5.h"
#include "example_6.h"
#include "example_8.h"
#include "example_9.h"

#include "example_c.h"

#include "example_bhwk_c.h"
#define ROBOT_ADDR "127.0.0.1"
#define ROBOT_PORT 30000

RSHD g_rshd = -1;

#define M_PI 3.1415926
using namespace std;

void funcTest()
{
    ServiceInterface robotService;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotService.robotServiceLogin("127.0.0.1", 8899, "aubo", "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }

    //    aubo_robot_namespace::RobotBaseParameters baseParameters;

    //    baseParameters.info.robot_type = 1;
    //    baseParameters.info.auth_type  = 2;
    //    baseParameters.info.robot_expire  = 3;
    //    baseParameters.info.robot_duration  = 4;

    //    for(int i=0;i<12;i++)
    //        baseParameters.info.reserve[i] = i+10;

    //    for(int i=0;i<6;i++)
    //        baseParameters.info.joint_duration[i] = i+30;

    //    ret = robotService.robotServiceSetRobotBaseParameters(baseParameters);

    //    if(ret!=0)
    //    {
    //        std::cout<<"ret:"<<ret<<std::endl;
    //    }

    aubo_robot_namespace::RobotJointsParameter jointsParameter,
        robotJointsParameter;

    //    for(int i=0;i<6;i++)
    //    {
    //        jointsParameter.frictionParam.FL[i]    = i;
    //        jointsParameter.frictionParam.FR[i]    = 10+i;
    //        jointsParameter.frictionParam.tmp_a[i] = 20+i;
    //        jointsParameter.frictionParam.tmp_b[i] = 30+i;
    //        jointsParameter.frictionParam.posvel_a1[i] = 40+i;
    //        jointsParameter.frictionParam.posvel_b1[i] = 50+i;
    //        jointsParameter.frictionParam.posvel_a2[i] = 60+i;
    //        jointsParameter.frictionParam.posvel_b2[i] = 70+i;
    //        jointsParameter.frictionParam.posvel_c2[i] = 80+i;

    //        jointsParameter.frictionParam.negvel_a1[i] = 90  + i;
    //        jointsParameter.frictionParam.negvel_b1[i] = 100 + i;
    //        jointsParameter.frictionParam.negvel_a2[i] = 110 + i;
    //        jointsParameter.frictionParam.negvel_b2[i] = 120 + i;
    //        jointsParameter.frictionParam.negvel_c2[i] = 130 + i;
    //    }

    //    ret =
    //    robotService.robotServiceSetRobotJointsParameter(jointsParameter);
    //    if(ret!=0)
    //    {
    //        std::cout<<"ret:"<<ret<<std::endl;
    //    }

    ret =
        robotService.robotServiceGetRobotJointsParameter(robotJointsParameter);
    if (ret != 0) {
        std::cout << "ret:" << ret << std::endl;
    }
    printf("jointsParameter.frictionParam.FL: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.FL[0],
           robotJointsParameter.frictionParam.FL[1],
           robotJointsParameter.frictionParam.FL[2],
           robotJointsParameter.frictionParam.FL[3],
           robotJointsParameter.frictionParam.FL[4],
           robotJointsParameter.frictionParam.FL[5]);
    printf("jointsParameter.frictionParam.FR: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.FR[0],
           robotJointsParameter.frictionParam.FR[1],
           robotJointsParameter.frictionParam.FR[2],
           robotJointsParameter.frictionParam.FR[3],
           robotJointsParameter.frictionParam.FR[4],
           robotJointsParameter.frictionParam.FR[5]);
    printf("jointsParameter.frictionParam.tmp_a:%d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.tmp_a[0],
           robotJointsParameter.frictionParam.tmp_a[1],
           robotJointsParameter.frictionParam.tmp_a[2],
           robotJointsParameter.frictionParam.tmp_a[3],
           robotJointsParameter.frictionParam.tmp_a[4],
           robotJointsParameter.frictionParam.tmp_a[5]);
    printf("jointsParameter.frictionParam.tmp_b: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.tmp_b[0],
           robotJointsParameter.frictionParam.tmp_b[1],
           robotJointsParameter.frictionParam.tmp_b[2],
           robotJointsParameter.frictionParam.tmp_b[3],
           robotJointsParameter.frictionParam.tmp_b[4],
           robotJointsParameter.frictionParam.tmp_b[5]);
    printf("jointsParameter.frictionParam.posvel_a1:%d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.posvel_a1[0],
           robotJointsParameter.frictionParam.posvel_a1[1],
           robotJointsParameter.frictionParam.posvel_a1[2],
           robotJointsParameter.frictionParam.posvel_a1[3],
           robotJointsParameter.frictionParam.posvel_a1[4],
           robotJointsParameter.frictionParam.posvel_a1[5]);
    printf("jointsParameter.frictionParam.posvel_b1: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.posvel_b1[0],
           robotJointsParameter.frictionParam.posvel_b1[1],
           robotJointsParameter.frictionParam.posvel_b1[2],
           robotJointsParameter.frictionParam.posvel_b1[3],
           robotJointsParameter.frictionParam.posvel_b1[4],
           robotJointsParameter.frictionParam.posvel_b1[5]);
    printf("jointsParameter.frictionParam.posvel_a1: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.posvel_a2[0],
           robotJointsParameter.frictionParam.posvel_a2[1],
           robotJointsParameter.frictionParam.posvel_a2[2],
           robotJointsParameter.frictionParam.posvel_a2[3],
           robotJointsParameter.frictionParam.posvel_a2[4],
           robotJointsParameter.frictionParam.posvel_a2[5]);
    printf("jointsParameter.frictionParam.posvel_b1: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.posvel_b2[0],
           robotJointsParameter.frictionParam.posvel_b2[1],
           robotJointsParameter.frictionParam.posvel_b2[2],
           robotJointsParameter.frictionParam.posvel_b2[3],
           robotJointsParameter.frictionParam.posvel_b2[4],
           robotJointsParameter.frictionParam.posvel_b2[5]);
    printf("jointsParameter.frictionParam.posvel_a1: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.posvel_c2[0],
           robotJointsParameter.frictionParam.posvel_c2[1],
           robotJointsParameter.frictionParam.posvel_c2[2],
           robotJointsParameter.frictionParam.posvel_c2[3],
           robotJointsParameter.frictionParam.posvel_c2[4],
           robotJointsParameter.frictionParam.posvel_c2[5]);

    printf("jointsParameter.frictionParam.negvel_a1:%d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.negvel_a1[0],
           robotJointsParameter.frictionParam.negvel_a1[1],
           robotJointsParameter.frictionParam.negvel_a1[2],
           robotJointsParameter.frictionParam.negvel_a1[3],
           robotJointsParameter.frictionParam.negvel_a1[4],
           robotJointsParameter.frictionParam.negvel_a1[5]);
    printf("jointsParameter.frictionParam.negvel_b1: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.negvel_b1[0],
           robotJointsParameter.frictionParam.negvel_b1[1],
           robotJointsParameter.frictionParam.negvel_b1[2],
           robotJointsParameter.frictionParam.negvel_b1[3],
           robotJointsParameter.frictionParam.negvel_b1[4],
           robotJointsParameter.frictionParam.negvel_b1[5]);
    printf("jointsParameter.frictionParam.negvel_a2: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.negvel_a2[0],
           robotJointsParameter.frictionParam.negvel_a2[1],
           robotJointsParameter.frictionParam.negvel_a2[2],
           robotJointsParameter.frictionParam.negvel_a2[3],
           robotJointsParameter.frictionParam.negvel_a2[4],
           robotJointsParameter.frictionParam.negvel_a2[5]);
    printf("jointsParameter.frictionParam.negvel_b2: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.negvel_b2[0],
           robotJointsParameter.frictionParam.negvel_b2[1],
           robotJointsParameter.frictionParam.negvel_b2[2],
           robotJointsParameter.frictionParam.negvel_b2[3],
           robotJointsParameter.frictionParam.negvel_b2[4],
           robotJointsParameter.frictionParam.negvel_b2[5]);
    printf("jointsParameter.frictionParam.negvel_a1: %d %d %d %d %d %d\n",
           robotJointsParameter.frictionParam.negvel_c2[0],
           robotJointsParameter.frictionParam.negvel_c2[1],
           robotJointsParameter.frictionParam.negvel_c2[2],
           robotJointsParameter.frictionParam.negvel_c2[3],
           robotJointsParameter.frictionParam.negvel_c2[4],
           robotJointsParameter.frictionParam.negvel_c2[5]);
}

int main()
{
    // funcTest();

    // Case 0: Using the SDK to build a control engineering for the simplest
    // robotic arm
    // Example_0::demo();

    // Case 1: The way of the callback function to obtain the real-time
    // waypoint, the end speed, the event of the manipulator, about the
    // acquisition of the relevant state of the manipulator
    // Example_1::demo();
    // Example_1::getJointStatus();
    // Case 3: Joint Movement
    // Example_3::demo();
    // Example_3::Relative_demo();
    // Example_3::Relative_base_demo();
    // Case 4: Line Movement
    // Example_4::demo();

    // Case 5: Trajectory movement
    // Example_5::demo();
    // Example_5::speedl_demo();
    // Example_5::circle_demo();
    // Example_5::spline_demo();
    // Example_5::OfflineTrack_demo();

    // Case 6: Movement to the target position
    // Example_6::demo();
    // Example_6::example_MoveLtoPosition();
    // Example_6::Robot_Move();

    // Case 8: Positive and negative solutions
    // Example_8::baseToUserCoordinate();
    // Example_8::toolPositionOriIk();
    // Example_8::toolToEnd_demo();
    // Example_8::toolPositionOriIk();
    // Example_8::CoordinateCalibration_demo();
    // Example_8::FK_IK();

    // Case 9: About the use case of io
    // Example_9::demo();
    // Example_9::BoardiO_demo();
    // Example_9::TooliO_demo();
    // Example_9::SafetyConfig_demo();

    //    Example_c exam;
    //    exam.demo_connectrobot();
    //    exam.demo_circle();
    // exam.demo_relative();
    // exam.demo_disconnect();
    // exam.demo_computer();
    //    exam.demo_io();
    // exam.demo_teach();
    //    exam.demo_rotate();

    example_login(g_rshd, ROBOT_ADDR, ROBOT_PORT);
    // example_move_stop(g_rshd);
    // 测试问题1：测试DH补偿是否成功
    bhwk_test1(g_rshd);

    // 测试问题2：机械臂轴动
    //    bhwk_test2(g_rshd);

    // 测试问题3：机械臂在工具坐标系下做旋转运动
    //    bhwk_test3(g_rshd);

    // 测试问题4：机械臂在基坐标系下做旋转运动
    //    bhwk_test4(g_rshd);

    // 测试问题5：机械臂在工具坐标系下做位置偏移运动
    //    bhwk_test5(g_rshd);

    // 测试问题6：机械臂在基坐标系下做位置偏移运动
    //    bhwk_test6(g_rshd);

    // 测试问题7：测试机械臂正逆解的功能，八组逆解是否DH补偿
    //    bhwk_test7(g_rshd);

    // 测试问题8：测试move_stop函数
    //    bhwk_test8(g_rshd);

    // 测试问题9：测试实时路点信息回调函数
    //    bhwk_test9(g_rshd);

    // 测试问题10：检测机械臂电源状态
    //    bhwk_test10(g_rshd);

    // 测试问题11：原路径返回退出奇异区？
    bhwk_test11(g_rshd);

    // 测试问题12：机械臂轨迹运动测试——圆弧、圆、MoveP 重点MoveP
    //    bhwk_test12(g_rshd);

    return 0;
}
