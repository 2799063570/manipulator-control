#ifndef RSDEF_H
#define RSDEF_H
#include <vector>
#include "rstype.h"
#include "AuboRobotMetaType.h"

#define TRUE              1
#define FALSE             0
#define RS_SUCC           0
#define RS_FAILED         -1
#define MAX_RS_INSTANCE   32
#define POS_SIZE          3
#define ORI_SIZE          4
#define INERTIA_SIZE      6
#define JOINT_RADIAN_SIZE 6

#define RS_UNUSED(x) (void)x;
//#define _DEBUG

using namespace aubo_robot_namespace;

typedef CoordCalibrateByJointAngleAndTool CoordCalibrate;

typedef struct
{
    CoordCalibrate user_coord;
    char used;
} CustomUserCoord;

typedef struct
{
    double rotateAxis[3];
} Move_Rotate_Axis;

typedef struct
{
    wayPoint_S waypoint[8];
    int solution_count;
} ik_solutions;

#ifdef __cplusplus
extern "C" {
#endif

// library initialize and uninitialize
/**
 * @brief 初始化机械臂控制库
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_initialize(void);

/**
 * @brief 反初始化机械臂控制库
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_uninitialize(void);

// robot service context
/**
 * @brief 创建机械臂控制上下文句柄
 * @param rshd
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_create_context(
    RSHD *rshd /*returned context handle*/);

