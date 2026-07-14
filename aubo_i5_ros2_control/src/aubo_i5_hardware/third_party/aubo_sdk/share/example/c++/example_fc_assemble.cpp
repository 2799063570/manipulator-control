#include <cstring>
#include <fstream>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "thread"
#include <mutex>

#include "aubo_sdk/rpc.h"
#include "aubo_sdk/rtde.h"

using namespace arcs::aubo_sdk;
using namespace arcs::common_interface;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define EMBEDDED

#define INF 99999

std::mutex rtde_mtx_;
bool cond_fullfiled_ = false;
std::vector<double> tcp_pose_(6, 0.);
std::vector<double> tcp_force_(6, 0.);
// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrival(RobotInterfacePtr impl)
{
    //接口调用: 获取当前的运动指令 ID
    int exec_id = impl->getMotionControl()->getExecId();

    int cnt = 0;
    // 在等待机械臂开始运动时，获取exec_id最大的重试次数
    int max_retry_count = 50;

    // 等待机械臂开始运动
    while (exec_id == -1) {
        if (cnt++ > max_retry_count) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        exec_id = impl->getMotionControl()->getExecId();
    }

    // 等待机械臂动作完成
    while (exec_id != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        exec_id = impl->getMotionControl()->getExecId();
    }

    return 0;
}

