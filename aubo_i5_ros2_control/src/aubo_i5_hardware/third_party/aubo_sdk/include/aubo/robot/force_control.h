/** @file  force_control.h
 *  @brief 力控接口
 *
 * 力控的限制
 * When the robot is force controlled, the following functionality is not
 * accessible:
 *
 * • Collision Detection (option 613-1) \n
 * • SoftMove (option 885-1) \n
 * • Tracking functionality like Conveyor Tracking (option 606-1), Optical
 *   Tracking (6601) and Weld Guide (815-2) \n
 * • Sensor Synchronization or Analog Synchronization \n
 * • World Zones (option 608-1) \n
 * • Independent Axes (option 610-1) \n
 * • Path Offset (option 612-1) \n
 * • Arc options \n
 * • PickMaster options \n
 * • Joint soft servo (instruction SoftAct) \n
 * • Force Control cannot be activated when the robot is running in MultiMove
 *   Coordinated mode (option 604-1). \n
 * • If Force Control is used together with SafeMove (option 810-2) or
 *   Electronic Position Switches (option 810-1), the function Operational
 *   Safety Range must be used. See the respective manual for these options. \n
 * • RAPID instructions such as FCAct, FCDeact, FCConditionWaitWhile and
 *   FCRefStop can only be called from normal level in a motion task.
 *
 * 应用：抛光、打磨、清洁 \n
 * FC Pressure \n
 * 设置轨迹坐标系的z方向为力控轴，spring设置为0 \n
 * 在还没接触前设置输出力为0，spring设置为固定值(根据vel确定) \n
 * 离开接触面：设置输出力为0，spring设置为固定值 \n
 *
 * 活塞(Piston)装配 \n
 * Forward clutch hub \n
 * 设置力控终止模式
 *
 * 基于末端力传感器的拖动示教 \n
 * spring = 0; force_ref = 0; 参考轨迹点任意
 */
#ifndef AUBO_SDK_FORCE_CONTROL_INTERFACE_H
#define AUBO_SDK_FORCE_CONTROL_INTERFACE_H

#include <vector>
#include <thread>

#include <aubo/global_config.h>
#include <aubo/type_def.h>

namespace arcs {
namespace common_interface {

/**
 * 力控接口抽象类
 */
class ARCS_ABI_EXPORT ForceControl
{
public:
    ForceControl();
    virtual ~ForceControl();

    /**
     * Start force control
     *
     * fcEnable is used to enable Force Control. At the same time as Force
     * Control is enabled, fcEnable is used to define the coordinate system
     * for Force Control, and tune the force and torque damping. If a coordinate
     * system is not specified in fcEnable a default force control coordinate
     * system is created with the same orientation as the work object coordinate
     * system. All Force Control supervisions are activated by fcEnable.
     *
     * 使能力控。
     * fcEnable 被用于使能力控。 在力控被使能的同时，
     * fcEnable 用于定义力控的坐标系，并调整力和力矩的阻尼。
     * 如果在 fcEnable 中未指定坐标系，
     * 则会创建一个默认的力控制坐标系，其方向与工作对象坐标系相同。
     * 所有力控制监管功能都被 fcEnable 激活。
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * fcEnable(self: pyaubo_sdk.ForceControl) -> int
     *
     * @par Lua函数原型
     * fcEnable() -> nil
     *
     */
    int fcEnable();

    /**
     * End force control
     *
     * fcDisable is used to disable Force Control. After a successful
     * deactivation the robot is back in position control.
     *
     * 失能力控。
     * fcDisable 被用于失能力控。 在成功失能力控之后，机器人将回到位置控制模式。
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * fcDisable(self: pyaubo_sdk.ForceControl) -> int
     *
     * @par Lua函数原型
     * fcDisable() -> nil
     *
     */
    int fcDisable();

    /**
     * 判断力控是否被使能
     *
     * @return 使能返回true，失能返回false
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isFcEnabled(self: pyaubo_sdk.ForceControl) -> bool
     *
     * @par Lua函数原型
     * isFcEnabled() -> boolean
     *
     */
    bool isFcEnabled();

