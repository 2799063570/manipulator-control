#include "rclcpp/rclcpp.hpp"
#include <memory>
#include <thread>
#include "moveit/move_group_interface/move_group_interface.hpp"

#include <geometry_msgs/msg/pose.hpp>

#include <tf2/LinearMath/Quaternion.h>


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