/** @file  type_def.h
 *  @brief 数据类型的定义
 */
#ifndef AUBO_SDK_TYPE_DEF_H
#define AUBO_SDK_TYPE_DEF_H

#include <stddef.h>
#include <array>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <string>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

namespace arcs {
namespace common_interface {

/// Cartesion degree of freedom, 6 for x,y,z,rx,ry,rz
#define CARTESIAN_DOF           6
#define SAFETY_PARAM_SELECT_NUM 2  /// normal + reduced
#define SAFETY_PLANES_NUM       8  /// 安全平面的数量
#define SAFETY_CUBIC_NUM        10 /// 安全立方体的数量
#define TOOL_CONFIGURATION_NUM  3  /// 工具配置数量

using Vector3d = std::array<double, 3>;
using Vector4d = std::array<double, 4>;
using Vector3f = std::array<float, 3>;
using Vector4f = std::array<float, 4>;
using Vector6f = std::array<float, 6>;

struct RobotSafetyParameterRange
{
    RobotSafetyParameterRange()
    {
        for (int i = 0; i < SAFETY_PARAM_SELECT_NUM; i++) {
            params[i].power = 0.;
            params[i].momentum = 0.;
            params[i].stop_time = 0.;
            params[i].stop_distance = 0.;
            params[i].reduced_entry_time = 0.;
            params[i].reduced_entry_distance = 0.;
            params[i].tcp_speed = 0.;
            params[i].elbow_speed = 0.;
            params[i].tcp_force = 0.;
            params[i].elbow_force = 0.;
            std::fill(params[i].qmin.begin(), params[i].qmin.end(), 0.);
            std::fill(params[i].qmax.begin(), params[i].qmax.end(), 0.);
            std::fill(params[i].qdmax.begin(), params[i].qdmax.end(), 0.);
            std::fill(params[i].joint_torque.begin(),
                      params[i].joint_torque.end(), 0.);
            params[i].tool_orientation.fill(0.);
            params[i].tool_deviation = 0.;
            for (int j = 0; j < SAFETY_PLANES_NUM; j++) {
                params[i].planes[j].fill(0.);
                params[i].restrict_elbow[j] = 0;
            }
        }
        for (int i = 0; i < SAFETY_PLANES_NUM; i++) {
            trigger_planes[i].plane.fill(0.);
            trigger_planes[i].restrict_elbow = 0;
        }
        for (int i = 0; i < SAFETY_CUBIC_NUM; i++) {
            cubic[i].orig.fill(0.);
            cubic[i].size.fill(0.);
            cubic[i].restrict_elbow = 0;
        }
        for (int i = 0; i < TOOL_CONFIGURATION_NUM; i++) {
            tools[i].fill(0.);
        }

        tool_inclination = 0.;
        tool_azimuth = 0.;
        std::fill(safety_home.begin(), safety_home.end(), 0.);

        safety_input_emergency_stop = 0;
        safety_input_safeguard_stop = 0;
        safety_input_safeguard_reset = 0;
        safety_input_auto_safeguard_stop = 0;
        safety_input_auto_safeguard_reset = 0;
        safety_input_three_position_switch = 0;
        safety_input_operational_mode = 0;
        safety_input_reduced_mode = 0;
        safety_input_handguide = 0;

        safety_output_emergency_stop = 0;
        safety_output_not_emergency_stop = 0;
        safety_output_robot_moving = 0;
        safety_output_robot_steady = 0;
        safety_output_reduced_mode = 0;
        safety_output_not_reduced_mode = 0;
        safety_output_safe_home = 0;
        safety_output_robot_not_stopping = 0;
        safety_output_safetyguard_stop = 0;

        tp_3pe_for_handguide = 1;
        allow_manual_high_speed = 0;
    }

    uint32_t crc32{ 0 };

    /// 最多可以保存2套参数, 默认使用第 0 套参数
    struct
    {
        float power;     ///< sum of joint torques times joint angular speeds
        float momentum;  ///< 机器人动量限制
        float stop_time; ///< 停机时间 ms
        float stop_distance;      ///< 停机距离 m
        float reduced_entry_time; ///< 进入缩减模式的最大时间
        float
            reduced_entry_distance; ///< 进入缩减模式的最大距离(可由安全平面触发)
        float tcp_speed;
        float elbow_speed;
        float tcp_force;
        float elbow_force;
        std::vector<float> qmin;
        std::vector<float> qmax;
        std::vector<float> qdmax;
        std::vector<float> joint_torque;
        Vector3f tool_orientation; ///<
        float tool_deviation;
        Vector4f planes[SAFETY_PLANES_NUM]; /// x,y,z,displacement
        int restrict_elbow[SAFETY_PLANES_NUM];
    } params[SAFETY_PARAM_SELECT_NUM];

