#include "rclcpp/rclcpp.hpp"
#include <memory>
#include <thread>
#include "moveit/move_group_interface/move_group_interface.hpp"

#include <geometry_msgs/msg/pose.hpp>

#include <tf2/LinearMath/Quaternion.h>

/**
 * @file      grasp_init.cpp
 * @brief     基于 MoveIt! C++ 接口的机械臂笛卡尔空间位姿控制示例
 * @details   
 * 这里我们需要调用的是move_group接口的MoveGroupInterface类，来向move_group节点发送运动规划请求
 * move_group节点会调用OMPL规划器来计算机械臂从当前状态到目标状态的运动轨迹，并将轨迹下发给底层控制器执行。
 * 该程序实现了以下完整的 MoveIt! 运动规划与执行流程：
 * 1. 初始化 ROS 节点并启动异步 Spinner 以处理回调。（因为这里会作为action的client）
 * 2. 连接到名为 "arm" 的规划组（JointModelGroup）。
 * 3. 通过设定欧拉角 (RPY) 并转换为四元数，构建末端执行器的目标姿态。
 * 4. 设定末端执行器在 "base_link" 坐标系下的目标空间位置 (X, Y, Z)。
 * 5. 调用 OMPL 规划器计算无碰撞轨迹（仅规划，不移动）。
 * 6. 规划成功后，将轨迹下发给底层控制器执行真实的物理/仿真运动。
 * * @note      运行此节点前，需确保已经启动了机器人的 MoveIt! 环境（包含 RobotModel 和 PlanningScene）。
 *          同时 该节点不能独立运行 需要launch加载机械臂的相关参数（URDF、SRDF）
 * @author    QueChen
 * @date      2026-07-15
 */


class graspInit : public rclcpp::Node
{
public:
    graspInit() : Node("grasp_init")
    {
        RCLCPP_INFO(this->get_logger(), "helloworld!");

    }
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<graspInit>();
    // auto node = rclcpp::Node::make_shared("moveit_custom_demo");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    std::thread spinner([&executor]()
    {
       executor.spin(); 
    });

    moveit::planning_interface::MoveGroupInterface move_group(node, "arm");
    move_group.setPoseReferenceFrame("base_link");
    tf2::Quaternion q;
    q.setRPY(3.14, 0.0, 0.0);
    geometry_msgs::msg::Pose target_pose;
    target_pose.orientation.x = q.x();
    target_pose.orientation.y = q.y();
    target_pose.orientation.z = q.z();
    target_pose.orientation.w = q.w();

    target_pose.position.x = -0.45;
    target_pose.position.y = 0.0;
    target_pose.position.z = 0.30;

    move_group.setPoseTarget(target_pose);
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    bool success =
        (move_group.plan(my_plan) ==
         moveit::core::MoveItErrorCode::SUCCESS);
    
    RCLCPP_INFO(
        node->get_logger(),
        "Pose planning %s",
        success ? "SUCCESS" : "FAILED");

    if (success)
    {
        move_group.execute(my_plan);
    }
    executor.cancel();

    if (spinner.joinable())
    {
        spinner.join();
    }

    rclcpp::shutdown();

    return 0;
}