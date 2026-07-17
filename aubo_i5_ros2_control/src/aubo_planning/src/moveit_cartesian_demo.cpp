#include <rclcpp/rclcpp.hpp>

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.hpp>

#include <moveit_msgs/msg/robot_trajectory.hpp>

#include <geometry_msgs/msg/pose.hpp>

#include <tf2/LinearMath/Quaternion.h>

#include <thread>
#include <moveit/robot_trajectory/robot_trajectory.hpp>
#include <moveit/trajectory_processing/time_optimal_trajectory_generation.hpp>
/**
 * @file      moveit_cartesian_demo.cpp
 * @brief     基于 MoveIt! C++ 接口的机械臂笛卡尔空间位姿控制示例
 * @details
 * 这里我们需要调用的是move_group接口的MoveGroupInterface类，来向move_group节点发送运动规划请求
 * move_group节点会调用OMPL规划器来计算机械臂从当前状态到目标状态的运动轨迹，并将轨迹下发给底层控制器执行。

 * 还是会设置目标位姿，通过接口向move_group发送请求(home\target)
 * 重点在于后面的笛卡尔路径规划：
 * 1. 构建姿态数组（waypoints），包含起始位姿和多个中间位姿。
 * 2. 调用 computeCartesianPath() 计算笛卡尔空间下的运动轨迹，得到一个纯几何路径（不包含时间参数）。
		输入：姿态数组、末端步长、跳跃阈值、存储轨迹的对象、是否避障（发生碰撞暂停剩下的规划路径）
		输出：规划成功的比例（0~1）
 * 3. 对计算出的纯几何路径进行时间参数化处理，生成带有速度和加速度信息的轨迹。
 * 4. 将优化后的轨迹发送给底层控制器执行。
 * * @note   运行此节点前，需确保已经启动了机器人的 MoveIt! 环境（包含 RobotModel 和 PlanningScene）。
 * * 笛卡尔路径规划的核心在于死死的咬住直线, 规划器只能在笛卡尔空间内寻找满足条件的轨迹，无法绕开障碍物。
 * * 底层只会进行纯几何插值，生成的轨迹不包含时间参数（速度、加速度等），需要后续的时间参数化算法来优化轨迹
 * @author    QueChen
 * @date      2026-07-16
 */

 double durationToSec(
    const builtin_interfaces::msg::Duration &d)
{
    return d.sec + d.nanosec * 1e-9;
}

 // 滤去重复的轨迹点，避免时间参数化失败
static void filterDuplicateTrajectoryPoints(moveit_msgs::msg::RobotTrajectory &trajectory)
{
	auto &points = trajectory.joint_trajectory.points;
	if (points.size() < 2)
	{
		return;
	}

	std::vector<trajectory_msgs::msg::JointTrajectoryPoint> filtered_points;
	filtered_points.reserve(points.size());
	filtered_points.push_back(points.front());

	const double position_epsilon = 1e-6;
	for (size_t i = 1; i < points.size(); ++i)
	{
		const auto &current_point = points[i];
		const auto &previous_point = filtered_points.back();
		bool is_duplicate = current_point.positions.size() == previous_point.positions.size();

		if (is_duplicate)
		{
			for (size_t j = 0; j < current_point.positions.size(); ++j)
			{
				if (std::abs(current_point.positions[j] - previous_point.positions[j]) > position_epsilon)
				{
					is_duplicate = false;
					break;
				}
			}
		}

		if (!is_duplicate)
		{
			filtered_points.push_back(current_point);
		}
	}

	points.swap(filtered_points);
}