/**
 * @brief 注销机械臂控制上下文句柄
 * @param rshd
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_destory_context(RSHD rshd);

// login logout
/**
 * @brief 链接机械臂服务器
 * @param rshd 械臂控制上下文句柄
 * @param addr 机械臂服务器的IP地址
 * @param port 机械臂服务器的端口号
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_login(RSHD rshd, const char *addr,
                                          int port);

/**
 * @brief 断开机械臂服务器链接
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_logout(RSHD rshd);

/**
 * @brief 获取当前的连接状态
 * @param rshd 械臂控制上下文句柄
 * @param status true 在线 false 离线
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_login_status(RSHD rshd, bool *status);
// set move profile
/**
 * @brief 初始化全局的运动属性
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_init_global_move_profile(RSHD rshd);

// joint max acc,velc
/**
 * @brief 设置六个关节的最大加速度
 * @param rshd 械臂控制上下文句柄
 * @param max_acc 六个关节的最大加速度，单位(rad/ss)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_global_joint_maxacc(
    RSHD rshd, const JointVelcAccParam *max_acc);

/**
 * @brief 设置六个关节的最大速度
 * @param rshd 械臂控制上下文句柄
 * @param max_velc 六个关节的最大速度，单位(rad/s)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_global_joint_maxvelc(
    RSHD rshd, const JointVelcAccParam *max_velc);

/**
 * @brief 获取六个关节的最大加速度
 * @param rshd 械臂控制上下文句柄
 * @param max_acc 返回六个关节的最大加速度单位(rad/s^2)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_global_joint_maxacc(
    RSHD rshd, JointVelcAccParam *max_acc);

/**
 * @brief 获取六个关节的最大速度
 * @param rshd 械臂控制上下文句柄
 * @param max_velc 返回六个关节的最大加度单位(rad/s)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_global_joint_maxvelc(
    RSHD rshd, JointVelcAccParam *max_velc);

// end line max acc,velc
/**
 * @brief 设置机械臂末端最大线加速度
 * @param rshd 械臂控制上下文句柄
 * @param max_acc 末端最大加线速度，单位(m/s^2)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_global_end_max_line_acc(RSHD rshd,
                                                                double max_acc);

/**
 * @brief 设置机械臂末端最大线速度
 * @param rshd 械臂控制上下文句柄
 * @param max_velc 末端最大线速度，单位(m/s)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_global_end_max_line_velc(
    RSHD rshd, double max_velc);

/**
 * @brief 获取机械臂末端最大线加速度
 * @param rshd 械臂控制上下文句柄
 * @param max_acc 机械臂末端最大线加速度，单位(m/s^2)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_global_end_max_line_acc(
    RSHD rshd, double *max_acc);

/**
 * @brief 获取机械臂末端最大线速度
 * @param rshd 械臂控制上下文句柄
 * @param max_velc 机械臂末端最大线速度，单位(m/s)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_global_end_max_line_velc(
    RSHD rshd, double *max_velc);

// end line max acc,velc
/**
 * @brief 设置机械臂末端最大角加速度
 * @param rshd 械臂控制上下文句柄
 * @param max_acc 末端最大角加速度，单位(rad/s^2)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_global_end_max_angle_acc(
    RSHD rshd, double max_acc);

/**
 * @brief 设置机械臂末端最大角速度
 * @param rshd 械臂控制上下文句柄
 * @param max_velc 末端最大速度，单位(rad/s)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_global_end_max_angle_velc(
    RSHD rshd, double max_velc);

/**
 * @brief 获取机械臂末端最大角加速度
 * @param rshd 械臂控制上下文句柄
 * @param max_acc 机械臂末端最大角加速度，单位(m/s^2)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_global_end_max_angle_acc(
    RSHD rshd, double *max_acc);

/**
 * @brief 获取机械臂末端最大角加速度
 * @param rshd 械臂控制上下文句柄
 * @param max_velc 机械臂末端最大角速度，单位(m/s)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_global_end_max_angle_velc(
    RSHD rshd, double *max_velc);

/**
 * @brief 设置机械臂关节运动范围
 * @param rshd 械臂控制上下文句柄
 * @param joint_range
 * 机械臂每个关节的运动范围（±175°）注意！！！单位使用弧度（rad）
 * @param enable 使能关节运动范围控制 true：启用 false：禁用
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_robot_joint_motion_range(
    RSHD rshd, RangeOfMotion joint_range[ARM_DOF], bool enable = true);

// robot move
/**
 * @brief 机械臂轴动
 * @param rshd 械臂控制上下文句柄
 * @param joint_radian 六个关节的关节角，单位(rad)
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_joint(RSHD rshd,
                                               double joint_radian[ARM_DOF],
                                               bool isblock = true);

/**
 * @brief 机械臂轴动
 * @param rshd 械臂控制上下文句柄
 * @param move_profile 运动属性
 * @param joint_radian 六个关节的关节角，单位(rad)
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_joint_ex(RSHD rshd,
                                                  MoveProfile_t *move_profile,
                                                  double joint_radian[ARM_DOF],
                                                  bool isblock = true);

/**
 * @brief 跟随模式机械臂轴动
 * @param rshd 械臂控制上下文句柄
 * @param joint_radian 六个关节的关节角，单位(rad)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_follow_mode_move_joint(
    RSHD rshd, double joint_radian[ARM_DOF]);

/**
 * @brief 机械臂保持当前姿态直线运动
 * @param rshd 械臂控制上下文句柄
 * @param joint_radian 六个关节的关节角，单位(rad)
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_line(RSHD rshd,
                                              double joint_radian[ARM_DOF],
                                              bool isblock = true);

/**
 * @brief 机械臂保持当前姿态直线运动
 * @param rshd 械臂控制上下文句柄
 * @param move_profile 运动属性
 * @param joint_radian 六个关节的关节角，单位(rad)
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_line_ex(RSHD rshd,
                                                 MoveProfile_t *move_profile,
                                                 double joint_radian[ARM_DOF],
                                                 bool isblock = true);

/**
 * @brief rs_move_rotate_to_waypoint保持当前位置变换姿态旋转运动至目标路点
 * @param rshd 械臂控制上下文句柄
 * @param target_waypiont 目标路点
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_rotate_to_waypoint(
    RSHD rshd, aubo_robot_namespace::wayPoint_S *target_waypiont,
    bool isblock = true);

/**
 * @brief 保持当前位置变换姿态做旋转运动
 * @param rshd 械臂控制上下文句柄
 * @param user_coord 用户坐标系
 * @param rotate_axis :转轴(x,y,z) 例如：(1,0,0)表示沿Y轴转动
 * @param rotate_angle 旋转角度 单位（rad）
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_rotate(
    RSHD rshd, const CoordCalibrate *user_coord,
    const Move_Rotate_Axis *rotate_axis, double rotate_angle,
    bool isblock = true);

/**
 * rs_get_rotate_target_waypiont
 * 根据根据起始点姿态及基坐标系下描述的旋转轴、旋转角，获取目标位姿
 * @param rshd 械臂控制上下文句柄
 * @param originateWayPointOnBaseCoord 起始路点信息
 * @param rotateAxisOnBaseCoord 基坐标系下表示的旋转轴
 * @param rotateAngle 旋转角
 * @param targetWayPointOnBaseCoord 目标路点信息 （传出参数）
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_rotate_target_waypiont(
    RSHD rshd,
    const aubo_robot_namespace::wayPoint_S *originateWayPointOnBaseCoord,
    const double rotateAxisOnBaseCoord[], double rotateAngle,
    aubo_robot_namespace::wayPoint_S *targetWayPointOnBaseCoord);

/**
 * @brief rs_get_rotateaxis_user_to_Base
 * 将用户坐标系下描述的旋转轴转换到基坐标系下描述
 * @param rshd 械臂控制上下文句柄
 * @param oriOnUserCoord 用户做标系旋转姿态
 * @param rotateAxisOnUserCoord 用户坐标系下描述的旋转轴
 * @param rotateAxisOnBaseCoord 基坐标系下描述的旋转轴
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_rotateaxis_user_to_Base(
    RSHD rshd, const aubo_robot_namespace::Ori *oriOnUserCoord,
    const double rotateAxisOnUserCoord[], double rotateAxisOnBaseCoord[]);

/**
 * @brief 清除所有已经设置的全局路点
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_remove_all_waypoint(RSHD rshd);

/**
 * @brief 添加全局路点用于轨迹运动
 * @param rshd 械臂控制上下文句柄
 * @param joint_radian 六个关节的关节角，单位(rad)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_add_waypoint(RSHD rshd,
                                                 double joint_radian[ARM_DOF]);

/**
 * @brief 设置交融半径
 * @param rshd 械臂控制上下文句柄
 * @param radius 交融半径，单位(m)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_blend_radius(RSHD rshd, double radius);

/**
 * @brief 设置圆运动圈数
 * @param rshd 械臂控制上下文句柄
 * @param times 当times大于0时，机械臂进行圆运动times次
 *              当times等于0时，机械臂进行圆弧轨迹运动
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_circular_loop_times(RSHD rshd,
                                                            int times);

/**
 * @brief 设置用户坐标系
 * @param rshd 械臂控制上下文句柄
 * @param user_coord 用户坐标系
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_user_coord(
    RSHD rshd, const CoordCalibrate *user_coord);

/**
 * @brief 设置基座坐标系
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_base_coord(RSHD rshd);

/**
 * @brief 检查用户坐标系参数设置是否合理
 * @param rshd 械臂控制上下文句柄
 * @param user_coord 用户坐标系
 * @return RS_SUCC 成功 其他失败 合理返回: 0 不合理返回: 其他
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_check_user_coord(
    RSHD rshd, const CoordCalibrate *user_coord);

/**
 * @brief 检查用户坐标系标定
 * @param rshd
 * @param user_coord
 * @param bInWPos 在世界坐标系中的位置
 * @param bInWOri 在世界坐标系中的姿态
 * @param wInBPos 在基座坐标系中的位置
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_user_coord_calibrate(
    RSHD rshd, const CoordCalibrate *user_coord, double bInWPos[3],
    double bInWOri[9], double wInBPos[3]);

/**
 * @brief rs_tool_calibration 工具标定　　该函数能标定出工具的位置信息和姿态信息
 * @param toolCalibrate           工具标定结构体 用于储存标定点，标定方法等
 * @param toolInEndDesc　　　　　　　标定的结果
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_tool_calibration(
    RSHD rshd, const ToolCalibrate &toolCalibrate,
    ToolInEndDesc &toolInEndDesc);
/**
 * @brief 设置基于基座标系运动偏移量
 * @param rshd 械臂控制上下文句柄
 * @param relative 相对位移(x, y, z) 单位(m)
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_relative_offset_on_base(
    RSHD rshd, const MoveRelative *relative);

/**
 * @brief 设置基于用户标系运动偏移量
 * @param rshd 械臂控制上下文句柄
 * @param relative 相对位移(x, y, z) 单位(m)
 * @param user_coord 用户坐标系
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_relative_offset_on_user(
    RSHD rshd, const MoveRelative *relative, const CoordCalibrate *user_coord);

/**
 * @brief 取消提前到位设置
 * @param rshd
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_no_arrival_ahead(RSHD rshd);

/**
 * @brief 设置距离模式下的提前到位距离
 * @param rshd
 * @param distance　提前到位距离 单位（米）
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_arrival_ahead_distance(RSHD rshd,
                                                               double distance);

/**
 * @brief 设置时间模式下的提前到位时间
 * @param rshd
 * @param sec 提前到位时间　单位（秒）
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_arrival_ahead_time(RSHD rshd,
                                                           double sec);

/**
 * @brief 设置距离模式下交融半径距离
 * @param rshd
 * @param distance　提前到位距离 单位（米）
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_arrival_ahead_blend(RSHD rshd,
                                                            double radius);

/**
 * @brief 轨迹运动
 * @param rshd 械臂控制上下文句柄
 * @param sub_move_mode 轨迹类型:
 *                      2:圆弧
 *                      3:轨迹
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_track(RSHD rshd,
                                               move_track sub_move_mode,
                                               bool isblock = true);

/**
 * @brief
 * 保持当前位姿通过直线运动的方式运动到目标位置,其中目标位置是通过相对当前位置的偏移给出
 * @param rshd 械臂控制上下文句柄
 * @param target 基于用户平面表示的目标位置
 * @param tool   工具参数
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_line_to(RSHD rshd, const Pos *target,
                                                 const ToolInEndDesc *tool,
                                                 bool isblock = true);

/**
 * @brief 保持当前位姿通过关节运动的方式运动到目标位置
 * @param rshd 械臂控制上下文句柄
 * @param target 基于用户平面表示的目标位置
 * @param tool 工具参数
 * @param isblock    isblock==true
 * 代表阻塞，机械臂运动直到到达目标位置或者出现故障后返回。 isblock==false
 * 代表非阻塞，立即返回，运动指令发送成功就返回，函数返回后机械臂开始运动。
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_joint_to(RSHD rshd, const Pos *target,
                                                  const ToolInEndDesc *tool,
                                                  bool isblock = true);

/**
 * @brief 设置示教坐标系
 * @param rshd 械臂控制上下文句柄
 * @param user_coord 示教坐标系
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_teach_coord(
    RSHD rshd, const CoordCalibrate *teach_coord);

/**
 * @brief 开始轴动示教
 * @param rshd 械臂控制上下文句柄
 * @param mode 示教关节:JOINT1,JOINT2,JOINT3, JOINT4,JOINT5,JOINT6,
 * 位置示教:MOV_X,MOV_Y,MOV_Z   姿态示教:ROT_X,ROT_Y,ROT_Z
 * @param dir  运动方向   正方向true  反方向false
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_teach_move_start(RSHD rshd, teach_mode mode,
                                                     bool dir);

/**
 * @brief 结束示教
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_teach_move_stop(RSHD rshd);

/**
 * @brief 清理服务器上的非在线轨迹运动数据
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_clear_offline_track(RSHD rshd);

/**
 * @brief 向服务器添加非在线轨迹运动路点
 * @param rshd 械臂控制上下文句柄
 * @param waypoints 路点数组 (路点个数小于等于3000)
 * @param waypoint_count 路点数组大小
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_append_offline_track_waypoint(
    RSHD rshd, const JointParam waypoints[], int waypoint_count);

/**
 * @brief 向服务器添加非在线轨迹运动路点文件
 * @param rshd 械臂控制上下文句柄
 * @param filename
 * 路点文件全路径,路点文件的每一行包含六个关节的关节角(弧度),用逗号隔开
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_append_offline_track_file(
    RSHD rshd, const char *filename);

/**
 * @brief 通知服务器启动非在线轨迹运动
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_startup_offline_track(RSHD rshd);

/**
 * @brief 通知服务器停止非在线轨迹运动
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_stop_offline_track(RSHD rshd);

/**
 * @brief 通知服务器启动非在线轨迹运动
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_startup_excit_traj_track(
    RSHD rshd, const char *trackfile, char type, char subtype);

/**
 * @brief 设置轨迹回放采样周期
 * @param rshd 械臂控制上下文句柄
 * @param second 采样周期（秒）
 * @param RS_SUCC 成功 其他失败
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_track_playback_cycle(RSHD rshd,
                                                             double second);

/**
 * @brief rs_get_dynidentify_results
 * @param param
 * @param len
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT bool rs_get_dynidentify_results(
    RSHD rshd, std::vector<int> &paramVector);

/**
 * @brief 通知服务器进入TCP2CANBUS透传模式
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_enter_tcp2canbus_mode(RSHD rshd);

/**
 * @brief 通知服务器退出TCP2CANBUS透传模式
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_leave_tcp2canbus_mode(RSHD rshd);

/**
 * @brief 透传运动路点到CANBUS
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_waypoint_to_canbus(
    RSHD rshd, double joint_radian[ARM_DOF]);

/**
 * @brief 透传运动路点到CANBUS
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_waypoints_to_canbus(
    RSHD rshd, double joint_radian[][ARM_DOF], int waypint_count);

/**
 * @brief 正解　　　　　此函数为正解函数，已知关节角求对应位置的位置和姿态。
 * @param rshd 械臂控制上下文句柄
 * @param joint_radian 六个关节的关节角，单位(rad)
 * @param waypoint 六个关节角,位置,姿态
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_forward_kin(
    RSHD rshd, const double joint_radian[ARM_DOF], wayPoint_S *waypoint);

/**
 * @brief 逆解
 * 此函数为机械臂逆解函数，根据位置信息(x,y,z)和对应位置的参考姿态(w,x,y,z)得到对应位置的关节角信息。
 * @param rshd 械臂控制上下文句柄
 * @param joint_radian 参考关节角（通常为当前机械臂位置）单位(rad)
 * @param pos 目标路点的位置 单位:米
 * @param ori 目标路点的参考姿态
 * @param waypoint 目标路点信息
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_inverse_kin(RSHD rshd,
                                                double joint_radian[ARM_DOF],
                                                const Pos *pos, const Ori *ori,
                                                wayPoint_S *waypoint);

/**
 * @brief 逆解
 * 此函数为机械臂逆解函数，根据位置信息(x,y,z)和对应位置的参考姿态(w,x,y,z)得到对应位置的关节角信息。
 * @param rshd 械臂控制上下文句柄
 * @param pos 目标路点的位置 单位:米
 * @param ori 目标路点的参考姿态
 * @param ik_solutions 目标路点信息（最多八组解）
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_inverse_kin_closed_form(
    RSHD rshd, const Pos *pos, const Ori *ori, ik_solutions *solutions);

/**
 * @brief 用户坐标系转基座坐标系
 *        概述:
 * 将法兰盘中心基于基座标系下的位置和姿态　转成　工具末端基于用户座标系下的位置和姿态。
 *
 *      　扩展1:  法兰盘中心可以看成是一个特殊的工具，即工具的位置为(0,0,0)
 *
 * 因此当工具为(0,0,0)时，相当于将法兰盘中心基于基座标系下的位置和姿态　转成　法兰盘中心基于用户座标系下的位置和姿态。
 *
 * 　　　　扩展2:  用户坐标系也可以选择成基座标系，　　即：userCoord.coordType =
 * BaseCoordinate
 *               因此当用户平面为基座标系时，相当于将法兰盘中心基于基座标系下的位置和姿态　转成　工具末端基于基座标系下的位置和姿态，
 *               即在基座标系加工具。
 * @param rshd 械臂控制上下文句柄
 * @param pos_onbase 基于基座标系的法兰盘中心位置信息（x,y,z）  单位(m)
 * @param ori_onbase 基于基座标系的姿态信息(w, x, y, z)
 * @param user_coord 用户坐标系
 * @param tool_pos   工具信息
 * @param pos_onuser 基于用户座标系的工具末端位置信息,输出参数
 * @param ori_onuser 基于用户座标系的工具末端姿态信息,输出参数
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_base_to_user(
    RSHD rshd, const Pos *pos_onbase, const Ori *ori_onbase,
    const CoordCalibrate *user_coord, const ToolInEndDesc *tool_pos,
    Pos *pos_onuser, Ori *ori_onuser);

/**
 * @brief 用户坐标系转基座标系
 * @param rshd 械臂控制上下文句柄
 * @param pos_onuser 基于用户座标系的工具末端位置信息
 * @param ori_onuser 基于用户座标系的工具末端姿态信息
 * @param user_coord 用户坐标系
 * @param tool_pos   工具信息
 * @param pos_onbase 基于基座标系的法兰盘中心位置信息
 * @param ori_onbase 基于基座标系的姿态信息
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_user_to_base(
    RSHD rshd, const Pos *pos_onuser, const Ori *ori_onuser,
    const CoordCalibrate *user_coord, const ToolInEndDesc *tool_pos,
    Pos *pos_onbase, Ori *ori_onbase);

/**
 * @brief 　基坐标系转基座标得到工具末端点的位置和姿态
 * @param rshd 械臂控制上下文句柄
 * @param flange_center_pos_onbase 基于基座标系的法兰盘中心位置信息
 * @param flange_center_ori_onbase 基于基座标系的姿态信息
 * @param tool 工具信息
 * @param tool_end_pos_onbase 基于基座标系的工具末端位置信息
 * @param tool_end_ori_onbase 基于基座标系的工具末端姿态信息
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_base_to_base_additional_tool(
    RSHD rshd, const Pos *flange_center_pos_onbase,
    const Ori *flange_center_ori_onbase, const ToolInEndDesc *tool,
    Pos *tool_end_pos_onbase, Ori *tool_end_ori_onbase);

/**
 * @brief rs_get_target_waypoint_by_position   　　根据位置获取目标路点信息
 * @param sourceWayPointOnBaseCoord　　　　　起始路点（基于基坐标系）
 * @param userCoordSystem　　　　　　　　　　　用户坐标系
 * @param toolEndPosition　　　　　　　　　　　工具末端位置
 * @param toolInEndDesc　　　　　　　　　　　　工具末端偏移
 * @param targetWayPointOnBaseCoord　　　　 目标路点（基于基坐标系）
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_target_waypoint_by_position(
    RSHD rshd,
    const aubo_robot_namespace::wayPoint_S &sourceWayPointOnBaseCoord,
    const aubo_robot_namespace::CoordCalibrateByJointAngleAndTool
        &userCoordSystem,
    const aubo_robot_namespace::Pos &toolEndPosition,
    const aubo_robot_namespace::ToolInEndDesc &toolInEndDesc,
    aubo_robot_namespace::wayPoint_S &targetWayPointOnBaseCoord);

/**
 * @brief 欧拉角转四元素
 * @param rshd 械臂控制上下文句柄
 * @param rpy 姿态的欧拉角表示方法
 * @param ori 姿态的四元素表示方法
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_rpy_to_quaternion(RSHD rshd, const Rpy *rpy,
                                                      Ori *ori);

/**
 * @brief 四元素转欧拉角
 * @param rshd 械臂控制上下文句柄
 * @param ori  姿态的四元素表示方法
 * @param rpy  姿态的欧拉角表示方法
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_quaternion_to_rpy(RSHD rshd, const Ori *ori,
                                                      Rpy *rpy);

/**
 * @brief 设置工具的运动学参数
 * @param rshd 械臂控制上下文句柄
 * @param tool 工具的运动学参数
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_tool_end_param(
    RSHD rshd, const ToolInEndDesc *tool);

// end tool parameters
/**
 * @brief 设置无工具的动力学参数
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_none_tool_dynamics_param(RSHD rshd);

/**
 * @brief 设置工具的动力学参数
 * @param rshd 械臂控制上下文句柄
 * @param tool 工具的动力学参数
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_tool_dynamics_param(
    RSHD rshd, const ToolDynamicsParam *tool);

/**
 * @brief 获取工具的动力学参数
 * @param rshd 械臂控制上下文句柄
 * @param tool 工具的动力学参数
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_tool_dynamics_param(
    RSHD rshd, ToolDynamicsParam *tool);

/**
 * @brief 获取工具的动力学参数
 * @param rshd 械臂控制上下文句柄
 * @param tool 工具的动力学参数
 * @return RS_SUCC 成功 其他失败
 * @note 由于rs_get_tool_dynamics_param获取的位置值单位是mm，增加此
 * 函数将单位与rs_set_tool_dynamics_param统一
 */
