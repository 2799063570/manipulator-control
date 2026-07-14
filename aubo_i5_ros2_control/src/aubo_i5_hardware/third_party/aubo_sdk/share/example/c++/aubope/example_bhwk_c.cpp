#include "example_bhwk_c.h"
#include "Eigen/Dense"

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
using namespace Eigen;

void example_move_stop(RSHD rshd)
{
    double destJoint[6]{
        -2.86944, -1.3256, -1.42723, 2.79761, -0.886683, -0.714745,
    };
    std::cout << "move to start pose " << std::endl;
    rs_move_joint(rshd, destJoint, true);

    double zeroJoint[6]{ 0, 0, 0, 0, 0, 0 };
    rs_move_joint(rshd, zeroJoint, false);
    std::cout << "move to zero " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "after sleep " << std::endl;
    int ret = rs_move_stop(rshd);
    std::cout << "after stop " << std::endl;

    if (RS_SUCC == ret) {
        std::cout << "move_stop succ" << std::endl;
    } else {
        std::cerr << "move_stop failed" << std::endl;
    }
}
// 机械臂登录
bool example_login(RSHD &rshd, const char *addr, int port)
{
    bool result = false;

    rshd = RS_FAILED;

    // 初始化接口库
    if (rs_initialize() == RS_SUCC) {
        // 创建上下文
        if (rs_create_context(&rshd) == RS_SUCC) {
            // 机械臂登录
            if (rs_login(rshd, addr, port) == RS_SUCC) {
                result = true;
                std::cout << "机械臂登录成功" << std::endl;
            } else {
                std::cerr << "机械臂登录失败" << std::endl;
            }
        } else {
            std::cerr << "创建上下文失败" << std::endl;
        }
    } else {
        // 初始化接口库失败
        std::cerr << "初始化接口库失败" << std::endl;
    }

    return result;
}

// 关闭机械臂（必须连接真实机械臂）
bool example_robotShutdown(RSHD rshd)
{
    bool result = false;
    if (rs_robot_shutdown(rshd) == RS_SUCC) {
        result = true;
        std::cout << "机械臂断电成功" << std::endl;
    } else {
        std::cerr << "机械臂断电失败" << std::endl;
    }
    return result;
}

// 设置机械臂实际速度
void SetRobotActualSpeed(RSHD rshd, double speed)
{
    // speed代表速度比例
    if (speed < 0)
        speed = 0.01;
    if (speed > 1)
        speed = 1;

    /** 初始化运动属性 ***/
    int ret = rs_init_global_move_profile(rshd);

    //设置关节转角速度
    JointVelcAccParam max_velc;
    for (int i = 0; i < ARM_DOF; i++)
        max_velc.jointPara[i] = 10.0 / 180.0 * M_PI * speed;

    ret = rs_set_global_joint_maxvelc(rshd, &max_velc);

    //设置TCP线性速度
    rs_set_global_end_max_line_acc(rshd, 10.0 / 180.0 * M_PI * speed);
    rs_set_global_end_max_angle_acc(rshd, 10.0 / 180.0 * M_PI * speed);
    rs_set_global_end_max_line_velc(rshd, 10.0 / 180.0 * M_PI * speed);
    rs_set_global_end_max_angle_velc(rshd, 10.0 / 180.0 * M_PI * speed);
}

// 绕基坐标轴旋转角度
void RotateAroundBaseFrameByAngle(RSHD rshd, double angle, int Axial,
                                  double speed, double acc)
{
    SetRobotActualSpeed(rshd, speed);

    CoordCalibrate UserCoord;
    UserCoord.coordType = BaseCoordinate;

    Move_Rotate_Axis rotateAxis_prarm;
    rotateAxis_prarm.rotateAxis[0] = 0;
    rotateAxis_prarm.rotateAxis[1] = 0;
    rotateAxis_prarm.rotateAxis[2] = 0;

    switch (Axial) {
    case 1:
        rotateAxis_prarm.rotateAxis[0] = 1; // X
        break;
    case 2:
        rotateAxis_prarm.rotateAxis[1] = 1; // Y
        break;
    case 3:
        rotateAxis_prarm.rotateAxis[2] = 1; // Z
        break;
    default:
        break;
    }

    double rotate_angle = angle / 180.0 * M_PI;

    int ret =
        rs_move_rotate(rshd, &UserCoord, &rotateAxis_prarm, rotate_angle, true);

    if (ret == RS_SUCC) {
        std::cout << "机械臂旋转成功！" << std::endl;

    } else {
        std::cerr << "机械臂旋转失败！" << std::endl;
    }
}

// 绕TCP坐标系旋转角度
void RotateAroundTCPFrameByAngle(RSHD rshd, double angle, int Axial,
                                 double speed, double acc)
{
    SetRobotActualSpeed(rshd, speed);

    ToolInEndDesc m_tool;
    double tcp[3];

    m_tool.toolInEndPosition.x = 0.177341;
    m_tool.toolInEndPosition.y = 0.002327;
    m_tool.toolInEndPosition.z = 0.146822;

    m_tool.toolInEndOrientation.w = 1;
    m_tool.toolInEndOrientation.x = 0;
    m_tool.toolInEndOrientation.y = 0;
    m_tool.toolInEndOrientation.z = 0;

    rs_set_tool_kinematics_param(rshd, &m_tool);

    CoordCalibrate UserCoord;
    UserCoord.coordType = EndCoordinate;
    UserCoord.toolDesc = m_tool;

    Move_Rotate_Axis rotateAxis_prarm;
    rotateAxis_prarm.rotateAxis[0] = 0;
    rotateAxis_prarm.rotateAxis[1] = 0;
    rotateAxis_prarm.rotateAxis[2] = 0;

    switch (Axial) {
    case 1:
        rotateAxis_prarm.rotateAxis[0] = 1; // X
        break;
    case 2:
        rotateAxis_prarm.rotateAxis[1] = 1; // Y
        break;
    case 3:
        rotateAxis_prarm.rotateAxis[2] = 1; // Z
        break;
    default:
        break;
    }

    double rotate_angle = angle / 180.0 * M_PI;

    int ret =
        rs_move_rotate(rshd, &UserCoord, &rotateAxis_prarm, rotate_angle, true);

    if (ret == RS_SUCC) {
        std::cout << "机械臂旋转成功！" << std::endl;

    } else {
        std::cerr << "机械臂旋转失败!" << std::endl;
    }
}