    /**
     * 设置力控参考(目标)值
     *
     * @param feature: 参考几何特征，用于生成力控参考坐标系
     * @param compliance: 柔性轴（方向）选择
     * @param wrench: 目标力/力矩
     * @param limits: 速度限制
     * @param type: 力控参考坐标系类型
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setTargetForce(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * List[bool], arg2: List[float], arg3: List[float], arg4:
     * arcs::common_interface::TaskFrameType) -> int
     *
     * @par Lua函数原型
     * setTargetForce(feature: table, compliance: table, wrench: table, limits:
     * table, type: number) -> nil
     *
     */
    int setTargetForce(const std::vector<double> &feature,
                       const std::vector<bool> &compliance,
                       const std::vector<double> &wrench,
                       const std::vector<double> &limits,
                       TaskFrameType type = TaskFrameType::FRAME_FORCE);

    /**
     * 设置力控动力学模型
     *
     * @param m
     * @param d
     * @param k
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setDynamicModel(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * List[float], arg2: List[float]) -> int
     *
     * @par Lua函数原型
     * setDynamicModel(m: table, d: table, k: table) -> nil
     *
     */
    int setDynamicModel(const std::vector<double> &m,
                        const std::vector<double> &d,
                        const std::vector<double> &k);

    /**
     * 获取力控动力学模型
     *
     * @return 力控动力学模型
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * getDynamicModel(self: pyaubo_sdk.ForceControl) -> Tuple[List[float],
     * List[float], List[float]]
     *
     * @par Lua函数原型
     * getDynamicModel() -> table
     *
     */
    DynamicsModel getDynamicModel();

    /**
     * 设置力控终止条件：力，当测量的力在设置的范围之内，力控算法将保持运行，直到设置的条件不满足，力控将退出
     *
     * FCCondForce is used to set up an end condition based on measured force.
     * The condition is lateractivated by calling the instruction
     * FCCondWaitWhile, which will wait and hold the program execution while the
     * specified condition is true. This allows the reference force, torque and
     * movement to continue until the force is outside the specified limits.
     *
     * A force condition is set up by defining minimum and maximum limits for
     * the force in the directions of the force control coordinate system. Once
     * activated with FCCondWaitWhile, the program execution will continue to
     * wait while the measured force is within its specified limits.
     *
     * It is possible to specify that the condition is fulfilled when the force
     * is outside the specified limits instead. This is done by using the switch
     * argument Outside. The condition on force is specified in the force
     * control coordinate system. This coordinate system is setup by the user in
     * the instruction FCAct.
     *
     * @param min 各方向最小的力/力矩
     * @param max 各方向最大的力/力矩
     * @param outside false 在设置条件的范围之内有效
     *          true  在设置条件的范围之外有效
     * @param timeout
     * 时间限制，单位s(秒)，从开始力控到达该时间时，不管是否满足力控终止条件，都会终止力控
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCondForce(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * List[float], arg2: bool, arg3: float) -> int
     *
     * @par Lua函数原型
     * setCondForce(min: table, max: table, outside: boolean, timeout: number)
     * -> nil
     *
     */
    int setCondForce(const std::vector<double> &min,
                     const std::vector<double> &max, bool outside,
                     double timeout);

    /**
     * FCCondOrient is used to set up an end condition for the tool orientation.
     * The condition is lateractivated by calling the instruction
     * FCCondWaitWhile, which will wait and hold the program execution while the
     * specified condition is true. This allows the reference force, torque and
     * movement to continue until the orientation is outside the specified
     * limits.
     *
     * An orientation condition is set up by defining a maximum angle and a
     * maximum rotation from a reference orientation. The reference orientation
     * is either defined by the current z direction of the tool, or by
     * specifying an orientation in relation to the z direction of the work
     * object.
     *
     * Once activated, the tool orientation must be within the limits (or
     * outside, if the argument Outside is used).
     *
     * @param frame
     * @param max_angle
     * @param max_rot
     * @param outside
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCondOrient(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * float, arg2: float, arg3: bool, arg4: float) -> int
     *
     * @par Lua函数原型
     * setCondOrient(frame: table, max_angle: number, max_rot: number, outside:
     * boolean, timeout: number) -> nil
     *
     */
    int setCondOrient(const std::vector<double> &frame, double max_angle,
                      double max_rot, bool outside, double timeout);