    /// 8个触发平面
    struct
    {
        Vector4f plane; /// x,y,z,displacement
        int restrict_elbow;
    } trigger_planes[SAFETY_PLANES_NUM];

    struct
    {
        Vector6f orig; ///< 立方块的原点 (x,y,z,rx,ry,rz)
        Vector3f size; ///< 立方块的尺寸 (x,y,z)
        int restrict_elbow;
    } cubic[SAFETY_CUBIC_NUM]; ///< 10个安全空间

    /// 3个工具
    Vector4f tools[TOOL_CONFIGURATION_NUM]; /// x,y,z,radius

    float tool_inclination{ 0. }; ///< 倾角
    float tool_azimuth{ 0. };     ///< 方位角
    std::vector<float> safety_home;

    /// 可配置IO的输入输出安全功能配置
    uint32_t safety_input_emergency_stop;
    uint32_t safety_input_safeguard_stop;
    uint32_t safety_input_safeguard_reset;
    uint32_t safety_input_auto_safeguard_stop;
    uint32_t safety_input_auto_safeguard_reset;
    uint32_t safety_input_three_position_switch;
    uint32_t safety_input_operational_mode;
    uint32_t safety_input_reduced_mode;
    uint32_t safety_input_handguide;

    uint32_t safety_output_emergency_stop;
    uint32_t safety_output_not_emergency_stop;
    uint32_t safety_output_robot_moving;
    uint32_t safety_output_robot_steady;
    uint32_t safety_output_reduced_mode;
    uint32_t safety_output_not_reduced_mode;
    uint32_t safety_output_safe_home;
    uint32_t safety_output_robot_not_stopping;
    uint32_t safety_output_safetyguard_stop;