// 沿基坐标系位置偏移量
int TransAlongBaseFrameByDistance(RSHD rshd, double distance, int Axial,
                                  double speed, double acc)
{
    SetRobotActualSpeed(rshd, speed);

    ToolInEndDesc tool;
    tool.toolInEndPosition.x = 0.177341;
    tool.toolInEndPosition.y = 0.002327;
    tool.toolInEndPosition.z = 0.146822;
    tool.toolInEndOrientation.w = 1;
    tool.toolInEndOrientation.x = 0;
    tool.toolInEndOrientation.y = 0;
    tool.toolInEndOrientation.z = 0;
    //修改
    rs_set_tool_kinematics_param(rshd, &tool);

    //设置偏移量

    aubo_robot_namespace::Rpy rpy;
    aubo_robot_namespace::Ori targetOri;

    rpy.rx = 0.0 / 180.0 * M_PI;
    rpy.ry = 0.0 / 180.0 * M_PI;
    rpy.rz = 0.0 / 180.0 * M_PI;
    rs_rpy_to_quaternion(rshd, &rpy, &targetOri);

    aubo_robot_namespace::MoveRelative relative;
    relative.ena = TRUE;
    relative.relativePosition[0] = 0.0;
    relative.relativePosition[1] = 0.0;
    relative.relativePosition[2] = 0.0;

    MatrixXd TransMatrix(4, 4);
    TransMatrix(0, 0) = 1;
    TransMatrix(0, 1) = 0;
    TransMatrix(0, 2) = 0;
    TransMatrix(0, 3) = 0;
    TransMatrix(1, 0) = 0;
    TransMatrix(1, 1) = 1;
    TransMatrix(1, 2) = 0;
    TransMatrix(1, 3) = 0;
    TransMatrix(2, 0) = 0;
    TransMatrix(2, 1) = 0;
    TransMatrix(2, 2) = 1;
    TransMatrix(2, 3) = 0;
    TransMatrix(3, 0) = 0;
    TransMatrix(3, 1) = 0;
    TransMatrix(3, 2) = 0;
    TransMatrix(3, 3) = 1;

    switch (Axial) {
    case 1:
        relative.relativePosition[0] = distance / 1000.0; // X轴偏移
        TransMatrix(0, 3) = distance / 1000.0;
        break;
    case 2:
        relative.relativePosition[1] = distance / 1000.0; // Y轴偏移
        TransMatrix(1, 3) = distance / 1000.0;
        break;
    case 3:
        relative.relativePosition[2] = distance / 1000.0; // X轴偏移
        TransMatrix(2, 3) = distance / 1000.0;
        break;
    default:
        break;
    }

    relative.relativeOri = targetOri;

    CoordCalibrate userCoord;
    userCoord.coordType = BaseCoordinate;

    //基于用户坐标系位置偏移

    if (RS_SUCC == rs_set_relative_offset_on_base(rshd, &relative)) {
        std::cout << "偏移运动成功！" << std::endl;
    } else {
        std::cerr << "偏移运动失败！" << std::endl;
    }

    wayPoint_S m_wayPoint;
    int ret = rs_get_current_waypoint(rshd, &m_wayPoint);

    Quaterniond ori(m_wayPoint.orientation.w, m_wayPoint.orientation.x,
                    m_wayPoint.orientation.y, m_wayPoint.orientation.z);
    MatrixXd center_matrix = ori.toRotationMatrix();
    MatrixXd centerToBase(4, 4), TcpToBase(4, 4);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            centerToBase(i, j) = center_matrix(i, j);
        }
    centerToBase(0, 3) = m_wayPoint.cartPos.position.x;
    centerToBase(1, 3) = m_wayPoint.cartPos.position.y;
    centerToBase(2, 3) = m_wayPoint.cartPos.position.z;

    centerToBase(3, 0) = 0;
    centerToBase(3, 1) = 0;
    centerToBase(3, 2) = 0;
    centerToBase(3, 3) = 1;

    MatrixXd TCP(4, 4);
    TCP(0, 0) = 1;
    TCP(0, 1) = 0;
    TCP(0, 2) = 0;
    TCP(0, 3) = 0;
    TCP(1, 0) = 0;
    TCP(1, 1) = 1;
    TCP(1, 2) = 0;
    TCP(1, 3) = 0;
    TCP(2, 0) = 0;
    TCP(2, 1) = 0;
    TCP(2, 2) = 1;
    TCP(2, 3) = 0;
    TCP(3, 0) = 0;
    TCP(3, 1) = 0;
    TCP(3, 2) = 0;
    TCP(3, 3) = 1;

    TcpToBase = TransMatrix * centerToBase * TCP;

    std::cout << "目标位置笛卡尔坐标值：" << TcpToBase(0, 3) << "  "
              << TcpToBase(1, 3) << "  " << TcpToBase(2, 3) << "\n";

    ret = rs_move_line(rshd, m_wayPoint.jointpos /*, false*/);

    wayPoint_S m_wayPoint1;
    int ret1 = rs_get_current_waypoint(rshd, &m_wayPoint1);

    // 由于现在 rs_get_current_waypoint 获取的是TCP在基坐标系下的路点
    // 所以不需要做转换了，便将以下代码注释掉
    //    Pos tool_end_pos_onbase;
    //    Ori tool_end_ori_onbase;
    //    ret = rs_base_to_base_additional_tool(
    //        rshd, &(m_wayPoint1.cartPos.position), &(m_wayPoint1.orientation),
    //        &tool, &tool_end_pos_onbase, &tool_end_ori_onbase);

    //    std::cout << "偏移后法兰笛卡尔坐标值：" << tool_end_pos_onbase.x << "
    //    "
    //              << tool_end_pos_onbase.y << "  " << tool_end_pos_onbase.z <<
    //              "\n";

    std::cout << "偏移后法兰笛卡尔坐标值：" << m_wayPoint1.cartPos.position.x
              << "  " << m_wayPoint1.cartPos.position.y << "  "
              << m_wayPoint1.cartPos.position.z << "\n";

    if (ret == RS_SUCC)
        return RS_SUCC;
}