inline int rs_get_tool_dynamics_param2(RSHD rshd, ToolDynamicsParam *tool)
{
    int retval = rs_get_tool_dynamics_param(rshd, tool);
    tool->positionX /= 1000.;
    tool->positionY /= 1000.;
    tool->positionZ /= 1000.;

    return retval;
}

/**
 * @brief 设置无工具运动学参数
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_none_tool_kinematics_param(RSHD rshd);

/**
 * @brief 设置工具的运动学参数
 * @param rshd 械臂控制上下文句柄
 * @param tool 工具的运动学参数
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_tool_kinematics_param(
    RSHD rshd, const ToolKinematicsParam *tool);

/**
 * @brief 获取工具的运动学参数
 * @param rshd 械臂控制上下文句柄
 * @param tool 工具的运动学参数
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_tool_kinematics_param(
    RSHD rshd, ToolKinematicsParam *tool);

// robot control
/**
 * @brief 启动机械臂
 * @param rshd 械臂控制上下文句柄
 * @param tool_dynamics 动力学参数
 * @param colli_class 碰撞等级
 * @param read_pos 是否允许读取位置
 * @param static_colli_detect 是否允许侦测静态碰撞
 * @param board_maxacc 接口板允许的最大加速度
 * @param state 机械臂启动状态
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_robot_startup(
    RSHD rshd, const ToolDynamicsParam *tool_dynamics, uint8 colli_class,
    bool read_pos, bool static_colli_detect, int board_maxacc,
    ROBOT_SERVICE_STATE *state);

/**
 * @brief 关闭机械臂
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_robot_shutdown(RSHD rshd);

/**
 * @brief 停止机械臂运动加速度设置
 * @param rshd 械臂控制上下文句柄
 * @param jerkRatio [0.1,20]
 * @param double acc 6维数组，
 *                   1，末端型运动，只使用acc[0] 最大加速度
 *                   2，轴动，使用acc[0,5] 关节最大加速度
 * @param size 6
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_reduce_para(RSHD rshd,
                                                    const double jerkRatio,
                                                    const double acc[],
                                                    int size);

/**
 * @brief 停止机械臂运动
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_stop(RSHD rshd);

/**
 * @brief 停止机械臂运动
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_fast_stop(RSHD rshd);

/**
 * @brief 暂停机械臂运动
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_pause(RSHD rshd);

/**
 * @brief 暂停后回复机械臂运动
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_continue(RSHD rshd);

/**
 * @brief 机械臂碰撞后恢复
 * @param rshd 械臂控制上下文句柄
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_collision_recover(RSHD rshd);

/**
 * @brief 获取机械臂当前状态
 * @param rshd 械臂控制上下文句柄
 * @param state 机械臂当前状态
 *              机械臂当前停止:RobotStatus.Stopped
 *              机械臂当前运行:RobotStatus.Running
 *              机械臂当前暂停:RobotStatus.Paused
 *              机械臂当前恢复:RobotStatus.Resumed
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_robot_state(RSHD rshd,
                                                    RobotState *state);

/**
 * @brief 设置机械臂运动进入缩减模式
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT bool rs_enter_reduce_mode(RSHD rshd);

/**
 * @brief 设置机械臂运动退出缩减模式
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT bool rs_exit_reduce_mode(RSHD rshd);

/**
 * @brief 通知机械臂工程启动，服务器同时开始检测安全IO
 * @param rshd
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT bool rs_project_startup(RSHD rshd);

/**
 * @brief 通知机械臂工程停止，服务器停止检测安全IO
 * @param rshd
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT bool rs_project_stop(RSHD rshd);

// tool interface

// robot parameters
/**
 * @brief 设置机械臂服务器工作模式
 * @param rshd 械臂控制上下文句柄
 * @param mode  机械臂服务器工作模式
 *              机械臂仿真模式:RobotRunningMode.RobotModeSimulator
 *              机械臂真实模式:RobotRunningMode.RobotModeReal
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_work_mode(RSHD rshd,
                                                  RobotWorkMode mode);

/**
 * @brief 获取机械臂服务器当前工作模式
 * @param rshd 械臂控制上下文句柄
 * @param mode  机械臂服务器工作模式
 *              机械臂仿真模式:RobotRunningMode.RobotModeSimulator
 *              机械臂真实模式:RobotRunningMode.RobotModeReal
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_work_mode(RSHD rshd,
                                                  RobotWorkMode *mode);

/**
 * @brief 获取重力分量
 * @param rshd 械臂控制上下文句柄
 * @param gravity 重力分量
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_gravity_component(
    RSHD rshd, RobotGravityComponent *gravity);

/**
 * @brief 设置机械臂碰撞等级
 * @param rshd 械臂控制上下文句柄
 * @param grade 碰撞等级 范围（0～10）
 * @param mode  碰撞模式
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_collision_class(RSHD rshd, int grade);
SERVICE_INTERFACE_ABI_EXPORT int rs_set_collision_class2(RSHD rshd, int grade,
                                                         CollisionMode mode);

/**
 * @brief 获取当前碰撞等级
 * @param rshd 机械臂控制上下文句柄
 * @param grade 碰撞等级 范围（0~10）
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_collision_class(RSHD rshd, int *grade);

/**
 * @brief 获取设备信息
 * @param rshd 械臂控制上下文句柄
 * @param dev 设备信息
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_device_info(RSHD rshd,
                                                    RobotDevInfo *dev);

/**
 * @brief 获取当前是否已经链接真实机械臂
 * @param rshd 械臂控制上下文句柄
 * @param exist true：存在 false：不存在
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_is_have_real_robot(RSHD rshd, bool *exist);

/**
 * @brief 当前机械臂是否运行在联机模式
 * @param rshd 械臂控制上下文句柄
 * @param isonline true：在 false：不在
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_is_online_mode(RSHD rshd, bool *isonline);

/**
 * @brief 当前机械臂是否运行在联机主模式
 * @param rshd 械臂控制上下文句柄
 * @param ismaster true：主模式 false：从模式
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_is_online_master_mode(RSHD rshd,
                                                          bool *ismaster);

/**
 * @brief 获取机械臂当前状态信息
 * @param rshd 械臂控制上下文句柄
 * @param jointStatus 返回六个关节状态，包括：电流，电压，温度
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_joint_status(
    RSHD rshd, JointStatus jointStatus[ARM_DOF]);

/**
 * @brief 获取机械臂当前位置信息
 * @param rshd 械臂控制上下文句柄
 * @param waypoint 关节位置信息
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_current_waypoint(RSHD rshd,
                                                         wayPoint_S *waypoint);

/**
 * @brief 获取机械臂诊断信息
 * @param rshd 械臂控制上下文句柄
 * @param info 机械臂诊断信息
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_diagnosis_info(
    RSHD rshd, RobotDiagnosis *robotDiagnosisInfo);

/**
 * @brief 根据错误号获取错误信息
 * @param rshd 机械臂控制上下文句柄
 * @param err_code 错误号
 * @return 调用成功返回ErrnoSucc;错误返回错误号
 */