    int tp_3pe_for_handguide; ///< 是否将示教器三档位开关作为拖动功能开关
    int allow_manual_high_speed; ///< 手动模式下允许高速运行
};

inline std::ostream &operator<<(std::ostream &os,
                                const RobotSafetyParameterRange &vd)
{
    // os << (int)vd;
    return os;
}

/// 接口函数返回值定义
///
/// 整数为警告，负数为错误，0为没有错误也没有警告
#define ENUM_AuboErrorCodes_DECLARES                                           \
    ENUM_ITEM(AUBO_OK, 0, "Success")                                           \
    ENUM_ITEM(AUBO_BAD_STATE, 1, "State error")                                \
    ENUM_ITEM(AUBO_QUEUE_FULL, 2, "Planning queue full")                       \
    ENUM_ITEM(AUBO_BUSY, 3, "The previous command is executing")               \
    ENUM_ITEM(AUBO_TIMEOUT, 4, "Timeout")                                      \
    ENUM_ITEM(AUBO_INVL_ARGUMENT, 5, "Invalid parameters")                     \
    ENUM_ITEM(AUBO_NOT_IMPLETEMENT, 6, "Interface not implemented")            \
    ENUM_ITEM(AUBO_NO_ACCESS, 7, "Cannot access")                              \
    ENUM_ITEM(AUBO_CONN_REFUSED, 8, "Connection refused")                      \
    ENUM_ITEM(AUBO_CONN_RESET, 9, "Connection is reset")                       \
    ENUM_ITEM(AUBO_INPROGRESS, 10, "Execution in progress")                    \
    ENUM_ITEM(AUBO_EIO, 11, "Input/Output error")                              \
    ENUM_ITEM(AUBO_NOBUFFS, 12, "")                                            \
    ENUM_ITEM(AUBO_REQUEST_IGNORE, 13, "Request was ignored")                  \
    ENUM_ITEM(AUBO_ALGORITHM_PLAN_FAILED, 14,                                  \
              "Motion planning algorithm error")                               \
    ENUM_ITEM(AUBO_VERSION_INCOMPAT, 15, "Interface version unmatch")          \
    ENUM_ITEM(AUBO_DIMENSION_ERR, 16,                                          \
              "Input parameter dimension is incorrect")                        \
    ENUM_ITEM(AUBO_SINGULAR_ERR, 17, "Input configuration may be singular")    \
    ENUM_ITEM(AUBO_POS_BOUND_ERR, 18,                                          \
              "Input position boundary exceeds the limit range")               \
    ENUM_ITEM(AUBO_INIT_POS_ERR, 19, "Initial position input is unreasonable") \
    ENUM_ITEM(AUBO_ELP_SETTING_ERR, 20, "Envelope body setting error")         \
    ENUM_ITEM(AUBO_TRAJ_GEN_FAIL, 21, "Trajectory generation failed")          \
    ENUM_ITEM(AUBO_TRAJ_SELF_COLLISION, 22, "Trajectory self collision")       \
    ENUM_ITEM(                                                                 \
        AUBO_IK_NO_CONVERGE, 23,                                               \
        "Inverse kinematics computation did not converge; computation failed") \
    ENUM_ITEM(AUBO_IK_OUT_OF_RANGE, 24,                                        \
              "Inverse kinematics result out of robot range")                  \
    ENUM_ITEM(AUBO_IK_CONFIG_DISMATCH, 25,                                     \
              "Inverse kinematics input configuration contains errors")        \
    ENUM_ITEM(AUBO_IK_JACOBIAN_FAILED, 26,                                     \
              "The calculation of the inverse Jacobian matrix failed")         \
    ENUM_ITEM(AUBO_IK_NO_SOLU, 27,                                             \
              "The target point has solutions, but it has exceeded the joint " \
              "limit conditions")                                              \
    ENUM_ITEM(AUBO_IK_UNKOWN_ERROR, 28, "Inverse kinematics unkown error")     \
    ENUM_ITEM(AUBO_ERR_UNKOWN, 99999, "Unkown error occurred.")

// clang-format off
/**
 * The RuntimeState enum
 *
 */
#define ENUM_RuntimeState_DECLARES                           \
    ENUM_ITEM(Running, 0, "正在运行中")                        \
    ENUM_ITEM(Retracting, 1, "倒退")                          \
    ENUM_ITEM(Pausing, 2, "暂停中")                           \
    ENUM_ITEM(Paused, 3, "暂停状态")                          \
    ENUM_ITEM(Stepping, 4, "单步执行中")                       \
    ENUM_ITEM(Stopping, 5, "受控停止中(保持原有轨迹)")           \
    ENUM_ITEM(Stopped, 6, "已停止")                           \
    ENUM_ITEM(Aborting, 7, "停止(最大速度关节运动停机)")

/**
 * @brief The RobotModeType enum
 *
 * 硬件强相关
 */
#define ENUM_RobotModeType_DECLARES                                                                     \
    ENUM_ITEM(NoController, -1,          "提供给示教器使用的, 如果aubo_control进程崩溃则会显示为NoController") \
    ENUM_ITEM(Disconnected, 0,           "没有连接到机械臂本体(控制器与接口板断开连接或是 EtherCAT 等总线断开)")  \
    ENUM_ITEM(ConfirmSafety, 1,          "正在进行安全配置, 断电状态下进行")                                  \
    ENUM_ITEM(Booting, 2,                "机械臂本体正在上电初始化")                                         \
    ENUM_ITEM(PowerOff, 3,               "机械臂本体处于断电状态")                                           \
    ENUM_ITEM(PowerOn, 4,                "机械臂本体上电成功, 刹车暂未松开(抱死), 关节初始状态未获取")            \
    ENUM_ITEM(Idle, 5,                   "机械臂上电成功, 刹车暂未松开(抱死), 电机不通电, 关节初始状态获取完成")    \
    ENUM_ITEM(BrakeReleasing, 6,         "机械臂上电成功, 刹车正在松开")                                      \
    ENUM_ITEM(BackDrive, 7,              "反向驱动：刹车松开, 电机不通电")                                    \
    ENUM_ITEM(Running, 8,                "机械臂刹车松开, 运行模式, 控制权由硬件移交给软件")                     \
    ENUM_ITEM(Maintaince, 9,             "维护模式: 包括固件升级、参数写入等")                                 \
    ENUM_ITEM(Error, 10,                 "")                                                              \
    ENUM_ITEM(PowerOffing, 11,           "机械臂本体处于断电过程中")

#define ENUM_SafetyModeType_DECLARES                           \
    ENUM_ITEM(Undefined, 0,          "安全状态待定")             \
    ENUM_ITEM(Normal, 1,             "正常运行模式")             \
    ENUM_ITEM(ReducedMode, 2,        "缩减运行模式")             \
    ENUM_ITEM(Recovery, 3,           "启动时如果在安全限制之外, 机器人将进入recovery模式") \
    ENUM_ITEM(Violation, 4,          "超出安全限制（根据安全配置, 例如速度超限等）") \
    ENUM_ITEM(ProtectiveStop, 5,     "软件触发的停机（保持轨迹, 不抱闸, 不断电）") \
    ENUM_ITEM(SafeguardStop, 6,      "IO触发的防护停机（不保持轨迹, 抱闸, 不断电）") \
    ENUM_ITEM(SystemEmergencyStop,7, "系统急停：急停信号由外部输入(可配置输入), 不对外输出急停信号") \
    ENUM_ITEM(RobotEmergencyStop, 8, "机器人急停：控制柜急停输入或者示教器急停按键触发, 对外输出急停信号") \
    ENUM_ITEM(Fault, 9,              "机械臂硬件故障或者系统故障")
    //ValidateJointId

/**
 * 根据ISO 10218-1:2011(E) 5.7节
 * Automatic: In automatic mode, the robot shall execute the task programme and
 * the safeguarding measures shall be functioning. Automatic operation shall be
 * prevented if any stop condition is detected. Switching from this mode shall
 * result in a stop.
 */
#define ENUM_OperationalModeType_DECLARES                                                \
    ENUM_ITEM(Disabled, 0, "禁用模式: 不使用Operational Mode")                              \
    ENUM_ITEM(Automatic, 1, "自动模式: 机器人正常工作模式, 运行速度不会被限制")                  \
    ENUM_ITEM(Manual, 2, "手动模式: 机器人编程示教模式(T1), 机器人运行速度将会被限制或者机器人程序校验模式(T2)")

/**
 * 机器人的控制模式, 最终的控制对象
 */
#define ENUM_RobotControlModeType_DECLARES                 \
    ENUM_ITEM(Unknown, 0,   "未知的控制模式")                \
    ENUM_ITEM(Position, 1,  "位置控制  movej")              \
    ENUM_ITEM(Speed, 2,     "速度控制  speedj/speedl")      \
    ENUM_ITEM(Servo, 3,     "位置控制  servoj")             \
    ENUM_ITEM(Freedrive, 4, "拖动示教  freedrive_mode")     \
    ENUM_ITEM(Force, 5,     "末端力控  force_mode")         \
    ENUM_ITEM(Torque, 6,    "关节力矩控制")                  \
    ENUM_ITEM(Collision, 7,    "碰撞模式")

#define ENUM_JointServoModeType_DECLARES                  \
    ENUM_ITEM(Unknown, -1, "未知")                         \
    ENUM_ITEM(Open, 0, "开环模式")                          \
    ENUM_ITEM(Current, 1, "电流伺服模式")                    \
    ENUM_ITEM(Velocity, 2, "速度伺服模式")                   \
    ENUM_ITEM(Position, 3, "位置伺服模式")                   \
    ENUM_ITEM(Torque, 4, "力矩伺服模式")

#define ENUM_JointStateType_DECLARES                             \
    ENUM_ITEM(Poweroff, 0, "节点未连接到接口板或者已经断电")          \
    ENUM_ITEM(Idle, 2,        "节点空闲")                         \
    ENUM_ITEM(Fault, 3,       "节点错误, 节点停止伺服运动, 刹车抱死") \
    ENUM_ITEM(Running, 4,     "节点伺服")                         \
    ENUM_ITEM(Bootload, 5,     "节点bootloader状态, 暂停一切通讯")

#define ENUM_StandardInputAction_DECLARES                        \
    ENUM_ITEM(Default, 0,  "无触发")                              \
    ENUM_ITEM(Handguide, 1, "拖动示教，高电平触发")                 \
    ENUM_ITEM(GoHome, 2, "运动到工程初始位姿，高电平触发")            \
    ENUM_ITEM(StartProgram, 3, "开始工程，上升沿触发")              \
    ENUM_ITEM(StopProgram, 4, "停止工程，上升沿触发")               \
    ENUM_ITEM(PauseProgram, 5, "暂停工程，上升沿触发")              \
    ENUM_ITEM(PopupDismiss, 6, "消除弹窗，上升沿触发")              \
    ENUM_ITEM(PowerOn, 7, "机器人上电/松刹车，上升沿触发")           \
    ENUM_ITEM(PowerOff, 8, "机器人抱死刹车/断电，上升沿触发")         \
    ENUM_ITEM(ResumeProgram, 9, "恢复工程，上升沿触发")             \
    ENUM_ITEM(SlowDown1, 10, "机器人减速触发1，高电平触发")          \
    ENUM_ITEM(SlowDown2, 11, "机器人减速触发2，高电平触发")          \
    ENUM_ITEM(SafeStop, 12, "安全停止，高电平触发")                 \
    ENUM_ITEM(RunningGuard, 13, "信号，高电平有效")                \
    ENUM_ITEM(MoveToFirstPoint, 14, "运动到工程初始位姿，高电平触发") \
    ENUM_ITEM(xSlowDown1, 15, "机器人减速触发1，低电平触发")         \
    ENUM_ITEM(xSlowDown2, 16, "机器人减速触发2，低电平触发")

#define ENUM_StandardOutputRunState_DECLARES             \
    ENUM_ITEM(None, 0,     "标准输出状态未定义")            \
    ENUM_ITEM(StopLow, 1, "低电平指示工程停止")             \
    ENUM_ITEM(StopHigh, 2, "高电平指示机器人停止")          \
    ENUM_ITEM(RunningHigh, 3,  "指示工程正在运行")         \
    ENUM_ITEM(PausedHigh, 4,  "指示工程已经暂停")          \
    ENUM_ITEM(AtHome, 5, "高电平指示机器人正在拖动")         \
    ENUM_ITEM(Handguiding, 6, "高电平指示机器人正在拖动")    \
    ENUM_ITEM(PowerOn, 7, "高电平指示机器人已经上电")           \
    ENUM_ITEM(RobotEmergencyStop, 8, "高电平指示机器人急停按下") \
    ENUM_ITEM(SystemEmergencyStop, 9, "高电平指示外部输入系统急停按下") \
    ENUM_ITEM(InternalEmergencyStop, 8, "高电平指示机器人急停按下") \
    ENUM_ITEM(ExternalEmergencyStop, 9, "高电平指示外部输入系统急停按下") \
    ENUM_ITEM(SystemError, 10, "系统错误，包括故障、超限、急停、安全停止、防护停止 ") \
    ENUM_ITEM(NotSystemError, 11, "无系统错误，包括普通模式、缩减模式和恢复模式 ") \
    ENUM_ITEM(RobotOperable, 12, "机器人可操作，机器人上电且松刹车了 ")

#define ENUM_SafetyInputAction_DECLARES                        \
    ENUM_ITEM(Unassigned, 0, "安全输入未分配动作")                \
    ENUM_ITEM(EmergencyStop, 1, "安全输入触发急停")               \
    ENUM_ITEM(SafeguardStop, 2, "安全输入触发防护停止, 边沿触发")   \
    ENUM_ITEM(SafeguardReset, 3, "安全输入触发防护重置, 边沿触发")  \
    ENUM_ITEM(ThreePositionSwitch, 4, "3档位使能开关")           \
    ENUM_ITEM(OperationalMode, 5, "切换自动模式和手动模式")        \
    ENUM_ITEM(HandGuide, 6, "拖动示教")              \
    ENUM_ITEM(ReducedMode, 7, "安全参数切换1(缩减模式)，序号越低优先级越高，三路输出都无效时，选用第0组安全参数")         \
    ENUM_ITEM(AutomaticModeSafeguardStop, 8, "自动模式下防护停机输入(需要配置三档位使能设备)") \
    ENUM_ITEM(AutomaticModeSafeguardReset, 9, "自动模式下上升沿触发防护重置(需要配置三档位使能设备)")

#define ENUM_SafetyOutputRunState_DECLARES                         \
    ENUM_ITEM(Unassigned, 0, "安全输出未定义")                       \
    ENUM_ITEM(SystemEmergencyStop, 1, "输出高当有机器人急停输入或者急停按键被按下")         \
    ENUM_ITEM(NotSystemEmergencyStop, 2, "输出低当有机器人急停输入或者急停按键被按下")      \
    ENUM_ITEM(RobotMoving, 3, "输出高当有关节运动速度超过 0.1rad/s")                     \
    ENUM_ITEM(RobotNotMoving, 4, "输出高当所有的关节运动速度不超过 0.1rad/s")             \
    ENUM_ITEM(ReducedMode, 5, "输出高当机器人处于缩减模式")                       \
    ENUM_ITEM(NotReducedMode, 6, "输出高当机器人不处于缩减模式")                   \
    ENUM_ITEM(SafeHome, 7, "输出高当机器人已经处于安全Home位姿")                    \
    ENUM_ITEM(RobotNotStopping, 8, "输出低当机器人正在急停或者安全停止中")

#define ENUM_PayloadIdentifyMoveAxis_DECLARES      \
    ENUM_ITEM(Joint_2_6, 0,"第2和6关节运动")         \
    ENUM_ITEM(Joint_3_6, 1,"第3和6关节运动")         \
    ENUM_ITEM(Joint_4_6, 2,"第4和6关节运动")         \
    ENUM_ITEM(Joint_4_5_6, 3,"第4、5、6关节运动")    \

#define ENUM_EnvelopingShape_DECLARES \
    ENUM_ITEM(Cube, 1,"立方体") \
    ENUM_ITEM(Column, 2,"柱状体") \
    ENUM_ITEM(Stl, 3,"以STL文件的形式描述负载碰撞集合体")

#define ENUM_TaskFrameType_DECLARES                           \
    ENUM_ITEM(NONE, 0,"")        \
    ENUM_ITEM(POINT_FORCE, 1, "力控坐标系发生变换, 使得力控参考坐标系的y轴沿着机器人TCP指向力控所选特征的原点, x和z轴取决于所选特征的原始方向" \
                              "力控坐标系发生变换, 使得力控参考坐标系的y轴沿着机器人TCP指向力控所选特征的原点, x和z轴取决于所选特征的原始方向" \
                              "机器人TCP与所选特征的起点之间的距离至少为10mm" \
                              "优先选择X轴, 为所选特征的X轴在力控坐标系Y轴垂直平面上的投影, 如果所选特征的X轴与力控坐标系的Y轴平行, " \
                              "通过类似方法确定力控坐标系Z轴, Y-X或者Y-Z轴确定之后, 通过右手法则确定剩下的轴") \
    ENUM_ITEM(FRAME_FORCE, 2,"力控坐标系不发生变换 SIMPLE_FORC") \
    ENUM_ITEM(MOTION_FORCE, 3,"力控坐标系发生变换, 使得力控参考坐标系的x轴为机器人TCP速度在所选特征x-y平面上的投影y轴将垂直于机械臂运动, 并在所选特征的x-y平面内")\
    ENUM_ITEM(TOOL_FORCE, 4,"以工具末端坐标系作为力控参考坐标系")

#ifdef ERROR
#undef ERROR
#endif

#define ENUM_TraceLevel_DECLARES  \
    ENUM_ITEM(FATAL, 0, "") \
    ENUM_ITEM(ERROR, 1, "") \
    ENUM_ITEM(WARNING, 2, "") \
    ENUM_ITEM(INFO, 3, "") \
    ENUM_ITEM(DEBUG, 4, "")

#define ENUM_AxisModeType_DECLARES  \
    ENUM_ITEM(NoController, -1, "提供给示教器使用的, 如果aubo_control进程崩溃则会显示为NoController") \
    ENUM_ITEM(Disconnected, 0, "未连接") \
    ENUM_ITEM(PowerOff, 1, "断电") \
    ENUM_ITEM(BrakeReleasing, 2, "刹车松开中") \
    ENUM_ITEM(Idle, 3, "空闲") \
    ENUM_ITEM(Running, 4, "运行中") \
    ENUM_ITEM(Fault, 5, "错误状态")

#define ENUM_SafeguedStopType_DECLARES  \
    ENUM_ITEM(None, 0, "无安全停止") \
    ENUM_ITEM(SafeguedStopIOInput, 1, "安全停止(IO输入)") \
    ENUM_ITEM(SafeguedStop3PE, 2, "安全停止(三态开关)") \
    ENUM_ITEM(SafeguedStopOperational, 3, "安全停止(操作模式)")

#define ENUM_ITEM(c, n, ...) c = n,
enum AuboErrorCodes : int
{
    ENUM_AuboErrorCodes_DECLARES
};

enum class RuntimeState : int
{
    ENUM_RuntimeState_DECLARES
};

enum class RobotModeType : int
{
    ENUM_RobotModeType_DECLARES
};

enum class AxisModeType : int
{
    ENUM_AxisModeType_DECLARES
};

/**
 * 安全状态:
 *
 */
enum class SafetyModeType : int
{
    ENUM_SafetyModeType_DECLARES
};

/**
 * 操作模式
 */
enum class OperationalModeType : int
{
    ENUM_OperationalModeType_DECLARES
};

/**
 * 机器人控制模式
 */
enum class RobotControlModeType : int
{
    ENUM_RobotControlModeType_DECLARES
};

/**
 * 关节伺服模式
 */
enum class JointServoModeType : int
{
    ENUM_JointServoModeType_DECLARES
};

/**
 * 关节状态
 */
enum class JointStateType : int
{
    ENUM_JointStateType_DECLARES
};

/**
 * 标准输出运行状态
 */
enum class StandardOutputRunState : int
{
    ENUM_StandardOutputRunState_DECLARES
};

/**
 * @brief The StandardInputAction enum
 */
enum class StandardInputAction : int
{
    ENUM_StandardInputAction_DECLARES
};

enum class SafetyInputAction : int
{
    ENUM_SafetyInputAction_DECLARES
};

enum class SafetyOutputRunState : int
{
    ENUM_SafetyOutputRunState_DECLARES
};

enum TaskFrameType
{
    ENUM_TaskFrameType_DECLARES
};

enum EnvelopingShape : int
{
    ENUM_EnvelopingShape_DECLARES
};

enum PayloadIdentifyMoveAxis : int
{
    ENUM_PayloadIdentifyMoveAxis_DECLARES
};

enum TraceLevel
{
    ENUM_TraceLevel_DECLARES
};

enum SafeguedStopType : int
{
    ENUM_SafeguedStopType_DECLARES
};
#undef ENUM_ITEM

// clang-format on

#define DECL_TO_STRING_FUNC(ENUM)                             \
    inline std::string toString(ENUM v)                       \
    {                                                         \
        using T = ENUM;                                       \
        std::string name = #ENUM ".";                         \
        ENUM_##ENUM##_DECLARES                                \
                                                              \
            return #ENUM ".Unkown";                           \
    }                                                         \
    inline std::ostream &operator<<(std::ostream &os, ENUM v) \
    {                                                         \
        os << toString(v);                                    \
        return os;                                            \
    }

#define ENUM_ITEM(c, n, ...) \
    if (v == T::c) {         \
        return name + #c;    \
    }

DECL_TO_STRING_FUNC(RuntimeState)
DECL_TO_STRING_FUNC(RobotModeType)
DECL_TO_STRING_FUNC(AxisModeType)
DECL_TO_STRING_FUNC(SafetyModeType)
DECL_TO_STRING_FUNC(OperationalModeType)
DECL_TO_STRING_FUNC(RobotControlModeType)
DECL_TO_STRING_FUNC(JointServoModeType)
DECL_TO_STRING_FUNC(JointStateType)
DECL_TO_STRING_FUNC(StandardInputAction)
DECL_TO_STRING_FUNC(StandardOutputRunState)
DECL_TO_STRING_FUNC(SafetyInputAction)
DECL_TO_STRING_FUNC(SafetyOutputRunState)
DECL_TO_STRING_FUNC(TaskFrameType)
DECL_TO_STRING_FUNC(TraceLevel)

#undef ENUM_ITEM

enum class ForceControlState
{
    Stopped,
    Starting,
    Stropping,
    Running
};

enum class RefFrameType
{
    None, ///
    Tool, ///< 工具坐标系
    Path, ///< 轨迹坐标系
    Base  ///< 基坐标系
};

/// 圆周运动参数定义
struct CircleParameters
{
    std::vector<double> pose_via; ///< 圆周运动途中点的位姿
    std::vector<double> pose_to;  ///< 圆周运动结束点的位姿
    double a;                     ///< 加速度, 单位: m/s^2
    double v;                     ///< 速度，单位: m/s
    double blend_radius;          ///< 交融半径,单位: m
    double duration;              ///< 运行时间，单位: s
    double helix;
    double spiral;
    double direction;
    int loop_times; ///< 暂不支持
};

inline std::ostream &operator<<(std::ostream &os, CircleParameters p)
{
    return os;
}

struct SpiralParameters
{
    std::vector<double> frame; ///< 参考点，螺旋线的中心点和参考坐标系
    int plane;                 ///< 参考平面选择 0-XY 1-YZ 2-ZX
    double angle; ///< 转动的角度，如果为正数，机器人逆时针旋转
    double spiral; ///< 正数外扩
    double helix;  ///< 正数上升
};

inline std::ostream &operator<<(std::ostream &os, SpiralParameters p)
{
    return os;
}

struct Enveloping
{
    EnvelopingShape shape; // 包络体形状
    std::vector<double>
        ep_args; // 包络体组合，shape为None或Stl时无需对ep_args赋值;
                 // shape为Cube时ep_args有9个元素，分别为xmin,xmax,ymin,ymax,zmin,zmax,rx,ry,rz;
                 // shape为Column时ep_args有5个元素，分别为radius,height,rx,ry,rz;
    std::string stl_path; // stl的路径(绝对路径)，stl文件需为二进制文件,
                          // shape设置为Stl时，此项生效
};

inline std::ostream &operator<<(std::ostream &os, Enveloping p)
{
    return os;
}

/// 用于负载辨识的轨迹配置
struct TrajConfig
{
    std::vector<Enveloping> envelopings;   // 包络体组合
    PayloadIdentifyMoveAxis move_axis;     // 运动的轴(ID), 下标从0开始
    std::vector<double> init_joint;        // 关节初始位置
    std::vector<double> upper_joint_bound; // 运动轴上限
    std::vector<double> lower_joint_bound; // 运动轴下限
    std::vector<double> max_velocity; // 关节运动的最大速度，默认值为　3.0
    std::vector<double> max_acceleration; // 关节运动的最大加速度，默认值为　5.0
};

inline std::ostream &operator<<(std::ostream &os, TrajConfig p)
{
    return os;
}

// result with error code
using ResultWithErrno = std::tuple<std::vector<double>, int>;
using ResultWithErrno1 = std::tuple<std::vector<std::vector<double>>, int>;

// mass, cog, aom, inertia
using Payload = std::tuple<double, std::vector<double>, std::vector<double>,
                           std::vector<double>>;

// force_offset, com, mass, angle
using ForceSensorCalibResult =
    std::tuple<std::vector<double>, std::vector<double>, double,
               std::vector<double>>;

// force_offset, com, mass, angle error
using ForceSensorCalibResultWithError =
    std::tuple<std::vector<double>, std::vector<double>, double,
               std::vector<double>, double>;

// 动力学模型m,d,k
using DynamicsModel =
    std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>;

// double xmin;
// double xmax;
// double ymin;
// double ymax;
// double zmin;
// double zmax;
using Box = std::vector<double>;

// double xcbottom;
// double ycbottom;
// double zcbottom;
// double height;
// double radius;
using Cylinder = std::vector<double>;

// double xc;
// double yc;
// double radius;
using Sphere = std::vector<double>;

struct RobotMsg
{
    uint64_t timestamp; ///< 时间戳，即系统时间
    TraceLevel level;   ///< 日志等级
    int code;           ///< 错误码
    std::string source; ///< 发送消息的机器人别名 alias
                        ///< 可在 /root/arcs_ws/config/aubo_control.conf
                        ///< 配置文件中查到机器人的alias
    std::vector<std::string> args; ///< 机器人参数
};
using RobotMsgVector = std::vector<RobotMsg>;

/// RTDE菜单
struct RtdeRecipe
{
    bool to_server;   ///< 输入/输出
    int chanel;       ///< 通道
    double frequency; ///< 更新频率
    int trigger; ///< 触发方式(该功能暂未实现): 0 - 周期; 1 - 变化
    std::vector<std::string> segments; ///< 字段列表
};

/// 异常类型
enum error_type
{
    parse_error = -32700,      ///< 解析错误
    invalid_request = -32600,  ///< 无效请求
    method_not_found = -32601, ///< 方法未找到
    invalid_params = -32602,   ///< 无效参数
    internal_error = -32603,   ///< 内部错误
    server_error,              ///< 服务器错误
    invalid                    ///< 无效
};

/// 异常码
enum ExceptionCode
{
    EC_DISCONNECTED = -1,      ///< 断开连接
    EC_NOT_LOGINED = -2,       ///< 未登录
    EC_INVAL_SOCKET = -3,      ///< 无效套接字
    EC_REQUEST_BUSY = -4,      ///< 请求繁忙
    EC_SEND_FAILED = -5,       ///< 发送失败
    EC_RECV_TIMEOUT = -6,      ///< 接收超时
    EC_RECV_ERROR = -7,        ///< 接收错误
    EC_PARSE_ERROR = -8,       ///< 解析错误
    EC_INVALID_REQUEST = -9,   ///< 无效请求
    EC_METHOD_NOT_FOUND = -10, ///< 方法未找到
    EC_INVALID_PARAMS = -11,   ///< 无效参数
    EC_INTERNAL_ERROR = -12,   ///< 内部错误
    EC_SERVER_ERROR = -13,     ///< 服务器错误
    EC_INVALID = -14           ///< 无效
};

/// 自定义异常类 AuboException
class AuboException : public std::exception
{
public:
    AuboException(int code, const std::string &prefix,
                  const std::string &message) noexcept
        : code_(code), message_(prefix + "-" + message)
    {
    }