// 沿TCP坐标系位置偏移量
int TransAlongTCPFrameByDistance(RSHD rshd, double distance, int Axial,
                                 double speed, double acc)
{
    SetRobotActualSpeed(rshd, speed);

    ToolInEndDesc tool;
    tool.toolInEndPosition.x = 0.17734;
    tool.toolInEndPosition.y = 0.00233;
    tool.toolInEndPosition.z = 0.14682;
    tool.toolInEndOrientation.w = 1;
    tool.toolInEndOrientation.x = 0;
    tool.toolInEndOrientation.y = 0;
    tool.toolInEndOrientation.z = 0;

    rs_set_tool_kinematics_param(rshd, &tool);

    aubo_robot_namespace::Rpy rpy;
    aubo_robot_namespace::Ori targetOri;

    rpy.rx = 0.0 / 180.0 * M_PI;
    rpy.ry = 0.0 / 180.0 * M_PI;
    rpy.rz = 0.0 / 180.0 * M_PI;
    rs_rpy_to_quaternion(rshd, &rpy, &targetOri);

    aubo_robot_namespace::MoveRelative relative;
    relative.ena = TRUE;
    relative.relativePosition[0] = 0.0;
    relative.relativePosition[1] = 0.0;
    relative.relativePosition[2] = 0.0;

    MatrixXd TransMatrix(4, 4);
    TransMatrix(0, 0) = 1;
    TransMatrix(0, 1) = 0;
    TransMatrix(0, 2) = 0;
    TransMatrix(0, 3) = 0;
    TransMatrix(1, 0) = 0;
    TransMatrix(1, 1) = 1;
    TransMatrix(1, 2) = 0;
    TransMatrix(1, 3) = 0;
    TransMatrix(2, 0) = 0;
    TransMatrix(2, 1) = 0;
    TransMatrix(2, 2) = 1;
    TransMatrix(2, 3) = 0;
    TransMatrix(3, 0) = 0;
    TransMatrix(3, 1) = 0;
    TransMatrix(3, 2) = 0;
    TransMatrix(3, 3) = 1;

    MatrixXd TCP(4, 4);
    TCP(0, 0) = 1;
    TCP(0, 1) = 0;
    TCP(0, 2) = 0;
    TCP(0, 3) = 0;
    TCP(1, 0) = 0;
    TCP(1, 1) = 1;
    TCP(1, 2) = 0;
    TCP(1, 3) = 0;
    TCP(2, 0) = 0;
    TCP(2, 1) = 0;
    TCP(2, 2) = 1;
    TCP(2, 3) = 0;
    TCP(3, 0) = 0;
    TCP(3, 1) = 0;
    TCP(3, 2) = 0;
    TCP(3, 3) = 1;

    switch (Axial) {
    case 1:
        relative.relativePosition[0] = distance / 1000.0; // X轴偏移
        TransMatrix(0, 3) = distance / 1000.0;
        break;
    case 2:
        relative.relativePosition[1] = distance / 1000.0; // Y轴偏移
        TransMatrix(1, 3) = distance / 1000.0;
        break;
    case 3:
        relative.relativePosition[2] = distance / 1000.0; // X轴偏移
        TransMatrix(2, 3) = distance / 1000.0;
        break;
    default:
        break;
    }

    relative.relativeOri = targetOri;

    CoordCalibrate userCoord;
    userCoord.coordType = EndCoordinate;
    userCoord.toolDesc = tool;

    //基于用户坐标系位置偏移
    if (RS_SUCC ==
        rs_set_relative_offset_on_user(rshd, &relative, &userCoord)) {
        std::cout << "偏移运动成功！" << std::endl;
    } else {
        std::cerr << "偏移运动失败！" << std::endl;
    }

    //获取当前路点
    wayPoint_S m_wayPoint;
    int ret = rs_get_current_waypoint(rshd, &m_wayPoint);

    Quaterniond ori(m_wayPoint.orientation.w, m_wayPoint.orientation.x,
                    m_wayPoint.orientation.y, m_wayPoint.orientation.z);
    MatrixXd center_matrix = ori.toRotationMatrix();
    MatrixXd centerToBase(4, 4), TcpToBase(4, 4);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            centerToBase(i, j) = center_matrix(i, j);
        }
    centerToBase(0, 3) = m_wayPoint.cartPos.position.x;
    centerToBase(1, 3) = m_wayPoint.cartPos.position.y;
    centerToBase(2, 3) = m_wayPoint.cartPos.position.z;

    centerToBase(3, 0) = 0;
    centerToBase(3, 1) = 0;
    centerToBase(3, 2) = 0;
    centerToBase(3, 3) = 1;

    TcpToBase = centerToBase * TCP * TransMatrix;

    std::cout << "目标位置笛卡尔坐标值：" << TcpToBase(0, 3) << "  "
              << TcpToBase(1, 3) << "  " << TcpToBase(2, 3) << "\n";

    ret = rs_move_line(rshd, m_wayPoint.jointpos /*, false*/);

    wayPoint_S m_wayPoint1;
    int ret1 = rs_get_current_waypoint(rshd, &m_wayPoint1);

    // 由于现在 rs_get_current_waypoint 获取的是TCP在基坐标系下的路点
    // 所以不需要做转换了，便将以下代码注释掉
    //    Pos tool_end_pos_onbase;
    //    Ori tool_end_ori_onbase;
    //    ret = rs_base_to_base_additional_tool(
    //        rshd, &(m_wayPoint1.cartPos.position), &(m_wayPoint1.orientation),
    //        &tool, &tool_end_pos_onbase, &tool_end_ori_onbase);

    //    std::cout << "偏移后的笛卡尔坐标值：" << tool_end_pos_onbase.x << "  "
    //              << tool_end_pos_onbase.y << "  " << tool_end_pos_onbase.z <<
    //              "\n";

    std::cout << "偏移后法兰笛卡尔坐标值：" << m_wayPoint1.cartPos.position.x
              << "  " << m_wayPoint1.cartPos.position.y << "  "
              << m_wayPoint1.cartPos.position.z << "\n";

    if (ret == RS_SUCC)
        return RS_SUCC;
}