    /**
     * 指定力控有效平面，x-y平面，z方向有效
     *
     * FCCondPos is used to set up an end condition for the TCP position. The
     * condition is later activated by calling the instruction FCCondWaitWhile,
     * which will wait and hold the program execution while the specified
     * condition is true. This allows the reference force, torque and movement
     * to continue until the specified position is outside the specified limits.
     *
     * A position condition is set up by defining a volume in space for the TCP
     * position. Once activated the measured TCP position has to be within the
     * specified volume (or outside, if the argument Outside is used).
     *
     * @param plane={A,B,C,D}
     *        平面表示方法 Ax +By +Cz + D = 0
     *        其中,n = (A, B, C)是平面的法向量，
     *        D 是将平面平移到坐标原点所需距离（所以D=0时，平面过原点）
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCondPlane(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * float) -> int
     *
     * @par Lua函数原型
     * setCondPlane(plane: table, timeout: number) -> nil
     *
     */
    int setCondPlane(const std::vector<double> &plane, double timeout);

    /**
     * 指定力控有效圆柱体，提供中心轴和圆柱半径，可以指定圆柱内部还是外部
     *
     * @param axis
     * @param radius
     * @param outside
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCondCylinder(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * float, arg2: bool, arg3: float) -> int
     *
     * @par Lua函数原型
     * setCondCylinder(axis: table, radius: number, outside: boolean, timeout:
     * number) -> nil
     *
     */
    int setCondCylinder(const std::vector<double> &axis, double radius,
                        bool outside, double timeout);

    /**
     * 指定力控有效球体，提供球心和半径，可以指定球体内部还是外部
     *
     * @param center
     * @param radius
     * @param outside
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCondSphere(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * float, arg2: bool, arg3: float) -> int
     *
     * @par Lua函数原型
     * setCondSphere(center: table, radius: number, outside: boolean, timeout:
     * number) -> nil
     *
     */
    int setCondSphere(const std::vector<double> &center, double radius,
                      bool outside, double timeout);

    /**
     * FCCondTCPSpeed is used to setup an end condition for the TCP speed. The
     * condition is lateractivated by calling the instruction FCCondWaitWhile,
     * which will wait and hold the program execution while the specified
     * condition is true. This allows the reference force, torque and movement
     * to continue until the speed is outside the specified limits.
     *
     * A TCP speed condition is setup up by defining minimum and maximum limits
     * for the TCP speed in all directions of the work object. Once activated
     * with FCCondWaitWhile, the program execution will continue to wait while
     * the measured speed is within its specified limits.
     *
     * It is possible to specify that the condition is fulfilled when the speed
     * is outside the specified limits instead. This is the done by using the
     * switch argument Outside. The condition on TCP speed is specified in the
     * work object coordinate system.
     *
     * @param min
     * @param max
     * @param outside
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCondTcpSpeed(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * List[float], arg2: bool, arg3: float) -> int
     *
     * @par Lua函数原型
     * setCondTcpSpeed(min: table, max: table, outside: boolean, timeout:
     * number) -> nil
     *
     */
    int setCondTcpSpeed(const std::vector<double> &min,
                        const std::vector<double> &max, bool outside,
                        double timeout);

    /**
     * 力控终止条件-距离
     *
     * @param distance
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setCondDistance(double distance, double timeout);

    /**
     * 高级力控终止条件
     *
     * @param type
     * @param args
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int setCondAdvanced(const std::string &type,
                        const std::vector<double> &args, double timeout);

    /**
     * 激活力控终止条件
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setCondActive(self: pyaubo_sdk.ForceControl) -> int
     *
     * @par Lua函数原型
     * setCondActive() -> nil
     *
     */
    int setCondActive();

    /**
     * 力控终止条件是否已经满足
     *
     * @return
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * isCondFullfiled(self: pyaubo_sdk.ForceControl) -> bool
     *
     * @par Lua函数原型
     * isCondFullfiled() -> boolean
     *
     */
    bool isCondFullfiled();

    /**
     * FCSupvForce is used to set up force supervision in Force Control. The
     * supervision is activated when Force Control is activated with the
     * instruction FCAct.
     *
     * The force supervision is set up by defining minimum and maximum limits
     * for the force in the directions of the force control coordinate system.
     * Once activated, the supervision will stop the execution if the force is
     * outside the allowed values. The force supervision is specified in the
     * force control coordinate system. This coordinate system is setup by the
     * user with the instruction FCAct.
     *
     * @param min
     * @param max
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setSupvForce(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * List[float]) -> int
     *
     * @par Lua函数原型
     * setSupvForce(min: table, max: table) -> nil
     *
     */
    int setSupvForce(const std::vector<double> &min,
                     const std::vector<double> &max);