static void ensureStrictlyIncreasingTimeFromStart(moveit_msgs::msg::RobotTrajectory &trajectory)
{
	auto &points = trajectory.joint_trajectory.points;
	const double minimum_step = 0.01;

	if (points.size() < 2)
	{
		return;
	}

	for (size_t i = 1; i < points.size(); ++i)
	{
		if (durationToSec(points[i].time_from_start) <= durationToSec(points[i-1].time_from_start))
		{
			auto &last = points[i-1].time_from_start;

			points[i].time_from_start.sec = last.sec;
			points[i].time_from_start.nanosec = last.nanosec + static_cast<uint32_t>(minimum_step * 1e9);
		}
	}
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("cartesian_demo");

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    std::thread spinner([&executor]()
    {
       executor.spin(); 
    });

    moveit::planning_interface::MoveGroupInterface arm(node, "arm");
	std::string end_effector_link = arm.getEndEffectorLink();
    // 设置目标位置所使用的参考坐标系
	std::string reference_frame = "base_link";
	arm.setPoseReferenceFrame(reference_frame);

    // 当运动规划失败后，允许重新规划
	arm.allowReplanning(true);

	// 设置位置(单位：米)和姿态（单位：弧度）的允许误差
	arm.setGoalPositionTolerance(0.001);
	arm.setGoalOrientationTolerance(0.01);

	// 设置允许的最大速度和加速度
	arm.setMaxAccelerationScalingFactor(0.2);
	arm.setMaxVelocityScalingFactor(0.2);

	// 控制机械臂先回到初始化位置
	arm.setNamedTarget("home");
	arm.move();
	sleep(1);
	geometry_msgs::msg::Pose target_posel; // 设置目标位置

	tf2::Quaternion q;
	q.setRPY(3.14, 0, -1.57); // radian

	target_posel.orientation.x = q.x();
	target_posel.orientation.y = q.y();
	target_posel.orientation.z = q.z();
	target_posel.orientation.w = q.w();

	target_posel.position.x = 0;
	target_posel.position.y = -0.4;
	target_posel.position.z = 0.4;

	arm.setPoseTarget(target_posel);
	arm.move();

	// 获取当前位姿数据最为机械臂运动的起始位姿
	geometry_msgs::msg::Pose start_pose = arm.getCurrentPose(end_effector_link).pose;

	std::vector<geometry_msgs::msg::Pose> waypoints;

	// 将初始位姿加入路点列表
	waypoints.push_back(start_pose);

	start_pose.position.x += 0.1;
	waypoints.push_back(start_pose);

	start_pose.position.x -= 0.1;
	waypoints.push_back(start_pose);

	start_pose.position.z -= 0.2;
	waypoints.push_back(start_pose);

	// 笛卡尔空间下的路径规划
	moveit_msgs::msg::RobotTrajectory trajectory;
	// const double jump_threshold = 0.2;
	const double eef_step = 0.01;
	double fraction = 0.0;
	int maxtries = 100; // 最大尝试规划次数
	int attempts = 0;	// 已经尝试规划次数

	while (fraction < 1.0 && attempts < maxtries)
	{
		fraction = arm.computeCartesianPath(waypoints, eef_step, trajectory);
		attempts++;

		if (attempts % 10 == 0)
			RCLCPP_INFO(node->get_logger(),
                "Still trying after %d attempts...", attempts);
	}

    if (fraction > 0.99)
	{
		RCLCPP_INFO(node->get_logger(),
            "Path computed successfully. Now optimizing trajectory (Time Parameterization)...");

		// 1. 获取当前的机器人状态
		moveit::core::RobotStatePtr current_state = arm.getCurrentState();

		// 2. 创建一个 robot_trajectory::RobotTrajectory 对象（这是内部处理类，非 msg 类型）
		// 参数需要传入机器人模型和对应的规划组名称
		robot_trajectory::RobotTrajectory rt(arm.getRobotModel(), "arm");

		// 3. 将之前算出的纯几何路径 moveit_msgs::RobotTrajectory 加载进来
		rt.setRobotTrajectoryMsg(*current_state, trajectory);
		filterDuplicateTrajectoryPoints(trajectory);
		rt.setRobotTrajectoryMsg(*current_state, trajectory);

		// 4. 实例化时间参数化算法 (TOTG)
		trajectory_processing::TimeOptimalTrajectoryGeneration totg;

		// 5. 计算并填充速度、加速度和时间戳
		// 这里的 0.2, 0.2 是最大速度和最大加速度的缩放因子 (Scaling Factor)
		// 建议在真实机械臂上测试时，先从极小的值（如 0.05）开始，确认无误再调大
		bool success = totg.computeTimeStamps(rt, 0.2, 0.2);

		if (success)
		{
			RCLCPP_INFO(node->get_logger(),
                "Trajectory time parameterization SUCCESS! Moving the arm.");

			// 6. 将优化好的、带平滑速度曲线的轨迹，重新转换回 ROS 消息格式
			rt.getRobotTrajectoryMsg(trajectory);

			// 7. 塞入 plan 并发送给真实机械臂执行
			moveit::planning_interface::MoveGroupInterface::Plan plan;
			plan.trajectory = trajectory;
			sleep(0.4);
			ensureStrictlyIncreasingTimeFromStart(plan.trajectory);

			const auto& pts = plan.trajectory.joint_trajectory.points;
			for (size_t i = 1; i < pts.size(); ++i)
			{
				double dt = durationToSec(pts[i].time_from_start) - durationToSec(pts[i-1].time_from_start);
				// double dt = pts[i].time_from_start.toSec() - pts[i-1].time_from_start.toSec();
				if (dt <= 0)
				{
					RCLCPP_ERROR(node->get_logger(),
                        "Non-increasing time at point %lu: dt = %f", i, dt);
				}
			}
			
			arm.execute(plan); // 执行运动
			sleep(1);
		}
		else
		{
			RCLCPP_WARN(node->get_logger(),"Trajectory time parameterization FAILED!");
		}
	}

	// 控制机械臂先回到初始化位置
	arm.setNamedTarget("home");
	arm.move();

    executor.cancel();

    if (spinner.joinable())
    {
        spinner.join();
    }

    rclcpp::shutdown();

    return 0;
}