// 打印路点
void printRoadPoint(const aubo_robot_namespace::wayPoint_S *wayPoint)
{
    std::cout << "pos.x=" << wayPoint->cartPos.position.x << "   ";
    std::cout << "pos.y=" << wayPoint->cartPos.position.y << "   ";
    std::cout << "pos.z=" << wayPoint->cartPos.position.z << std::endl;

    std::cout << "ori.w=" << wayPoint->orientation.w << "   ";
    std::cout << "ori.x=" << wayPoint->orientation.x << "   ";
    std::cout << "ori.y=" << wayPoint->orientation.y << "   ";
    std::cout << "ori.z=" << wayPoint->orientation.z << std::endl;

    std::cout << "joint_1=" << wayPoint->jointpos[0] * 180.0 / M_PI << "   ";
    std::cout << "joint_2=" << wayPoint->jointpos[1] * 180.0 / M_PI << "   ";
    std::cout << "joint_3=" << wayPoint->jointpos[2] * 180.0 / M_PI << "   ";
    std::cout << "joint_4=" << wayPoint->jointpos[3] * 180.0 / M_PI << "   ";
    std::cout << "joint_5=" << wayPoint->jointpos[4] * 180.0 / M_PI << "   ";
    std::cout << "joint_6=" << wayPoint->jointpos[5] * 180.0 / M_PI
              << std::endl;
    std::cout << std::endl;
}

// 实时路点信息回调
void callback_RealTimeRoadPoint(
    const aubo_robot_namespace::wayPoint_S *wayPoint, void *arg)
{
    printRoadPoint(wayPoint);
}

//轨迹运动 rs_move_track
//示例1：圆运动
void moveTrack1(RSHD rshd)
{
    //初始化运动属性
    rs_init_global_move_profile(rshd);

    //设置关节型运动的最大加速度
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] = 30.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxacc(rshd, &jointMaxAcc);

    //设置关节型运动的最大速度
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] = 30.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxvelc(rshd, &jointMaxVelc);

    //设置末端型运动的最大加速度
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2; //单位米每秒
    rs_set_global_end_max_line_acc(rshd, endMoveMaxAcc);
    rs_set_global_end_max_angle_acc(rshd, endMoveMaxAcc);

    //设置末端型运动的最大速度
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2; //单位米每秒
    rs_set_global_end_max_line_velc(rshd, endMoveMaxVelc);
    rs_set_global_end_max_angle_velc(rshd, endMoveMaxVelc);

    //准备点
    double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };
    jointAngle[0] = -0.000003;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570796;
    jointAngle[5] = -0.000008;

    //关节运动到准备点
    int ret = rs_move_joint(rshd, jointAngle);
    if (ret != RS_SUCC) {
        std::cerr << "关节运动到准备点失败。	错误号: " << ret << std::endl;
    }

    //添加圆轨迹路点
    jointAngle[0] = -0.000003;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570796;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    jointAngle[0] = 0.200000;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570794;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    jointAngle[0] = 0.600000;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570796;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    //开始轨迹运动
    ret = rs_move_track(rshd, CIRCLE);
    if (RS_SUCC != ret) {
        std::cerr << "圆轨迹运动失败。　错误号:" << ret << std::endl;
    } else {
        std::cout << "圆轨迹运动成功。" << std::endl;
    }
}

//示例2：圆弧运动
void moveTrack2(RSHD rshd)
{
    //初始化运动属性
    rs_init_global_move_profile(rshd);

    //设置关节型运动的最大加速度
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] = 30.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxacc(rshd, &jointMaxAcc);

    //设置关节型运动的最大速度
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] = 30.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxvelc(rshd, &jointMaxVelc);

    //设置末端型运动的最大加速度
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2; //单位米每秒
    rs_set_global_end_max_line_acc(rshd, endMoveMaxAcc);
    rs_set_global_end_max_angle_acc(rshd, endMoveMaxAcc);

    //设置末端型运动的最大速度
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2; //单位米每秒
    rs_set_global_end_max_line_velc(rshd, endMoveMaxVelc);
    rs_set_global_end_max_angle_velc(rshd, endMoveMaxVelc);

    //准备点
    double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };
    jointAngle[0] = -0.000003;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570796;
    jointAngle[5] = -0.000008;

    //关节运动到准备点
    int ret = rs_move_joint(rshd, jointAngle);
    if (ret != RS_SUCC) {
        std::cerr << "关节运动到准备点失败。	错误号: " << ret << std::endl;
    }

    //添加圆弧轨迹路点
    jointAngle[0] = -0.000003;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570796;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    jointAngle[0] = 0.200000;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570794;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    jointAngle[0] = 0.600000;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570796;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    //开始轨迹运动
    ret = rs_move_track(rshd, ARC);
    if (RS_SUCC != ret) {
        std::cerr << "圆弧轨迹运动失败。　错误号:" << ret << std::endl;
    } else {
        std::cout << "圆弧轨迹运动成功。" << std::endl;
    }
}