    /**
     * FCSupvOrient is used to set up an supervision for the tool orientation.
     * The supervision is activated when Force Control is activated with the
     * instruction FCAct.
     *
     * An orientation supervision is set up by defining a maximum angle and a
     * maximum rotation from a reference orientation. The reference orientation
     * is either defined by the current z direction of the tool, or by
     * specifying an orientation in relation to the z direction of the work
     * object.
     *
     * Once activated, the tool orientation must be within the limits otherwise
     * the supervision will stop the execution.
     *
     * @param frame
     * @param max_angle
     * @param max_rot
     * @param outside
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setSupvOrient(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * float, arg2: float, arg3: bool) -> int
     *
     * @par Lua函数原型
     * setSupvOrient(frame: table, max_angle: number, max_rot: number,
     * outside: boolean) -> nil
     *
     */
    int setSupvOrient(const std::vector<double> &frame, double max_angle,
                      double max_rot, bool outside);

    /**
     * FCSupvPos is used to set up position supervision in Force Control.
     * Supervision is activated when Force Control is activated with the
     * instruction FCAct. Position supervision is set up by defining a volume in
     * space for the TCP. Once activated, the supervision will stop the
     * execution if the TCP is outside this volume.
     *
     * @param frame
     * @param box
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setSupvPosBox(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * List[float[6]]) -> int
     *
     * @par Lua函数原型
     * setSupvPosBox(frame: table, box: table) -> nil
     *
     */
    int setSupvPosBox(const std::vector<double> &frame, const Box &box);

    /**
     *
     * @param frame
     * @param cylinder
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setSupvPosCylinder(self: pyaubo_sdk.ForceControl, arg0: List[float],
     * arg1: List[float[5]]) -> int
     *
     * @par Lua函数原型
     * setSupvPosCylinder(frame: table, cylinder: table) -> nil
     *
     */
    int setSupvPosCylinder(const std::vector<double> &frame,
                           const Cylinder &cylinder);

    /**
     *
     * @param frame
     * @param sphere
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setSupvPosSphere(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * List[float[3]]) -> int
     *
     * @par Lua函数原型
     * setSupvPosSphere(frame: table, sphere: table) -> nil
     *
     */
    int setSupvPosSphere(const std::vector<double> &frame,
                         const Sphere &sphere);

    /**
     * FCSupvReoriSpeed is used to set up reorientation speed supervision in
     * Force Control. The supervision is activated when Force Control is
     * activated with the instruction FCAct.
     *
     * The reorientation speed supervision is set up by defining minimum and
     * maximum limits for the reorientation speed around the axis of the work
     * object coordinate system. Once activated, the supervision will stop the
     * execution if the values of the reorientation speed are to high.
     *
     * There are two speed supervisions: FCSupvReoriSpeed and FCSupvTCPSpeed,
     * which is described in section FCSupvTCPSpeed on page 199.
     * Both supervisions may be required because:
     * - A robot axis can rotate with high speed while the TCP is stationary.
     * - The TCP can be far from the rotating axisand asmallaxis rotation may
     * result in a high speed movement of the TCP
     *
     * @param speed_limit
     * @param outside
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setSupvReoriSpeed(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * bool, arg2: float) -> int
     *
     * @par Lua函数原型
     * setSupvReoriSpeed(speed_limit: table, outside: boolean, timeout: number)
     * -> nil
     *
     */
    int setSupvReoriSpeed(const std::vector<double> &speed_limit, bool outside,
                          double timeout);

    /**
     * FCSupvTCPSpeed is used to set up TCP speed supervision in Force Control.
     * The supervision is activated when Force Control is activated with the
     * instruction FCAct. The TCP speed supervision is set up by defining
     * minimum and maximum limits for the TCP speed in the directions of the
     * work object coordinate system. Once activated, the supervision will stop
     * the execution if too high TCP speed values are detected.
     *
     * There are two speed supervisions: FCSupvTCPSpeed and FCSupvReorispeed,
     * which is described in section FCSupvReoriSpeed on page 197.
     *
     * Both supervisions may be required because:
     * - A robot axis can rotate with high speed while the TCP is stationary.
     * - The TCP can be far from the rotating axisand asmallaxis rotation may
     * result in a high speed movement of the TCP.
     *
     * @param speed_limit
     * @param outside
     * @param timeout
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setSupvTcpSpeed(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1:
     * bool, arg2: float) -> int
     *
     * @par Lua函数原型
     * setSupvTcpSpeed(speed_limit: table, outside: boolean, timeout: number) ->
     * nil
     *
     */
    int setSupvTcpSpeed(const std::vector<double> &speed_limit, bool outside,
                        double timeout);

