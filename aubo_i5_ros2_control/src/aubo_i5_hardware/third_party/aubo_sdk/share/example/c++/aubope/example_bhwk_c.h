#ifndef EXAMPLE_BHWK_C_H
#define EXAMPLE_BHWK_C_H

#include "AuboRobotMetaType.h"
#include "rsdef.h"

// 机械臂登录
bool example_login(RSHD &rshd, const char *addr, int port);
void example_move_stop(RSHD rshd);
// 机械臂关机（必须连接真实机械臂）
bool example_robotShutdown(RSHD rshd);

// 设置机械臂实际速度
void SetRobotActualSpeed(RSHD rshd, double speed);

// 绕基坐标轴旋转角度
void RotateAroundBaseFrameByAngle(RSHD rshd, double angle, int Axial,
                                  double speed, double acc);

// 绕TCP坐标系旋转角度
void RotateAroundTCPFrameByAngle(RSHD rshd, double angle, int Axial,
                                 double speed, double acc);
// 沿基坐标系位置偏移量
int TransAlongBaseFrameByDistance(RSHD rshd, double distance, int Axial,
                                  double speed, double acc);

// 沿TCP坐标系位置偏移量
int TransAlongTCPFrameByDistance(RSHD rshd, double distance, int Axial,
                                 double speed, double acc);

// 打印路点
void printRoadPoint(const aubo_robot_namespace::wayPoint_S *wayPoint);

// 实时路点信息回调
void callback_RealTimeRoadPoint(
    const aubo_robot_namespace::wayPoint_S *wayPoint, void *arg);

//轨迹运动 rs_move_track
//示例1：圆运动
void moveTrack1(RSHD rshd);
//示例2：圆弧运动
void moveTrack2(RSHD rshd);
//示例3：MOVEP运动
void moveTrack3(RSHD rshd);

// 测试问题1：测试DH补偿是否成功
bool bhwk_test1(RSHD rshd);

// 测试问题2：机械臂轴动
bool bhwk_test2(RSHD rshd);

// 测试问题3：机械臂在工具坐标系下做旋转运动
void bhwk_test3(RSHD rshd);

// 测试问题4：机械臂在基坐标系下做旋转运动
void bhwk_test4(RSHD rshd);

// 测试问题5：机械臂在工具坐标系下做位置偏移运动
void bhwk_test5(RSHD rshd);

// 测试问题6：机械臂在基坐标系下做位置偏移运动
void bhwk_test6(RSHD rshd);

// 测试问题7：测试机械臂正逆解的功能，八组逆解是否DH补偿
void bhwk_test7(RSHD rshd);

// 测试问题8：测试move_stop函数
void bhwk_test8(RSHD rshd);

// 测试问题9：测试实时路点信息回调函数
bool bhwk_test9(RSHD rshd);

// 测试问题10：检测机械臂电源状态
void bhwk_test10(RSHD rshd);

// 测试问题11：原路径返回退出奇异区？
bool bhwk_test11(RSHD rshd);

// 测试问题12：机械臂轨迹运动测试——圆弧、圆、MoveP 重点MoveP
void bhwk_test12(RSHD rshd);

#endif // EXAMPLE_BHWK_C_H