//示例3：MOVEP运动
void moveTrack3(RSHD rshd)
{
    //初始化运动属性
    rs_init_global_move_profile(rshd);

    //设置关节型运动的最大加速度
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 30.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] = 30.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxacc(rshd, &jointMaxAcc);

    //设置关节型运动的最大速度
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 30.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] = 30.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxvelc(rshd, &jointMaxVelc);

    //设置末端型运动的最大加速度
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2; //单位米每秒
    rs_set_global_end_max_line_acc(rshd, endMoveMaxAcc);
    rs_set_global_end_max_angle_acc(rshd, endMoveMaxAcc);

    //设置末端型运动的最大速度
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2; //单位米每秒
    rs_set_global_end_max_line_velc(rshd, endMoveMaxVelc);
    rs_set_global_end_max_angle_velc(rshd, endMoveMaxVelc);

    //准备点
    double jointAngle[aubo_robot_namespace::ARM_DOF] = { 0 };
    jointAngle[0] = -0.000003;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570796;
    jointAngle[5] = -0.000008;

    //    //关节运动到准备点
    int ret = rs_move_joint(rshd, jointAngle);
    if (ret != RS_SUCC) {
        std::cerr << "关节运动到准备点失败。	错误号: " << ret << std::endl;
    }

    //添加MOVEP轨迹路点
    jointAngle[0] = -0.000003;
    jointAngle[1] = -0.127267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570796;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    jointAngle[0] = 0.100000;
    jointAngle[1] = -0.147267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570794;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    jointAngle[0] = 0.200000;
    jointAngle[1] = -0.167267;
    jointAngle[2] = -1.321122;
    jointAngle[3] = 0.376934;
    jointAngle[4] = -1.570794;
    jointAngle[5] = -0.000008;
    rs_add_waypoint(rshd, jointAngle);

    //设置交融半径
    //    rs_set_blend_radius(rshd, 0.03);

    //开始轨迹运动
    ret = rs_move_track(rshd, CARTESIAN_MOVEP);
    if (RS_SUCC != ret) {
        std::cerr << "MOVEP轨迹运动失败。　错误号:" << ret << std::endl;
    } else {
        std::cout << "MOVEP轨迹运动成功。" << std::endl;
    }
}

// 测试问题1：测试DH补偿是否成功
bool bhwk_test1(RSHD rshd)
{
    bool result = false;

    //工具的动力学参数和运动学参数
    ToolDynamicsParam tool_dynamics = { 0 };
    //机械臂碰撞等级
    uint8 colli_class = 6;
    //机械臂启动是否读取姿态（默认开启）
    bool read_pos = true;
    //机械臂静态碰撞检测（默认开启）
    bool static_colli_detect = true;
    //机械臂最大加速度（系统自动控制，默认为30000)
    int board_maxacc = 30000;
    //机械臂服务启动状态
    ROBOT_SERVICE_STATE state = ROBOT_SERVICE_READY;

    if (rs_robot_startup(rshd, &tool_dynamics, colli_class, read_pos,
                         static_colli_detect, board_maxacc,
                         &state) == RS_SUCC) {
        result = true;
        std::cout << "机械臂上电成功！ 机械臂状态:" << state << std::endl;
    } else {
        std::cerr << "机械臂上电失败！" << std::endl;
    }

    wayPoint_S m_waypoint;
    Rpy rpy;
    int ret = rs_get_current_waypoint(rshd, &m_waypoint);
    ret = rs_quaternion_to_rpy(rshd, &m_waypoint.orientation, &rpy);

    std::cout << "当前笛卡尔坐标值：" << m_waypoint.cartPos.position.x << "  "
              << m_waypoint.cartPos.position.y << "  "
              << m_waypoint.cartPos.position.z << "\n";
    std::cout << "当前位置姿态：" << rpy.rx / M_PI * 180 << "  "
              << rpy.ry / M_PI * 180 << "  " << rpy.rz / M_PI * 180 << "\n";

    return result;
}

// 测试问题2：机械臂轴动
bool bhwk_test2(RSHD rshd)
{
    SetRobotActualSpeed(rshd, 0.2);
    bool result = false;

    //该位置为机械臂的初始位置
    //    double initPos[6] = { 49.27 / 180 * M_PI,  55.14 / 180 * M_PI,
    //                          -87.87 / 180 * M_PI, 30.39 / 180 * M_PI,
    //                          44.63 / 180 * M_PI,  -36.66 / 180 * M_PI };

    double initPos[6] = { -0.211675, -0.325189, -1.466753,
                          0.429232,  -1.570794, -0.211680 };

    /*double initPos[6] = {
        0.100000,
        -0.147267,
        -1.321122,
        0.376934,
        -1.570794,
        -0.000008 };*/

    //首先运动到初始位置
    if (rs_move_joint(rshd, initPos) == RS_SUCC) {
        result = true;
        std::cout << "机械臂轴动成功！" << std::endl;
    } else {
        std::cerr << "机械臂轴动失败！" << std::endl;
    }

    return result;
}