    // 设置低通滤波器
    // --- force frame filter: 过滤测量到的力/力矩
    // +++ force loop filter: 力控输出参考速度的滤波器

    /**
     * 设置低通滤波器
     *
     * --- force frame filter: 过滤测量到的力/力矩
     * +++ force loop filter: 力控输出参考速度的滤波器
     *
     * FCSetLPFilterTune is used change the response of force loop according to
     * description in Damping and LP-filter on page 103.
     *
     * @param cutoff_freq
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setLpFilter(self: pyaubo_sdk.ForceControl, arg0: List[float]) -> int
     *
     * @par Lua函数原型
     * setLpFilter(cutoff_freq: table) -> nil
     *
     */
    int setLpFilter(const std::vector<double> &cutoff_freq);

    /**
     * 重置低通滤波器
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * resetLpFilter(self: pyaubo_sdk.ForceControl) -> int
     *
     * @par Lua函数原型
     * resetLpFilter() -> nil
     *
     */
    int resetLpFilter();

    /**
     * The FCSpdChgAct is used to activate FC SpeedChange function with desired
     * reference and recover behavior. When FC SpeedChange function is active,
     * the robot speed will be reduced/increased in order to keep the measured
     * signal close to the reference.
     *
     * @param ref_force
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * speedChangeEnable(self: pyaubo_sdk.ForceControl, arg0: float) -> int
     *
     * @par Lua函数原型
     * speedChangeEnable(ref_force: number) -> nil
     *
     */
    int speedChangeEnable(double ref_force);

    /**
     * Deactivate FC SpeedChange function.
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * speedChangeDisable(self: pyaubo_sdk.ForceControl) -> int
     *
     * @par Lua函数原型
     * speedChangeDisable() -> nil
     *
     */
    int speedChangeDisable();

    /**
     * FCSpdChgTunSet is used to set FC SpeedChange system parameters to a new
     * value.
     *
     * @param speed_levels
     * @param speed_ratio_min
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * speedChangeTune(self: pyaubo_sdk.ForceControl, arg0: int, arg1: float) ->
     * int
     *
     * @par Lua函数原型
     * speedChangeTune(speed_levels: number, speed_ratio_min: number) -> nil
     *
     */
    int speedChangeTune(int speed_levels, double speed_ratio_min);

    /* Defines how many Newtons are required to make the robot move 1 m/s. The
       higher the value, the less responsive the robot gets.
       The damping can be tuned (as a percentage of the system parameter values)
       by the RAPID instruction FCAct. */
    // 设置阻尼系数，阻尼的系统参数需要通过配置文件设置
    // [damping_fx, damping_fy, damping_fz, damping_tx, damping_ty, damping_tz]
    // A value between min and 10,000,000 Ns/m.
    // A value between minimum and 10,000,000 Nms/rad.

    /**
     * FCSetDampingTune is used to tune the damping in the force control
     * coordinate systems. The parameters tuned are those described in Damping
     * in Torque x Direction - Damping in Torque z Direction on page 255 and
     * Damping in Force x Direction - Damping in Force z Direction on page 254.
     *
     * Damping can be set in the configuration file or by the instruction FCAct.
     * The difference is that this instruction can be used when force control is
     * active. FCSetDampingTune tunes the actual values set by the instruction
     * FCAct, not the value in the configuration file.
     *
     * @param damping
     * @param ramp_time
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_INVL_ARGUMENT
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * setDamping(self: pyaubo_sdk.ForceControl, arg0: List[float], arg1: float)
     * -> int
     *
     * @par Lua函数原型
     * setDamping(damping: table, ramp_time: number) -> nil
     *
     */
    int setDamping(const std::vector<double> &damping, double ramp_time);

    /**
     *
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     * @par Python函数原型
     * resetDamping(self: pyaubo_sdk.ForceControl) -> int
     *
     * @par Lua函数原型
     * resetDamping() -> nil
     *
     */
    int resetDamping();