inline bool equal(const std::vector<double> &q1, const std::vector<double> &q2,
                  double eps = 0.001)
{
    if (q1.size() && (q1.size() == q2.size())) {
        for (unsigned int i = 0; i < q1.size(); i++) {
            if (std::abs(q1[i] - q2[i]) > eps) {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const std::vector<T> &list)
{
    for (size_t i = 0; i < list.size(); i++) {
        try {
            os << list.at(i);
            if (i != (list.size() - 1)) {
                os << ",";
            }
        } catch (std::exception &e) {
        }
    }
    return os;
}

void tcpSensorTest(RpcClientPtr cli)
{
    // 接口调用: 获取机器人的名字
    auto robot_name = cli->getRobotNames().front();
#ifdef EMBEDDED
    // 内置传感器
    std::vector<double> sensor_pose = { 0, 0, 0, 0, 0, 0 };
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->selectTcpForceSensor("embedded");
#else
    // 外置坤维传感器
    std::vector<double> sensor_pose = { 0, 0, 0.047, 0, 0, 0 };
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->selectTcpForceSensor("kw_ftsensor");
#endif
    while (1) {
        std::cout << "------------------------------------------" << std::endl;
        auto sensor_data = cli->getRobotInterface(robot_name)
                               ->getRobotState()
                               ->getTcpForceSensors();
        std::cout << "force sensor: " << sensor_data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        auto curr_pose =
            cli->getRobotInterface(robot_name)->getRobotState()->getTcpPose();
        auto payload =
            cli->getRobotInterface(robot_name)->getRobotConfig()->getPayload();
        std::cout << "mass: " << std::get<0>(payload)
                  << " cog: " << std::get<1>(payload) << std::endl;
        auto result =
            cli->getRobotInterface(robot_name)
                ->getRobotAlgorithm()
                ->calibrateTcpForceSensor({ sensor_data }, { curr_pose });

        std::cout << "force offset: " << std::get<0>(result) << std::endl;
        std::cout << "------------------------------------------" << std::endl;
    }
}

/**
 * @brief condFullfiled 判断力控终止条件是否满足
 * @param timeout 时间限制,如果时间超过timeout,条件未满足,则返回-1;
 *        如果timeout=-1,则不对时间进行限制
 * @param target_pose 到达目标点后,条件依然没有满足,则返回 0;
 *        如果target_pose={},则不对目标点进行判断
 *
 * @return 条件满足返回1;到位后条件不满足返回0;超时返回-1;
 *         若timeout=-1,target_pose={},该函数一直阻塞,直到满足条件后退出
 */
int condFullfiled(double timeout, std::vector<double> target_pose)
{
    int ret = 0;
    int cnt = 0;

    while (!cond_fullfiled_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        if ((timeout > 0) && (cnt++ > timeout * 500)) {
            std::cout << "条件等待超时 !" << std::endl;
            ret = -1;
            break;
        }
        if ((target_pose.size() == 6) && equal(target_pose, tcp_pose_)) {
            std::cout << "机械臂已到位 !" << std::endl;
            ret = 0;
            break;
        }
    }
    if (cond_fullfiled_) {
        // 条件满足, z方向探寻结束
        ret = 1;
    }
    return ret;
}

/**
 * @brief locateZ z方向定位
 * 通过判断z方向力的变化是否超过一定阈值，通过setCondForce实现
 * @param cli
 */
int locateZ(RpcClientPtr cli)
{
    // 力控参数
    std::vector<double> locate_z_M = { 30.0, 30.0, 30.0, 10.0, 10.0, 10.0 };
    std::vector<double> locate_z_D = {
        300.0, 250.0, 200.0, 100.0, 100.0, 100.0
    };
    std::vector<double> locate_z_K = {
        500.0, 500.0, 500.0, 100.0, 100.0, 100.0
    };
    //力控开启方向
    std::vector<bool> locate_z_enable = {
        true, true, true, false, false, false
    };
    // 目标力
    std::vector<double> locate_z_goal_wrench = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    // 探寻距离
    std::vector<double> seeking_distance = { 0.0, 0.0, -0.015, 0.0, 0.0, 0.0 };
    // 力控坐标系选择 基坐标系
    std::vector<double> feature = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    TaskFrameType frame_type = TaskFrameType::NONE;
    // 力控速度限制
    std::vector<double> speed_limits(6, 2.0);
    // 末端力条件,INF为一个非常大的数值,[-INF,INF]可以认为范围非常大,忽略该方向的监控
    std::vector<double> min_force = { -INF, -INF, -INF, -INF, -INF, -INF };
    std::vector<double> max_force = { INF, INF, 5.0, INF, INF, INF };

    auto robot_name = cli->getRobotNames().front();
    // 设置力控参数
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setDynamicModel(locate_z_M, locate_z_D, locate_z_K);
    // 设置目标力
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(feature, locate_z_enable, locate_z_goal_wrench,
                         speed_limits, frame_type);
    // 设置力的监控条件
    double timeout = 1000;
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setCondForce(min_force, max_force, true, timeout);
    // 获取当前tcp位置
    auto current_pose =
        cli->getRobotInterface(robot_name)->getRobotState()->getTcpPose();
    // 计算探寻终止位置
    std::vector<double> target_pose(6, 0.);
    for (int i = 0; i < 6; i++) {
        target_pose[i] = current_pose[i] + seeking_distance[i];
    }

    // 直线运动
    double v = 0.0025, a = 4.0;
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveLine(target_pose, a, v, 0, 0);
    // 开启力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcEnable();
    cli->getRobotInterface(robot_name)->getForceControl()->setCondActive();
    auto ret = condFullfiled(-1, target_pose);
    cli->getRobotInterface(robot_name)->getMotionControl()->stopJoint(1);
    // 关闭力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcDisable();

    return ret;
}

/**
 * @brief insertDetect 判断螺栓是否插入孔中,前后动两次,两次全部接触认为插入孔中
 * @param cli
 * @return
 */
int insertDetect(RpcClientPtr cli)
{
    // 力控参数
    std::vector<double> insert_detect_M = {
        30.0, 30.0, 30.0, 10.0, 10.0, 10.0
    };
    std::vector<double> insert_detect_D = { 1000.0, 1000.0, 500.0,
                                            100.0,  100.0,  100.0 };
    std::vector<double> insert_detect_K = { 500.0, 500.0, 500.0,
                                            100.0, 100.0, 100.0 };

    //力控开启方向
    std::vector<bool> insert_detect_enable = { true,  true,  false,
                                               false, false, false };
    // 力控坐标系选择 基坐标系
    std::vector<double> feature = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    TaskFrameType frame_type = TaskFrameType::NONE;
    // 力控速度限制
    std::vector<double> speed_limits(6, 2.0);

    auto robot_name = cli->getRobotNames().front();
    // 设置力控参数
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setDynamicModel(insert_detect_M, insert_detect_D, insert_detect_K);

    // 目标力
    std::vector<double> insert_detect_goal_wrench = { -8.0, 0.0, 0.0,
                                                      0.0,  0.0, 0.0 };
    // 设置目标力
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(feature, insert_detect_enable,
                         insert_detect_goal_wrench, speed_limits, frame_type);

    double min_force = -INF;
    double max_force = 5.0;
    std::vector<double> select = { 1., 1., 0., 0., 0., 0. };
    double timeout = -1;
    double count = 3.;
    double outside = 1;
    std::vector<double> args;
    args.insert(args.end(), min_force);
    args.insert(args.end(), max_force);
    args.insert(args.end(), select.begin(), select.end());
    args.insert(args.end(), count);
    args.insert(args.end(), outside);
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setCondAdvanced("ConditionContactForce", args, timeout);

    // 开启力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcEnable();
    cli->getRobotInterface(robot_name)->getForceControl()->setCondActive();
    auto ret = condFullfiled(3, {});
    if (ret < 0) {
        cli->getRobotInterface(robot_name)->getMotionControl()->stopJoint(1);
        // 关闭力控
        cli->getRobotInterface(robot_name)->getForceControl()->fcDisable();
        return ret;
    } else {
        std::cout << "插入检测: 正向接触成功!" << std::endl;
    }
    // 目标力
    insert_detect_goal_wrench = { 8.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    // 开始反向接触
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(feature, insert_detect_enable,
                         insert_detect_goal_wrench, speed_limits, frame_type);
    ret = condFullfiled(3, {});
    if (ret < 0) {
        std::cout << "插入失败, 插入检测结束!" << std::endl;
        return ret;
    } else {
        std::cout << "插入检测: 反向接触成功!" << std::endl;
    }
    cli->getRobotInterface(robot_name)->getMotionControl()->stopJoint(1);
    // 关闭力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcDisable();
    return ret;
}

/**
 * @brief locateXY 定位螺栓位置,在xy平面走螺旋轨迹
 * @param cli
 * @return
 */
int locateXY(RpcClientPtr cli)
{
    // 力控参数
    std::vector<double> locate_xy_M = { 30.0, 30.0, 30.0, 10.0, 10.0, 10.0 };
    std::vector<double> locate_xy_D = { 1000.0, 1000.0, 500.0,
                                        100.0,  100.0,  100.0 };
    std::vector<double> locate_xy_K = {
        500.0, 500.0, 500.0, 100.0, 100.0, 100.0
    };

    //力控开启方向
    std::vector<bool> locate_xy_enable = {
        true, true, true, false, false, false
    };
    // 目标力
    std::vector<double> locate_xy_goal_wrench = {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

    // 力控坐标系选择 基坐标系
    std::vector<double> feature = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    TaskFrameType frame_type = TaskFrameType::NONE;
    // 力控速度限制
    std::vector<double> speed_limits(6, 2.0);

    auto robot_name = cli->getRobotNames().front();
    // 设置力控参数
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setDynamicModel(locate_xy_M, locate_xy_D, locate_xy_K);
    // 设置目标力
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(feature, locate_xy_enable, locate_xy_goal_wrench,
                         speed_limits, frame_type);
    double min_force = -INF;
    double max_force = 8.0;
    std::vector<double> select = { 1., 0., 0., 0., 0., 0. };
    double timeout = -1;
    double count = 1.;
    double outside = 1;
    std::vector<double> args_x;
    args_x.insert(args_x.end(), min_force);
    args_x.insert(args_x.end(), max_force);
    args_x.insert(args_x.end(), select.begin(), select.end());
    args_x.insert(args_x.end(), count);
    args_x.insert(args_x.end(), outside);
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setCondAdvanced("ConditionContactForce", args_x, timeout);

    select = { 0., 1., 0., 0., 0., 0. };
    std::vector<double> args_y;
    args_y.insert(args_y.end(), min_force);
    args_y.insert(args_y.end(), max_force);
    args_y.insert(args_y.end(), select.begin(), select.end());
    args_y.insert(args_y.end(), count);
    args_y.insert(args_y.end(), outside);
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setCondAdvanced("ConditionContactForce", args_y, timeout);

    SpiralParameters spiral;
    spiral.spiral = 0.001;       // 螺旋线步长
    spiral.helix = 0.;           // 平面螺旋
    spiral.angle = 5 * 2 * M_PI; // 旋转5圈
    spiral.plane = 0;
    double radius = 0.001; // 螺旋线第一圈半径

    spiral.frame =
        cli->getRobotInterface(robot_name)->getRobotState()->getTcpPose();

    // 计算螺旋轨迹中心点
    spiral.frame[0] =
        std::sqrt(2) * radius / 2 + spiral.frame[0]; //计算螺旋线半径参考点
    spiral.frame[1] = std::sqrt(2) * radius / 2 + spiral.frame[1];
    double v = 0.015, a = 0.2;
    // 螺旋运动
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveSpiral(spiral, 0, v, a, 0);
    // 开启力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcEnable();
    // 激活终止条件
    cli->getRobotInterface(robot_name)->getForceControl()->setCondActive();
    auto ret = condFullfiled(10, {});
    cli->getRobotInterface(robot_name)->getMotionControl()->stopJoint(1);
    // 关闭力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcDisable();
    return ret;
}

/**
 * @brief calBoltPose 计算螺栓位置
 * @param cli
 * @param bolt_len  螺栓长度
 * @return
 */
std::vector<double> calBoltPose(RpcClientPtr cli, double bolt_len)
{
    double max_force = 8.0;
    double diameter = 0.008; // 螺栓直径
    auto robot_name = cli->getRobotNames().front();
    auto tcp_wrench =
        cli->getRobotInterface(robot_name)->getRobotState()->getTcpForce();
    std::vector<double> bolt_pose =
        cli->getRobotInterface(robot_name)->getRobotState()->getTcpPose();

    if ((fabs(tcp_wrench[0]) > max_force) &&
        (fabs(tcp_wrench[1]) < max_force)) {
        // x方向力>limi，cht，y方向<limit,向x方向移动
        bolt_pose[0] =
            bolt_pose[0] - (tcp_wrench[0] / fabs(tcp_wrench[0])) * diameter;

    }

    else if ((fabs(tcp_wrench[0]) < max_force) &&
             (fabs(tcp_wrench[1]) > max_force)) {
        // x方向力<limit，y方向>limit,向y方向移动
        bolt_pose[1] =
            bolt_pose[1] - (tcp_wrench[1] / fabs(tcp_wrench[1])) * diameter;

    } else if ((fabs(tcp_wrench[0]) > max_force) &&
               (fabs(tcp_wrench[1]) > max_force)) {
        // x方向力>limit，y方向>limit,向xy复合方向移动
        double contact_force = std::sqrt(tcp_wrench[0] * tcp_wrench[0] +
                                         tcp_wrench[1] * tcp_wrench[1]);
        bolt_pose[0] =
            bolt_pose[0] - (tcp_wrench[0] / contact_force) * diameter;
        bolt_pose[1] =
            bolt_pose[1] - (tcp_wrench[1] / contact_force) * diameter;
    }
    bolt_pose[2] = bolt_pose[2] + bolt_len;
    return bolt_pose;
}

/**
 * @brief moveToBoltPose 移动到螺栓上方
 * @param cli
 * @param bolt_pose 螺栓位置
 * @param bolt_len  螺栓长度
 * @return
 */
int moveToBoltPose(RpcClientPtr cli, std::vector<double> &bolt_pose,
                   double bolt_len)

{
    // 力控参数
    std::vector<double> move_M = { 30.0, 30.0, 30.0, 10.0, 10.0, 10.0 };
    std::vector<double> move_D = { 300.0, 250.0, 200.0, 100.0, 100.0, 100.0 };
    std::vector<double> move_K = { 500.0, 500.0, 500.0, 100.0, 100.0, 100.0 };
    //力控开启方向
    std::vector<bool> move_fc_enable = {
        true, true, false, false, false, false
    };
    // 目标力
    std::vector<double> move_goal_wrench = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    // 力控坐标系选择 基坐标系
    std::vector<double> feature = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    TaskFrameType frame_type = TaskFrameType::NONE;
    // 力控速度限制
    std::vector<double> speed_limits(6, 2.0);

    auto robot_name = cli->getRobotNames().front();
    // 设置力控参数
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setDynamicModel(move_M, move_D, move_K);
    // 设置目标力
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(feature, move_fc_enable, move_goal_wrench,
                         speed_limits, frame_type);
    double v = 0.02, a = 4.0;
    std::vector<double> trans_pose =
        cli->getRobotInterface(robot_name)->getRobotState()->getTcpPose();
    trans_pose[2] = trans_pose[2] + bolt_len;
    // 移动至过渡点
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveLine(trans_pose, a, v, 0, 0);

    // 先调用运动接口,在开启力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcEnable();
    waitArrival(cli->getRobotInterface(robot_name));
    // 移动至螺栓上方
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveLine(bolt_pose, a, v, 0, 0);
    waitArrival(cli->getRobotInterface(robot_name));
    cli->getRobotInterface(robot_name)->getMotionControl()->stopJoint(1);
    // 关闭力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcDisable();
    return 0;
}

/**
 * @brief seekZ z方向探寻, 在螺栓上方,向下探寻
 * @return
 */
/*
int seekZ(RpcClientPtr cli)
{
    std::vector<double> seek_M = { 30.0, 30.0, 30.0, 10.0, 10.0, 10.0 };
    std::vector<double> seek_D = { 300.0, 300.0, 500.0, 100.0, 100.0, 100.0 };
    std::vector<double> seek_K = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    std::vector<bool> seek_fc_enable = {
        false, false, true, false, false, false
    };
    std::vector<double> seek_goal_wrench = { 0.0, 0.0, 5.0, 0.0, 0.0, 0.0 };
    std::vector<double> seek_force_threshold = { 1.0, 1.0, 1.0, 0.1, 0.1, 0.1 };

    // 力控坐标系选择 基坐标系
    std::vector<double> feature = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    TaskFrameType frame_type = TaskFrameType::NONE;
    // 力控速度限制
    std::vector<double> speed_limits(6, 2.0);

    auto robot_name = cli->getRobotNames().front();
    // 设置力控参数
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setDynamicModel(seek_M, seek_D, seek_K);

    // 设置目标力
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(feature, seek_fc_enable, seek_goal_wrench,
                         speed_limits, frame_type);

    double min_force = -INF;
    double max_force = fabs(seek_goal_wrench[2]) - 2.0;
    std::vector<double> select = { 0., 0., 1., 0., 0., 0. };
    double timeout = -1;
    double count = 5.;
    double outside = 1;
    std::vector<double> args;
    args.insert(args.end(), min_force);
    args.insert(args.end(), max_force);
    args.insert(args.end(), select.begin(), select.end());
    args.insert(args.end(), count);
    args.insert(args.end(), outside);
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setCondAdvanced("ConditionContactForce", args, timeout);
    cli->getRobotInterface(robot_name)->getForceControl()->fcEnable();
    // 激活终止条件
    cli->getRobotInterface(robot_name)->getForceControl()->setCondActive();
    auto ret = condFullfiled(5, {});
    if (ret < 0) {
        std::cout << "z方向探寻失败, 未找到螺栓!" << std::endl;
    } else {
        std::cout << "z方向探寻成功!" << std::endl;
    }
    cli->getRobotInterface(robot_name)->getMotionControl()->stopJoint(1);
    // 关闭力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcDisable();
    return ret;
}
*/

int searchXY(RpcClientPtr cli)
{
    std::vector<double> search_M = { 30.0, 30.0, 30.0, 10.0, 10.0, 10.0 };
    std::vector<double> search_D = { 600.0, 500.0, 500.0, 200.0, 200.0, 200.0 };
    std::vector<double> search_K = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    std::vector<bool> search_fc_enable = {
        false, false, true, false, false, false,
    };
    std::vector<double> search_goal_wrench = { 0.0, 0.0, 8.5, 0.0, 0.0, 0.0 };

    // 力控坐标系选择 基坐标系
    std::vector<double> feature = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    TaskFrameType frame_type = TaskFrameType::NONE;
    // 力控速度限制
    std::vector<double> speed_limits(6, 2.0);

    auto robot_name = cli->getRobotNames().front();
    // 设置力控参数
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setDynamicModel(search_M, search_D, search_K);

    // 设置目标力
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(feature, search_fc_enable, search_goal_wrench,
                         speed_limits, frame_type);

    double min_force = -INF;
    double max_force = 4.5;
    std::vector<double> select = { 0., 0., 1., 0., 0., 0. };
    double timeout = -1;
    double count = 3.;
    double outside = 0;
    std::vector<double> args_x;
    args_x.insert(args_x.end(), min_force);
    args_x.insert(args_x.end(), max_force);
    args_x.insert(args_x.end(), select.begin(), select.end());
    args_x.insert(args_x.end(), count);
    args_x.insert(args_x.end(), outside);
    // z方向受力突然变小,认为搜到螺孔
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setCondAdvanced("ConditionContactForce", args_x, timeout);
    // 下探2mm,认为搜到螺孔
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setCondDistance(0.002, -1);
    SpiralParameters spiral;
    spiral.spiral = 0.001;       // 螺旋线步长
    spiral.helix = 0.;           // 平面螺旋
    spiral.angle = 7 * 2 * M_PI; // 旋转5圈
    spiral.plane = 0;
    double radius = 0.001; // 螺旋线第一圈半径

    spiral.frame =
        cli->getRobotInterface(robot_name)->getRobotState()->getTcpPose();
    // 计算螺旋轨迹中心点
    spiral.frame[0] =
        std::sqrt(2) * radius / 2 + spiral.frame[0]; //计算螺旋线半径参考点
    spiral.frame[1] = std::sqrt(2) * radius / 2 + spiral.frame[1];
    double v = 0.002, a = 0.1;
    // 螺旋运动
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveSpiral(spiral, 0, v, a, 0);
    // 开启力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcEnable();
    // 激活终止条件
    cli->getRobotInterface(robot_name)->getForceControl()->setCondActive();

    auto ret = condFullfiled(60, {});
    cli->getRobotInterface(robot_name)->getMotionControl()->stopJoint(1);
    // 关闭力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcDisable();
    return ret;
}

int insertZ(RpcClientPtr cli)
{
    std::vector<double> insert_M = { 30.0, 30.0, 30.0, 5.0, 5.0, 5.0 };
    std::vector<double> insert_D = { 600.0, 600.0, 600.0, 100.0, 100.0, 100.0 };
    std::vector<double> insert_K = { 10.0, 10.0, 500.0, 1.0, 1.0, 400.0 };
    std::vector<bool> insert_fc_enable = { true, true, true, true, true, true };
    std::vector<double> insert_goal_wrench = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    std::vector<double> insert_depth = { 0.0, 0.0, -0.01, 0.0, 0.0, 0.0 };
    // 力控坐标系选择 基坐标系
    std::vector<double> feature = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    TaskFrameType frame_type = TaskFrameType::NONE;
    // 力控速度限制
    std::vector<double> speed_limits(6, 2.0);

    auto robot_name = cli->getRobotNames().front();
    // 设置力控参数
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setDynamicModel(insert_M, insert_D, insert_K);

    // 设置目标力
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setTargetForce(feature, insert_fc_enable, insert_goal_wrench,
                         speed_limits, frame_type);
    // 获取当前tcp位置
    auto current_pose =
        cli->getRobotInterface(robot_name)->getRobotState()->getTcpPose();

    std::vector<double> cylinder_center = { current_pose[0], current_pose[1],
                                            current_pose[2] + insert_depth[2] };

    double radius_ = 1000.;
    cli->getRobotInterface(robot_name)
        ->getForceControl()
        ->setCondCylinder(cylinder_center, radius_, true, -1);
    // 计算插入终止位置
    std::vector<double> target_pose(6, 0.);
    for (int i = 0; i < 6; i++) {
        target_pose[i] = current_pose[i] + insert_depth[i];
    }
    // 直线运动
    double v = 0.0025, a = 4.0;
    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveLine(target_pose, a, v, 0, 0);

    // 开启力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcEnable();
    // 激活终止条件
    cli->getRobotInterface(robot_name)->getForceControl()->setCondActive();

    auto ret = condFullfiled(5, target_pose);

    cli->getRobotInterface(robot_name)->getMotionControl()->stopJoint(1);
    // 关闭力控
    cli->getRobotInterface(robot_name)->getForceControl()->fcDisable();
    return ret;
}

int example(RpcClientPtr cli)
{
    auto robot_name = cli->getRobotNames().front();
#ifdef EMBEDDED
    // 内置传感器
    std::vector<double> sensor_pose = { 0, 0, 0, 0, 0, 0 };
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->selectTcpForceSensor("embedded");
#else
    // 外置坤维传感器
    std::vector<double> sensor_pose = { 0, 0, 0.047, 0, 0, 0 };
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->selectTcpForceSensor("kw_ftsensor");
#endif

    // 设置传感器安装位姿
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->setTcpForceSensorPose(sensor_pose);
    // 设置TCP偏移
    std::vector<double> tcp_pose = { 0, 0, 0.04, 0, 0, 0 };
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->setTcpOffset(tcp_pose);

    double mass = 0.542767;
    std::vector<double> com = { 0.00650296, -0.000310792, 0.0199925 };
    // 力传感器偏移需要根据实际情况设置
    std::vector<double> force_offset = { 13.0463,  -11.9162, -33.9405,
                                         0.168684, -1.07947, 0.403233 };
    // 设置负载
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->setPayload(mass, com, { 0. }, { 0. });

    // 设置力传感器偏移
    cli->getRobotInterface(robot_name)
        ->getRobotConfig()
        ->setTcpForceOffset(force_offset);

    std::vector<double> q = { -22.74 / 180 * M_PI, 1.52 / 180 * M_PI,
                              82.7 / 180 * M_PI,   -8.6 / 180 * M_PI,
                              93.42 / 180 * M_PI,  112.02 / 180 * M_PI };

    cli->getRobotInterface(robot_name)
        ->getMotionControl()
        ->moveJoint(q, 20.0 / 180.0 * M_PI, 20.0 / 180.0 * M_PI, 0.0, 0.0);
    waitArrival(cli->getRobotInterface(robot_name));

    // 两次重试机会
    for (int i = 0; i < 2; i++) {
        if (locateZ(cli) > 0) {
            std::cout << "定位到螺栓,开始xy平面搜索螺孔位置" << std::endl;
            if (searchXY(cli) > 0) {
                std::cout << "条件满足, 确认是否插入" << std::endl;
                if (insertDetect(cli) > 0) {
                    std::cout << "检测到已插入, 开始插入动作" << std::endl;
                    if (insertZ(cli) < 0) {
                        std::cout << "插入超时,请检查是否插入,或调整等待时间"
                                  << std::endl;
                    } else {
                        std::cout << "插入已完成,结束程序" << std::endl;
                    }
                    return 0;
                } else {
                    std::cout << "检测到未插入,可能插入孔外, 回到起点"
                              << std::endl;
                    cli->getRobotInterface(robot_name)
                        ->getMotionControl()
                        ->moveJoint(q, 20.0 / 180.0 * M_PI, 20.0 / 180.0 * M_PI,
                                    0.0, 0.0);
                    waitArrival(cli->getRobotInterface(robot_name));
                    continue;
                }
            } else {
                std::cout << "搜索超时, 退出搜索, 回到起点" << std::endl;
                cli->getRobotInterface(robot_name)
                    ->getMotionControl()
                    ->moveJoint(q, 20.0 / 180.0 * M_PI, 20.0 / 180.0 * M_PI,
                                0.0, 0.0);
                waitArrival(cli->getRobotInterface(robot_name));
                continue;
            }
        } else {
            std::cout << "探寻距离已到, 未定位到螺栓, 确认是否插入"
                      << std::endl;
            if (insertDetect(cli) > 0) {
                std::cout << "检测到已插入, 开始插入动作" << std::endl;
                if (insertZ(cli) < 0) {
                    std::cout << "插入超时, 请检查是否插入, 或调整等待时间"
                              << std::endl;
                } else {
                    std::cout << "插入已完成, 结束程序" << std::endl;
                }
                return 0;
            } else {
                std::cout << "检测到未插入, 开始xy平面定位螺栓位置"
                          << std::endl;
                if (locateXY(cli) < 0) {
                    std::cout << "搜索超时, 未定位到螺栓, 结束搜索,回到起点"
                              << std::endl;
                    cli->getRobotInterface(robot_name)
                        ->getMotionControl()
                        ->moveJoint(q, 20.0 / 180.0 * M_PI, 20.0 / 180.0 * M_PI,
                                    0.0, 0.0);
                    waitArrival(cli->getRobotInterface(robot_name));
                    continue;
                } else {
                    auto bolt_pose = calBoltPose(cli, 0.015);
                    std::cout << "定位到螺栓, 螺栓位置为: " << bolt_pose
                              << std::endl;
                    moveToBoltPose(cli, bolt_pose, 0.015);
                    std::cout << "移动到螺栓上方" << std::endl;
                }
            }
        }
    }
}

#define LOCAL_IP "172.16.3.68"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    auto rpc = std::make_shared<RpcClient>();
    // 接口调用: 设置 RPC 超时
    rpc->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务
    rpc->connect(LOCAL_IP, 30004);
    // 接口调用: 登录
    rpc->login("aubo", "123456");

    auto rtde_cli = std::make_shared<RtdeClient>();
    // 接口调用: 连接到 RTDE 服务
    rtde_cli->connect(LOCAL_IP, 30010);
    // 接口调用: 登录
    rtde_cli->login("aubo", "123456");

    // 接口调用: 设置话题, 力控条件,tcp位置,tcp力
    int topic = rtde_cli->setTopic(
        false,
        { "R1_fc_cond_fullfiled", "R1_actual_TCP_pose", "R1_actual_TCP_force" },
        200, 1);
    // 接口调用: 订阅
    rtde_cli->subscribe(topic, [](InputParser &parser) {
        std::unique_lock<std::mutex> lck(rtde_mtx_);
        cond_fullfiled_ = parser.popBool();
        tcp_pose_ = parser.popVectorDouble();
        tcp_force_ = parser.popVectorDouble();
    });

    example(rpc);

    /* 测试力传感器数据 */
    // tcpSensorTest(rpc);
    return 0;
}