// 测试问题3：机械臂在工具坐标系下做旋转运动
void bhwk_test3(RSHD rshd)
{
    SetRobotActualSpeed(rshd, 0.2);
    bool result = false;
    //该位置为机械臂的初始位置
    double initPos[6] = { 49.354340 / 180 * M_PI,  55.404920 / 180 * M_PI,
                          -87.756768 / 180 * M_PI, 30.533112 / 180 * M_PI,
                          45.547263 / 180 * M_PI,  -36.195731 / 180 * M_PI };
    //首先运动到初始位置
    if (rs_move_joint(rshd, initPos) == RS_SUCC) {
        result = true;
        std::cout << "轴动到初始位置成功！" << std::endl;
    } else {
        std::cerr << "轴动到初始位置失败！" << std::endl;
    }

    char ch = 'l';
    double angle = 0;
    int Axial = 0;
    while (1) {
        ch = getchar();

        printf("请输入一个字符 ：\n");
        printf(" 'X' 'Y' 'Z' 分别代表 x+  y+ z+ 。 \n");
        printf(" 'U' 'V' 'W' 分别代表 x-  y- z- 。\n");
        printf(" 'Q' 代表退出。\n");

        if (ch == 'Q')
            break;

        switch (ch) {
        case 'X':
            angle = 30.0;
            Axial = 1;
            break;
        case 'Y':
            angle = 30.0;
            Axial = 2;
            break;
        case 'Z':
            angle = 30.0;
            Axial = 3;
            break;
        case 'U':
            angle = -30.0;
            Axial = 1;
            break;
        case 'V':
            angle = -30.0;
            Axial = 2;
            break;
        case 'W':
            angle = -30.0;
            Axial = 3;
            break;
        default:
            break;
        }
        if (ch == 'X' || ch == 'Y' || ch == 'Z' || ch == 'U' || ch == 'V' ||
            ch == 'W') {
            RotateAroundTCPFrameByAngle(rshd, angle, Axial, 0.2, 0.2);
        }
    }
}

// 测试问题4：机械臂在基坐标系下做旋转运动
void bhwk_test4(RSHD rshd)
{
    SetRobotActualSpeed(rshd, 0.2);
    bool result = false;
    //该位置为机械臂的初始位置
    double initPos[6] = { 49.354340 / 180 * M_PI,  55.404920 / 180 * M_PI,
                          -87.756768 / 180 * M_PI, 30.533112 / 180 * M_PI,
                          45.547263 / 180 * M_PI,  -36.195731 / 180 * M_PI };
    //首先运动到初始位置
    if (rs_move_joint(rshd, initPos) == RS_SUCC) {
        result = true;
        std::cout << "轴动到初始位置成功！" << std::endl;
    } else {
        std::cerr << "轴动到初始位置失败！" << std::endl;
    }

    char ch = 'l';
    double angle = 0;
    int Axial = 0;
    while (1) {
        ch = getchar();

        printf("请输入一个字符： \n");
        printf(" 'X' 'Y' 'Z' 代表 x+  y+ z+  \n");
        printf(" 'U' 'V' 'W' 代表 x-  y- z-  \n");
        printf(" 'Q' 代表退出 \n");

        if (ch == 'Q')
            break;

        switch (ch) {
        case 'X':
            angle = 30.0;
            Axial = 1;
            break;
        case 'Y':
            angle = 30.0;
            Axial = 2;
            break;
        case 'Z':
            angle = 30.0;
            Axial = 3;
            break;
        case 'U':
            angle = -30.0;
            Axial = 1;
            break;
        case 'V':
            angle = -30.0;
            Axial = 2;
            break;
        case 'W':
            angle = -30.0;
            Axial = 3;
            break;
        default:
            break;
        }
        if (ch == 'X' || ch == 'Y' || ch == 'Z' || ch == 'U' || ch == 'V' ||
            ch == 'W') {
            RotateAroundBaseFrameByAngle(rshd, angle, Axial, 0.2, 0.2);
        }
    }
}

// 测试问题5：机械臂在工具坐标系下做位置偏移运动
void bhwk_test5(RSHD rshd)
{
    SetRobotActualSpeed(rshd, 0.2);
    bool result = false;
    //该位置为机械臂的初始位置
    double initPos[6] = { 49.354340 / 180 * M_PI,  55.404920 / 180 * M_PI,
                          -87.756768 / 180 * M_PI, 30.533112 / 180 * M_PI,
                          45.547263 / 180 * M_PI,  -36.195731 / 180 * M_PI };
    //首先运动到初始位置
    if (rs_move_joint(rshd, initPos) == RS_SUCC) {
        result = true;
        std::cout << "轴动到初始位置成功！" << std::endl;
    } else {
        std::cerr << "轴动到初始位置失败！" << std::endl;
    }

    char ch = 'l';
    double distance = 0;
    int Axial = 0;
    while (1) {
        ch = getchar();

        printf("请输入一个字符： \n");
        printf(" 'X' 'Y' 'Z' 代表 x+  y+ z+。  \n");
        printf(" 'U' 'V' 'W' 代表 for x-  y- z-。  \n");
        printf(" 'Q' 代表退出。 \n");

        if (ch == 'Q')
            break;

        switch (ch) {
        case 'X':
            distance = 30.0;
            Axial = 1;
            break;
        case 'Y':
            distance = 30.0;
            Axial = 2;
            break;
        case 'Z':
            distance = 30.0;
            Axial = 3;
            break;
        case 'U':
            distance = -30.0;
            Axial = 1;
            break;
        case 'V':
            distance = -30.0;
            Axial = 2;
            break;
        case 'W':
            distance = -30.0;
            Axial = 3;
            break;
        default:
            break;
        }
        if (ch == 'X' || ch == 'Y' || ch == 'Z' || ch == 'U' || ch == 'V' ||
            ch == 'W') {
            TransAlongTCPFrameByDistance(rshd, distance, Axial, 0.2, 0.2);
        }
    }
}