SERVICE_INTERFACE_ABI_EXPORT const char *rs_get_error_information_by_errcode(
    RSHD rshd, RobotErrorCode err_code);

/**
 * @brief 获取socket链接状态
 * @param rshd 械臂控制上下文句柄
 * @param connected true：已连接 false：未连接
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_socket_status(RSHD rshd,
                                                      bool *connected);

/**
 * @brief 获取机械表末端速度
 * @param rshd 械臂控制上下文句柄
 * @param endspeed 末端速度 单位（m/s）
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_robot_end_speed(RSHD rshd,
                                                        float *endspeed);

// IO interaface
/**
 * @brief 获取接口板指定IO集合的配置信息
 * @param rshd 械臂控制上下文句柄
 * @param type IO类型
 * @param config IO配置信息集合
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_board_io_config(
    RSHD rshd, RobotIoType type, std::vector<RobotIoDesc> *config);

/**
 * @brief 根据接口板IO类型和地址设置IO状态
 * @param rshd 械臂控制上下文句柄
 * @param type IO类型
 * @param name IO名称
 * @param val  IO状态
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_board_io_status_by_name(
    RSHD rshd, RobotIoType type, const char *name, double val);

/**
 * @brief 根据接口板IO类型和地址设置IO状态
 * @param rshd 械臂控制上下文句柄
 * @param type IO类型
 * @param addr IO状态
 * @param val  IO状态
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_board_io_status_by_addr(
    RSHD rshd, RobotIoType type, int addr, double val);

/**
 * @brief 根据接口板IO类型和地址获取IO状态
 * @param rshd 械臂控制上下文句柄
 * @param type IO类型
 * @param name IO名称
 * @param val  IO状态
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_board_io_status_by_name(
    RSHD rshd, RobotIoType type, const char *name, double *val);

/**
 * @brief 根据接口板IO类型和地址获取IO状态
 * @param rshd 械臂控制上下文句柄
 * @param type IO类型
 * @param addr IO地址
 * @param val
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_board_io_status_by_addr(
    RSHD rshd, RobotIoType type, int addr, double *val);

// tool device interface
/**
 * @brief 设置工具端电源电压类型
 * @param rshd 械臂控制上下文句柄
 * @param type ower_type:电源类型
 *              0:.OUT_0V
 *              1:.OUT_12V
 *              2:.OUT_24V
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_tool_power_type(RSHD rshd,
                                                        ToolPowerType type);

/**
 * @brief 获取工具端电源电压类型
 * @param rshd 械臂控制上下文句柄
 * @param type ower_type:电源类型
 *              0:.OUT_0V
 *              1:.OUT_12V
 *              2:.OUT_24V
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_tool_power_type(RSHD rshd,
                                                        ToolPowerType *type);

/**
 * @brief 设置工具端数字量IO的类型
 * @param rshd 械臂控制上下文句柄
 * @param addr 地址
 * @param type 类型  0:输入 1:输出
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_tool_io_type(RSHD rshd,
                                                     ToolDigitalIOAddr addr,
                                                     ToolIOType type);

/**
 * @brief 获取工具端电压数值
 * @param rshd 械臂控制上下文句柄
 * @param voltage 电压数值，单位（伏特）
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_tool_power_voltage(RSHD rshd,
                                                           double *voltage);

/**
 * @brief 获取工具端IO状态
 * @param rshd 械臂控制上下文句柄
 * @param name IO名称
 * @param val 工具端IO状态
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_tool_io_status(RSHD rshd,
                                                       const char *name,
                                                       double *val);

/**
 * @brief 设置工具端IO状态
 * @param rshd 械臂控制上下文句柄
 * @param name IO名称
 * @param status 工具端IO状态
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_tool_do_status(RSHD rshd,
                                                       const char *name,
                                                       IO_STATUS status);

/**
 * @brief 暂停工程
 * @param rshd
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_client_status_pause(RSHD rshd);

/**
 * @brief 继续工程
 * @param rshd
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_client_status_continue(RSHD rshd);

/**
 * @brief 工程停止
 * @param rshd
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_client_status_stop(RSHD rshd);

/**
 * @brief 工程启动
 * @param rshd
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_client_status_run(RSHD rshd);

/**
 * @brief 通知接口板上位机停止状态
 * @param data
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_robot_orpe_stop(RSHD rshd, uint8 data);

/**
 * @brief rs_robot_control
 * @param rshd
 * @param cmd
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_robot_control(RSHD rshd,
                                                  RobotControlCommand cmd);

/**
 * @brief 设置机械臂辨识参数
 * @param param
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_robot_recognition_param(
    RSHD rshd, const RobotRecongnitionParam *param);

/**
 * @brief 获取机械臂辨识参数
 * @param type
 * @param param
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_robot_recognition_param(
    RSHD rshd, int type, RobotRecongnitionParam *param);

/**
 * @brief 获取机械臂安全配置
 * @param rshd
 * @param safetyConfig
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_robot_safety_config(
    RSHD rshd, RobotSafetyConfig *param);

/**
 * @brief 获取机械臂安全配置
 * @param rshd
 * @param safetyConfig
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_robot_safety_config(
    RSHD rshd, RobotSafetyConfig *param);

/**
 * @brief 获取机械臂安全状态
 * @param rshd
 * @param safetyStatus
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_robot_safety_status(
    RSHD rshd, OrpeSafetyStatus *param);

//#ifndef FOR_FORCE_LIBRARY
/**
 * @brief 设置底座参数信息
 * @param rshd
 * @param param
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_robot_base_parameters(
    RSHD rshd, const RobotBaseParameters *param);

/**
 * @brief 获取底座参数信息
 * @param rshd
 * @param param
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_robot_base_parameters(
    RSHD rshd, RobotBaseParameters *param);

/**
 * @brief 设置关节参数信息
 * @param rshd
 * @param param
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_robot_joints_parameter(
    RSHD rshd, const RobotJointsParameter *param);

/**
 * @brief 获取关节参数信息
 * @param rshd
 * @param param
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_robot_joints_parameter(
    RSHD rshd, RobotJointsParameter *param);

/**
 * @brief 刷新关节参数信息
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_robot_refresh_robot_arm_paramter(
    RSHD rshd);

//#else

/**
 * @brief 设置调速模式配置
 * @param rshd
 * @param config
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_regulate_speed_param(
    RSHD rshd, const RegulateSpeedModeParamConfig_t *config);

/**
 * @brief 获取调速模式配置
 * @param rshd
 * @param config
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_regulate_speed_param(
    RSHD rshd, RegulateSpeedModeParamConfig_t *config);

/**
 * @brief 使能/失能调速模式
 * @param rshd
 * @param enbaleFlag
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_enable_regulate_speed_mode(RSHD rshd,
                                                               bool enbaleFlag);

/**
 * @brief 获取"主动探寻力"参数
 * @param rshd
 * @param forceLimit
 * @param distLimit
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_force_control_mode_explore_force_param(
    RSHD rshd, double *forceLimit, double *distLimit);

/**
 * @brief 设置"主动探寻力"参数
 * @param rshd
 * @param forceLimit
 * @param distLimit
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_set_force_control_mode_explore_force_param(
    RSHD rshd, double forceLimit, double distLimit);

/**
 * @brief 使能/失能力控模式
 * @param rshd
 * @param enbaleFlag
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_enable_force_control_mode(RSHD rshd,
                                                              bool enbaleFlag);

/**
 * @brief 获取实时力数据
 * @param rshd
 * @param 传出参数
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_get_realtime_force_data(
    RSHD rshd, double forceData[6]);

/**
 * @brief 将一个轨迹文件(位置+姿态)转换为路点容器，进行平滑处理和限制条件检查
 * @param rshd                  械臂控制上下文句柄
 * @param filePath              轨迹文件路径      输入参数
 * @param poseType              位姿类型
 * @param referPointJointAngle  逆解参考点的关节角 输入参数
 * @param toolInEndDesc         末端工具的信息    输入参数
 * @param wayPointVector        路点集合         输出参数
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_parse_file_as_roadpoint_collection(
    RSHD rshd, const char *filePath,
    aubo_robot_namespace::POSITION_ORIENTATION_TYPE poseType,
    const double *referPointJointAngle,
    const aubo_robot_namespace::ToolInEndDesc *toolInEndDesc,
    aubo_robot_namespace::wayPoint_S wayPoint[], int waypointSize,
    int *sizeReturn);

/**
 * @brief 解析路点文件并缓存结果
 * @param rshd      械臂控制上下文句柄
 * @param filePath  轨迹文件路径      输入参数
 * @param referPointJointAngle  逆解参考点的关节角 输入参数
 * @param toolInEndDesc         末端工具的信息    输入参数
 * @param firstWayPoint         轨迹的第一个点    输出参数
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_parse_roadpoint_file_and_cache_result(
    RSHD rshd, const char *filePath, const double *referPointJointAngle,
    aubo_robot_namespace::ToolInEndDesc *toolInEndDesc,
    aubo_robot_namespace::wayPoint_S *firstWayPoint);
/**
 * @brief 运行缓存好的轨迹
 * 即：运行rs_parse_roadpoint_file_and_cache_result的结果
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_move_cached_track(RSHD rshd);

//#endif

// callback
/**
 * @brief 注册用于获取实时路点的回调函数
 * @param rshd 械臂控制上下文句柄
 * @param ptr 获取实时路点信息的函数指针
 * @param arg
 * 这个参数系统不做任何处理，只是进行了缓存，当回调函数触发时该参数会通过回调函数的参数传回
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_setcallback_realtime_roadpoint(
    RSHD rshd, const RealTimeRoadPointCallback ptr, void *arg);

/**
 * @brief 注册用于获取关节状态的回调函数
 * @param rshd 械臂控制上下文句柄
 * @param ptr 获取实时关节状态信息的函数指针
 * @param arg
 * 这个参数系统不做任何处理，只是进行了缓存，当回调函数触发时该参数会通过回调函数的参数传回
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_setcallback_realtime_joint_status(
    RSHD rshd, const RealTimeJointStatusCallback ptr, void *arg);

/**
 * @brief 注册用于获取实时末端速度的回调函数
 * @param rshd 械臂控制上下文句柄
 * @param ptr 获取实时末端速度的函数指针
 * @param arg
 * 个参数系统不做任何处理，只是进行了缓存，当回调函数触发时该参数会通过回调函数的参数传回
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_setcallback_realtime_end_speed(
    RSHD rshd, const RealTimeEndSpeedCallback ptr, void *arg);

/**
 * @brief 注册用于获取机械臂事件信息的回调函数
 * @param rshd 械臂控制上下文句柄
 * @param ptr 获取机械臂事件信息的函数指针
 * @param arg
 * 个参数系统不做任何处理，只是进行了缓存，当回调函数触发时该参数会通过回调函数的参数传回
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_setcallback_robot_event(
    RSHD rshd, const RobotEventCallback ptr, void *arg);

/**
 * @brief 注册用于获取函数的日志信息的回调函数
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_setcallback_robot_loginfo(
    RSHD rshd, const RobotLogPrintCallback ptr, void *arg);

/**
 * @brief 注册用于获取MOVEP运动进度信息的回调函数
 * @return
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_setcallback_movep_step_num(
    RSHD rshd, const RealTimeMovepStepNumNotifyCallback ptr, void *arg);

// enable push information
/**
 * @brief 设置是否允许实时路点信息推送
 * @param rshd 械臂控制上下文句柄
 * @param enable true表示允许 false表示不允许
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_enable_push_realtime_roadpoint(RSHD rshd,
                                                                   bool enable);

/**
 * @brief 设置是否允许实时关节状态推送
 * @param rshd 械臂控制上下文句柄
 * @param enable true表示允许 false表示不允许
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_enable_push_realtime_joint_status(
    RSHD rshd, bool enable);

/**
 * @brief 设置是否允许实时末端速度推送
 * @param rshd 械臂控制上下文句柄
 * @param enable true表示允许 false表示不允许
 * @return RS_SUCC 成功 其他失败
 */
SERVICE_INTERFACE_ABI_EXPORT int rs_enable_push_realtime_end_speed(RSHD rshd,
                                                                   bool enable);

// other
SERVICE_INTERFACE_ABI_EXPORT const char *rs_str_error(RSHD rshd, int err);
SERVICE_INTERFACE_ABI_EXPORT void print_plan(const CoordCalibrate *user_coord);
SERVICE_INTERFACE_ABI_EXPORT void print_move_relative_offset(
    const MoveRelative *relative);
SERVICE_INTERFACE_ABI_EXPORT void print_waypoint(wayPoint_S *waypoint);
SERVICE_INTERFACE_ABI_EXPORT void print_tool_dynamics(
    ToolDynamicsParam *tool_dynamics);

#ifdef __cplusplus
}
#endif

#endif // RSDEF_H