    AuboException(int code, const std::string &message) noexcept
        : code_(code), message_(message)
    {
    }

    error_type type() const
    {
        if (code_ >= -32603 && code_ <= -32600) {
            return static_cast<error_type>(code_);
        } else if (code_ >= -32099 && code_ <= -32000) {
            return server_error;
        } else if (code_ == -32700) {
            return parse_error;
        }
        return invalid;
    }

    int code() const { return code_; }
    const char *what() const noexcept override { return message_.c_str(); }

private:
    int code_;            ///< 异常码
    std::string message_; ///< 异常消息
};

inline const char *returnValue2Str(int retval)
{
    static const char *retval_str[] = {
#define ENUM_ITEM(n, v, s) s,
        ENUM_AuboErrorCodes_DECLARES
#undef ENUM_ITEM
    };

    enum arcs_index
    {
#define ENUM_ITEM(n, v, s) n##_INDEX,
        ENUM_AuboErrorCodes_DECLARES
#undef ENUM_ITEM
    };

    int index = -1;

#define ENUM_ITEM(n, v, s) \
    if (retval == v)       \
        index = n##_INDEX;
    ENUM_AuboErrorCodes_DECLARES
#undef ENUM_ITEM

        if (index == -1)
    {
        index = AUBO_ERR_UNKOWN;
    }

    return retval_str[(unsigned)index];
}

} // namespace common_interface
} // namespace arcs
#endif

#if defined ENABLE_JSON_TYPES
#include "bindings/jsonrpc/json_types.h"
#endif