    /**
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int softFloatEnable();

    /**
     *
     * @return 成功返回0；失败返回错误码
     * AUBO_BUSY
     * AUBO_BAD_STATE
     * -AUBO_BAD_STATE
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    int softFloatDisable();

    /**
     * 返回是否开启了软浮动
     *
     * @return
     *
     * @throws arcs::common_interface::AuboException
     *
     */
    bool isSoftFloatEnabled();

    /**
     * 设置软浮动参数
     *
     * @param joint_softfloat
     * @param select
     * @param stiff_percent
     * @param stiff_damp_ratio
     * @param force_threshold
     * @param force_limit
     * @return
     */
    int setSoftFloatParams(bool joint_space, const std::vector<bool> &select,
                           const std::vector<double> &stiff_percent,
                           const std::vector<double> &stiff_damp_ratio,
                           const std::vector<double> &force_threshold,
                           const std::vector<double> &force_limit);

    /**
     * 检测工具和外部物体的接触
     *
     * @param direction
     * 预期的接触方向，如果所有的元素为0，表示检测所有方向的接触
     * @return
     * 返回从当前点回退到碰撞开始点的周期步数，如果返回值为0，表示没有接触
     */
    int toolContact(const std::vector<bool> &direction);

protected:
    void *d_{ nullptr };
};
using ForceControlPtr = std::shared_ptr<ForceControl>;

// clang-format off
#define ForceControl_DECLARES                                                           \
    _INST(ForceControl, 0, fcEnable)                                                    \
    _INST(ForceControl, 0, fcDisable)                                                   \
    _FUNC(ForceControl, 0, isFcEnabled)                                                 \
    _INST(ForceControl, 5, setTargetForce, feature, compliance, wrench, limits, type)   \
    _INST(ForceControl, 3, setDynamicModel, m, d, k)                                    \
    _FUNC(ForceControl, 0, getDynamicModel)                                             \
    _INST(ForceControl, 4, setCondForce, min, max, outside, timeout)                    \
    _INST(ForceControl, 5, setCondOrient, frame, max_angle, max_rot, outside, timeout)  \
    _INST(ForceControl, 2, setCondPlane, plane, timeout)                                \
    _INST(ForceControl, 4, setCondCylinder, axis, radius, outside, timeout)             \
    _INST(ForceControl, 4, setCondSphere, center, radius, outside, timeout)             \
    _INST(ForceControl, 4, setCondTcpSpeed, min, max, outside, timeout)                 \
    _INST(ForceControl, 0, setCondActive)                                               \
    _INST(ForceControl, 2, setCondDistance, distance, timeout)                          \
    _INST(ForceControl, 3, setCondAdvanced, type, args, timeout)                        \
    _FUNC(ForceControl, 0, isCondFullfiled)                                             \
    _INST(ForceControl, 2, setSupvForce, min, max)                                      \
    _INST(ForceControl, 4, setSupvOrient, frame, max_angle, max_rot, outside)           \
    _INST(ForceControl, 2, setSupvPosBox, frame, box)                                   \
    _INST(ForceControl, 2, setSupvPosCylinder, frame, cylinder)                         \
    _INST(ForceControl, 2, setSupvPosSphere, frame, sphere)                             \
    _INST(ForceControl, 3, setSupvReoriSpeed, speed_limit, outside, timeout)            \
    _INST(ForceControl, 3, setSupvTcpSpeed, speed_limit, outside, timeout)              \
    _INST(ForceControl, 1, setLpFilter, cutoff_freq)                                    \
    _INST(ForceControl, 0, resetLpFilter)                                               \
    _INST(ForceControl, 2, speedChangeTune, speed_levels, speed_ratio_min)              \
    _INST(ForceControl, 1, speedChangeEnable, ref_force)                                \
    _INST(ForceControl, 0, speedChangeDisable)                                          \
    _INST(ForceControl, 2, setDamping, damping, ramp_time)                              \
    _INST(ForceControl, 0, resetDamping)                                                \
    _INST(ForceControl, 0, softFloatEnable)                                             \
    _INST(ForceControl, 0, softFloatDisable)                                            \
    _FUNC(ForceControl, 0, isSoftFloatEnabled)                                          \
    _INST(ForceControl, 6, setSoftFloatParams, joint_space, select, stiff_percent, stiff_damp_ratio, force_threshold, force_limit) \
    _FUNC(ForceControl, 1, toolContact, direction)
// clang-format on
} // namespace common_interface
} // namespace arcs

#endif // AUBO_SDK_FORCE_CONTROL_INTERFACE_H
