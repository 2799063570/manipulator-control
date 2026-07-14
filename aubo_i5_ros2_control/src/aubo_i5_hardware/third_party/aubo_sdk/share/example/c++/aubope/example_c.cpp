#include "example_c.h"
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

void Example_c::demo_connectrobot()
{
    if (rs_initialize() != RS_SUCC) {
        std::cout << "初始化失败" << std::endl;
    }
    if (rs_create_context(&rshd_) != RS_SUCC) {
        std::cout << "创建上下文失败" << std::endl;
    }
    if (rs_login(rshd_, SERVER_HOST, 30000) != RS_SUCC) {
        std::cout << "登录失败" << std::endl;
    } else {
        std::cout << "登录成功" << std::endl;
    }

    //工具的动力学参数和运动学参数
    ToolDynamicsParam tool_dynamics = { 0 };
    //机械臂碰撞等级
    uint8 colli_class = 9;
    //机械臂启动是否读取姿态（默认开启）
    bool read_pos = true;
    //机械臂静态碰撞检测（默认开启）
    bool static_colli_detect = true;
    //机械臂最大加速度（系统自动控制，默认为30000)
    int board_maxacc = 30000;
    //机械臂服务启动状态
    ROBOT_SERVICE_STATE m_startState = ROBOT_SERVICE_READY;

    int ret =
        rs_robot_startup(rshd_, &tool_dynamics, colli_class, read_pos,
                         static_colli_detect, board_maxacc, &m_startState);
    if (ret != RS_SUCC) {
        std::cout << "启动机械臂失败" << std::endl;
    } else {
        std::cout << "启动机械臂成功" << std::endl;
    }

    if (m_startState != ROBOT_SERVICE_WORKING)
        std::cout << "机械臂启动状态错误" << std::endl;
}