// 测试问题6：机械臂在基坐标系下做位置偏移运动
void bhwk_test6(RSHD rshd)
{
    SetRobotActualSpeed(rshd, 0.2);
    bool result = false;
    //该位置为机械臂的初始位置（提供6个关节角的关节信息（单位：弧度））
    double initPos[6] = { 49.354340 / 180 * M_PI,  55.404920 / 180 * M_PI,
                          -87.756768 / 180 * M_PI, 30.533112 / 180 * M_PI,
                          45.547263 / 180 * M_PI,  -36.195731 / 180 * M_PI };
    //首先运动到初始位置
    if (rs_move_joint(rshd, initPos) == RS_SUCC) {
        result = true;
        std::cout << "轴动到初始位置成功！" << std::endl;
    } else {
        std::cerr << "轴动到初始位置失败！" << std::endl;
    }

    char ch = 'l';
    double distance = 0;
    int Axial = 0;
    while (1) {
        ch = getchar();

        printf("请输入一个字符：\n");
        printf(" 'X' 'Y' 'Z' 代表 x+  y+ z+。  \n");
        printf(" 'U' 'V' 'W' 代表 for x-  y- z-。  \n");
        printf(" 'Q' 代表退出。 \n");

        if (ch == 'Q')
            break;

        switch (ch) {
        case 'X':
            distance = 30.0;
            Axial = 1;
            break;
        case 'Y':
            distance = 30.0;
            Axial = 2;
            break;
        case 'Z':
            distance = 30.0;
            Axial = 3;
            break;
        case 'U':
            distance = -30.0;
            Axial = 1;
            break;
        case 'V':
            distance = -30.0;
            Axial = 2;
            break;
        case 'W':
            distance = -30.0;
            Axial = 3;
            break;
        default:
            break;
        }
        if (ch == 'X' || ch == 'Y' || ch == 'Z' || ch == 'U' || ch == 'V' ||
            ch == 'W') {
            TransAlongBaseFrameByDistance(rshd, distance, Axial, 0.2, 0.2);
        }
    }
}