void Example_c::demo_disconnect()
{
    bool status = false;
    if (rs_get_login_status(rshd_, &status) != RS_SUCC) {
        std::cout << "获取当前的连接状态失败" << std::endl;
    }
    int ret = rs_robot_shutdown(rshd_);
    if (ret == RS_SUCC) {
        std::cout << "关闭机械臂成功" << std::endl;
    }
    if (status) {
        rs_logout(rshd_);
        std::cout << "断开机械臂服务器链接" << std::endl;
    }
    if (rshd_ > 0) {
        rs_destory_context(rshd_);
        std::cout << "注销机械臂控制上下文句柄" << std::endl;
    }
    rs_uninitialize();
}
void Example_c::demo_circle()
{
    /** 接口调用: 初始化运动属性 ***/
    rs_init_global_move_profile(rshd_);

    /** 接口调用: 设置关节型运动的最大加速度 ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] = 30.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxacc(rshd_, &jointMaxAcc);

    /** 接口调用: 设置关节型运动的最大速度 ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] = 30.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxvelc(rshd_, &jointMaxVelc);

    /** 接口调用: 设置末端型运动的最大加速度 　　直线运动属于末端型运动***/
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2; //单位米每秒
    rs_set_global_end_max_line_acc(rshd_, endMoveMaxAcc);
    rs_set_global_end_max_angle_acc(rshd_, endMoveMaxAcc);

    /** 接口调用: 设置末端型运动的最大速度 直线运动属于末端型运动***/
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2; //单位米每秒
    rs_set_global_end_max_line_velc(rshd_, endMoveMaxVelc);
    rs_set_global_end_max_angle_velc(rshd_, endMoveMaxVelc);

    double jointAngle[aubo_robot_namespace::ARM_DOF];

    Util::initJointAngleArray(jointAngle, 89.25 / 180.0 * M_PI,
                              -35.33 / 180.0 * M_PI, 69.75 / 180.0 * M_PI,
                              15.17 / 180.0 * M_PI, 90 / 180.0 * M_PI,
                              -0.65 / 180.0 * M_PI);
    //关节运动至准备点
    int ret = rs_move_joint(rshd_, jointAngle, true);
    if (ret != RS_SUCC) {
        std::cout << "关节运动至准备点失败" << std::endl;
    }

    //圆弧
    for (int i = 0; i < 1; i++) {
        Util::initJointAngleArray(jointAngle, 89.25 / 180.0 * M_PI,
                                  -35.33 / 180.0 * M_PI, 69.75 / 180.0 * M_PI,
                                  15.17 / 180.0 * M_PI, 90 / 180.0 * M_PI,
                                  -0.65 / 180.0 * M_PI);
        rs_add_waypoint(rshd_, jointAngle);
        Util::initJointAngleArray(jointAngle, 82 / 180.0 * M_PI,
                                  -18.04 / 180.0 * M_PI, 95.96 / 180.0 * M_PI,
                                  24.09 / 180.0 * M_PI, 90.01 / 180.0 * M_PI,
                                  -7.91 / 180.0 * M_PI);
        rs_add_waypoint(rshd_, jointAngle);
        Util::initJointAngleArray(jointAngle, 106.34 / 180.0 * M_PI,
                                  16.45 / 180.0 * M_PI, 131.01 / 180.0 * M_PI,
                                  24.64 / 180.0 * M_PI, 89.97 / 180.0 * M_PI,
                                  16.43 / 180.0 * M_PI);
        rs_add_waypoint(rshd_, jointAngle);
        ret = rs_move_track(rshd_, ARC, false);
        if (ret != aubo_robot_namespace::InterfaceCallSuccCode) {
            std::cerr << "圆弧运动失败.　ret:" << ret << std::endl;
        }
    }
}
void Example_c::demo_relative()
{
    aubo_robot_namespace::Rpy rpy;
    aubo_robot_namespace::Ori targetOri;

    rpy.rx = 0.0 / 180.0 * M_PI;
    rpy.ry = 0.0 / 180.0 * M_PI;
    rpy.rz = 0.0 / 180.0 * M_PI;
    rs_rpy_to_quaternion(rshd_, &rpy, &targetOri);

    aubo_robot_namespace::MoveRelative relative;
    relative.ena = true;
    relative.relativePosition[0] = 0.03;
    relative.relativePosition[1] = 0.0;
    relative.relativePosition[2] = 0.0;
    relative.relativeOri = targetOri;

    //基于基坐标系位置偏移

    //    if (RS_SUCC == rs_set_relative_offset_on_base(rshd_, &relative)) {
    //        std::cout << "relative_offset SUCC" << std::endl;
    //    } else {
    //        std::cerr << "relative_offset error" << std::endl;
    //    }

    //偏移：TCP在末端坐标系或工具坐标系下偏移
    ToolInEndDesc m_tool;
    m_tool.toolInEndPosition.x = 0;
    m_tool.toolInEndPosition.y = 0;
    m_tool.toolInEndPosition.z = 0;
    m_tool.toolInEndOrientation.w = 1;
    m_tool.toolInEndOrientation.x = 0;
    m_tool.toolInEndOrientation.y = 0;
    m_tool.toolInEndOrientation.z = 0;
    rs_set_tool_kinematics_param(rshd_, &m_tool);

    aubo_robot_namespace::ToolKinematicsParam tool;
    rs_get_tool_kinematics_param(rshd_, &tool);

    CoordCalibrate userCoord;
    userCoord.coordType = EndCoordinate;
    userCoord.toolDesc = m_tool;
    if (RS_SUCC ==
        rs_set_relative_offset_on_user(rshd_, &relative, &userCoord)) {
        std::cout << "relative_offset SUCC" << std::endl;
    } else {
        std::cerr << "relative_offset error" << std::endl;
    }

    wayPoint_S m_wayPoint;
    int ret = rs_get_current_waypoint(rshd_, &m_wayPoint);

    ret = rs_move_line(rshd_, m_wayPoint.jointpos /*, false*/);
}
void Example_c::demo_computer()
{
    /*    aubo_robot_namespace::wayPoint_S wayPoint;
        aubo_robot_namespace::wayPoint_S targetPoint;
        double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };

        Util::initJointAngleArray(jointAngle, 0.0 / 180.0 * M_PI,
                                  -15.0 / 180.0 * M_PI, 100.0 / 180.0 * M_PI,
                                  25.0 / 180.0 * M_PI, 90.0 / 180.0 * M_PI,
                                  0.0 / 180.0 * M_PI);

        if (RS_SUCC == rs_forward_kin(rshd_, jointAngle, &wayPoint)) {
            Util::printWaypoint(wayPoint);
        } else {
            std::cerr << "fk failed" << std::endl;
        }

        Pos pos = { wayPoint.cartPos.position.x + 0.03,
       wayPoint.cartPos.position.y, wayPoint.cartPos.position.z }; if (RS_SUCC
       == rs_inverse_kin(rshd_, wayPoint.jointpos, &pos, &wayPoint.orientation,
       &targetPoint)) { Util::printWaypoint(targetPoint); } else { std::cerr <<
       "ik failed" << std::endl;
        }*///
    //求解全部逆解
    /*
    ik_solutions m_inversePoint_array;
    rs_inverse_kin_closed_form(rshd_, &pos, &wayPoint.orientation,
                               &m_inversePoint_array);
    for (auto i = 0; i < m_inversePoint_array.solution_count; i++) {
        std::cout << "逆解" << i << ": ";
        for (unsigned int j = 0; j < 6; j++) {
            Util::printWaypoint(m_inversePoint_array.waypoint[i]);
        }
        std::cout << std::endl;
    }
    */
    Pos tempos;
    tempos.x = 0.4785;
    tempos.y = -0.1215;
    tempos.z = 0.4125;
    Rpy rpy;
    rpy.rx = 180 / 180 * M_PI;
    rpy.ry = 0;
    rpy.rz = 0;
    Ori ori;
    int ret = rs_rpy_to_quaternion(rshd_, &rpy, &ori);
    CoordCalibrate m_coord;
    m_coord.coordType = BaseCoordinate;

    ToolInEndDesc toolInEndDesc;
    toolInEndDesc.toolInEndPosition.x = 0.02;
    toolInEndDesc.toolInEndPosition.y = 0;
    toolInEndDesc.toolInEndPosition.z = 0;
    toolInEndDesc.toolInEndOrientation.w = 1.0;
    toolInEndDesc.toolInEndOrientation.x = 0;
    toolInEndDesc.toolInEndOrientation.y = 0;
    toolInEndDesc.toolInEndOrientation.z = 0;
    Pos flange_pos_onbase;
    Ori flange_ori_onbase;
    ret = rs_user_to_base(rshd_, &tempos, &ori, &m_coord, &toolInEndDesc,
                          &flange_pos_onbase, &flange_ori_onbase);
    if (ret == RS_SUCC) {
        std::cerr << flange_pos_onbase.x << "," << flange_pos_onbase.y << ","
                  << flange_pos_onbase.z << std::endl;
        std::cerr << flange_ori_onbase.x << "," << flange_ori_onbase.y << ","
                  << flange_ori_onbase.z << std::endl;
    }
    Pos tool_end_pos_onbase;
    Ori tool_end_ori_onbase;
    ret = rs_base_to_base_additional_tool(
        rshd_, &flange_pos_onbase, &flange_ori_onbase, &toolInEndDesc,
        &tool_end_pos_onbase, &tool_end_ori_onbase);
    if (ret == RS_SUCC) {
        std::cerr << tool_end_pos_onbase.x << "," << tool_end_pos_onbase.y
                  << "," << tool_end_pos_onbase.z << std::endl;
        std::cerr << tool_end_ori_onbase.x << "," << tool_end_ori_onbase.y
                  << "," << tool_end_ori_onbase.z << std::endl;
    }
}
void Example_c::demo_teach()
{
    teach_mode mode = MOV_X;
    bool dir = true;
    /** Robotic 开始示教**/
    std::cout << "TeachStart" << std::endl;
    aubo_robot_namespace::CoordCalibrateByJointAngleAndTool userCoord;
    userCoord.coordType = aubo_robot_namespace::BaseCoordinate;
    rs_set_teach_coord(rshd_, &userCoord);
    rs_teach_move_start(rshd_, mode, dir);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    rs_teach_move_stop(rshd_);
}
void Example_c::demo_rotate()
{
    CoordCalibrate userCoord;
    userCoord.coordType = BaseCoordinate;
    Move_Rotate_Axis rotateAxis_prarm;
    rotateAxis_prarm.rotateAxis[0] = 0;
    rotateAxis_prarm.rotateAxis[1] = 0;
    rotateAxis_prarm.rotateAxis[2] = 1;
    double rotate_angle = 30 / 180.0 * M_PI;
    int ret = rs_move_rotate(rshd_, &userCoord, &rotateAxis_prarm, rotate_angle,
                             true);
    if (ret == RS_SUCC) {
        std::cout << "旋转运动成功" << std::endl;
    }
}
void Example_c::demo_io()
{
    int ret = rs_set_board_io_status_by_name(
        rshd_, RobotIoType::RobotBoardUserDO, "U_DO_03", 0);
    if (ret != RS_SUCC) {
        std::cout << "设置IO失败" << std::endl;
    }
    double val[1];
    rs_get_board_io_status_by_name(rshd_, RobotIoType::RobotBoardUserDI,
                                   "U_DI_00", val);
    if (ret == RS_SUCC) {
        std::cout << val[0] << std::endl;
    }
}