// 测试问题7：测试机械臂正逆解的功能，八组逆解是否DH补偿
void bhwk_test7(RSHD rshd)
{
    aubo_robot_namespace::wayPoint_S wayPoint;

    ik_solutions solutions;

    aubo_robot_namespace::Pos targetPosition = { -0.235867, -0.5557, 0.237866 };
    aubo_robot_namespace::Ori targetOri = { -0.193377, 0.471923, 0.86038,
                                            -0.00755828 };

    //逆解
    double startPointJointAngle[aubo_robot_namespace::ARM_DOF] = {
        1.5632 / 180.0 * M_PI,    -62.4792 / 180.0 * M_PI,
        -140.7880 / 180.0 * M_PI, 100.1395 / 180.0 * M_PI,
        0.5601 / 180.0 * M_PI,    -83.1491 / 180.0 * M_PI
    };

    if (RS_SUCC == rs_inverse_kin(rshd, startPointJointAngle, &targetPosition,
                                  &targetOri, &wayPoint)) {
        std::cout << "逆解成功！" << std::endl;
        std::cout << "系统默认的最优逆解如下：" << std::endl;
        printRoadPoint(&wayPoint);
    } else {
        std::cerr << "逆解失败！" << std::endl;
    }

    if (RS_SUCC == rs_inverse_kin_closed_form(rshd, &targetPosition, &targetOri,
                                              &solutions)) {
        std::cout << "逆解的个数 = " << solutions.solution_count << std::endl;
        for (int i = 0; i < solutions.solution_count; i++) {
            std::cout << "第" << i + 1 << "组逆解" << std::endl;
            printRoadPoint(&solutions.waypoint[i]);
        }
    } else {
        std::cerr << "获取逆解集失败！" << std::endl;
    }

    /** 接口调用: 初始化运动属性 ***/
    rs_init_global_move_profile(rshd);

    /** 接口调用: 设置关节型运动的最大加速度 ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[1] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[2] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[3] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[4] = 10.0 / 180.0 * M_PI;
    jointMaxAcc.jointPara[5] = 10.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxacc(rshd, &jointMaxAcc);

    /** 接口调用: 设置关节型运动的最大速度 ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[1] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[2] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[3] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[4] = 10.0 / 180.0 * M_PI;
    jointMaxVelc.jointPara[5] = 10.0 / 180.0 * M_PI; //接口要求单位是弧度
    rs_set_global_joint_maxvelc(rshd, &jointMaxVelc);

    rs_move_joint(rshd, startPointJointAngle);

    double target_pos[6];
    for (int i = 0; i < 6; i++) {
        target_pos[i] = solutions.waypoint[3].jointpos[i];
    }
    rs_move_joint(rshd, target_pos);

    rs_get_current_waypoint(rshd, &wayPoint);

    std::cout << "系统第四组逆解值为：" << wayPoint.cartPos.position.x << " "
              << wayPoint.cartPos.position.y << " "
              << wayPoint.cartPos.position.z << "\n";
}

// 测试问题8：测试move_stop函数
void bhwk_test8(RSHD rshd)
{
    int ret = rs_move_stop(rshd);

    if (RS_SUCC == ret) {
        std::cout << "机械臂运动停止成功！" << std::endl;
    } else {
        std::cerr << "机械臂运动停止失败！" << std::endl;
    }
}

// 测试问题9：测试实时路点信息回调函数
bool bhwk_test9(RSHD rshd)
{
    bool result = false;

    //允许实时路点信息推送
    if (RS_SUCC == rs_enable_push_realtime_roadpoint(rshd, true)) {
        if (RS_SUCC == rs_setcallback_realtime_roadpoint(
                           rshd, callback_RealTimeRoadPoint, NULL)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            result = true;
        } else {
            std::cerr << "获取实时路点信息失败！" << std::endl;
        }
    } else
        std::cerr << "实时路点信息推送失败！" << std::endl;

    return result;
}

// 测试问题10：检测机械臂电源状态
void bhwk_test10(RSHD rshd)
{
    int i = 0;
    while (1) {
        if (i == 5) {
            if (rs_robot_shutdown(rshd) == RS_SUCC) {
                std::cout << "机械臂断电成功" << std::endl;
            } else {
                std::cerr << "机械臂断电失败" << std::endl;
            }
        }
        RobotDiagnosis info;
        int ret = rs_get_diagnosis_info(rshd, &info);

        if (info.armPowerStatus) {
            std::cout << "机械臂已上电！" << std::endl;
        } else {
            std::cout << "机械臂已下电！" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        i++;
    }
}

// 测试问题11：原路径返回退出奇异区？
bool bhwk_test11(RSHD rshd)
{
    bool result = false;

    //    //首先移动到初始位置
    //    bhwk_test2(rshd);

    //    //获取当前路点信息
    //    aubo_robot_namespace::wayPoint_S wayPoint;

    //    //逆解位置信息
    //    aubo_robot_namespace::wayPoint_S targetPoint;

    //    //目标位置对应的关节角
    //    double targetRadian[ARM_DOF] = { 0 };

    //    //目标位置
    //    Pos pos = { -0.489605, -0.155672, 0.448430 };

    //    if (RS_SUCC == rs_get_current_waypoint(rshd, &wayPoint)) {
    //        //参考当前姿态逆解得到六个关节角
    //        if (RS_SUCC == rs_inverse_kin(rshd, wayPoint.jointpos, &pos,
    //                                      &wayPoint.orientation,
    //                                      &targetPoint)) {
    //            //将得到目标位置,将6关节角度设置为用户给定的角度（必须在+-175度）
    //            targetRadian[0] = targetPoint.jointpos[0];
    //            targetRadian[1] = targetPoint.jointpos[1];
    //            targetRadian[2] = targetPoint.jointpos[2];
    //            targetRadian[3] = targetPoint.jointpos[3];
    //            targetRadian[4] = targetPoint.jointpos[4];
    //            targetRadian[5] = targetPoint.jointpos[5];

    //            //直线运动到目标位置
    //            if (RS_SUCC == rs_move_line(rshd, targetRadian)) {
    //                std::cout << "直线运动到目标位置！" << std::endl;
    //            } else {
    //                std::cerr << "直线运动到目标位置失败！" << std::endl;
    //                double OriginPos[6] = {
    //                    49.27 / 180 * M_PI, 55.14 / 180 * M_PI, -87.87 / 180 *
    //                    M_PI, 30.39 / 180 * M_PI, 44.63 / 180 * M_PI, -36.66 /
    //                    180 * M_PI
    //                };
    //                if (RS_SUCC == rs_move_line(rshd, OriginPos)) {
    //                    std::cout << "原路径返回成功，可正常退出奇异区！"
    //                              << std::endl;
    //                }
    //            }

    //        } else {
    //            std::cerr << "逆解失败！" << std::endl;
    //        }

    //    } else {
    //        std::cerr << "获取当前路点信息失败！" << std::endl;
    //    }

    SetRobotActualSpeed(rshd, 0.2);

    //该位置为机械臂的初始位置
    double initPos[6] = { 1.72 / 180 * M_PI,   48.21 / 180 * M_PI,
                          -2.03 / 180 * M_PI,  39.78 / 180 * M_PI,
                          -89.82 / 180 * M_PI, 1.70 / 180 * M_PI };

    //首先运动到初始位置
    if (rs_move_joint(rshd, initPos) == RS_SUCC) {
        std::cout << "机械臂轴动成功！" << std::endl;
    } else {
        std::cerr << "机械臂轴动失败！" << std::endl;
    }

    //奇异位置
    double pos[6] = { 1.73 / 180 * M_PI,   48.28 / 180 * M_PI,
                      -1.90 / 180 * M_PI,  39.85 / 180 * M_PI,
                      -89.82 / 180 * M_PI, 1.71 / 180 * M_PI };

    //直线运动到目标位置
    if (RS_SUCC == rs_move_line(rshd, pos)) {
        std::cout << "直线运动到目标位置！" << std::endl;
    } else {
        std::cerr << "直线运动到目标位置失败！" << std::endl;
        double OriginPos[6] = { 49.27 / 180 * M_PI,  55.14 / 180 * M_PI,
                                -87.87 / 180 * M_PI, 30.39 / 180 * M_PI,
                                44.63 / 180 * M_PI,  -36.66 / 180 * M_PI };
        if (RS_SUCC == rs_move_line(rshd, OriginPos)) {
            std::cout << "原路径返回成功，可正常退出奇异区！" << std::endl;
        }
    }

    return result;
}

// 测试问题12：机械臂轨迹运动测试——圆弧、圆、MoveP 重点MoveP
void bhwk_test12(RSHD rshd)
{
    //轨迹运动 rs_move_track
    //示例1：圆运动
    //    moveTrack1(rshd);
    //示例2：圆弧运动
    moveTrack2(rshd);
    //示例3：MOVEP运动
    moveTrack3(rshd);
}
