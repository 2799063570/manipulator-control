#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <rclcpp/rclcpp.hpp>

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>(
      "moveit_cpp_demo",
      rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  std::thread spinner([&executor]() { executor.spin(); });

  static const std::string planning_group = "arm";
  moveit::planning_interface::MoveGroupInterface move_group(node, planning_group);

  move_group.setPlanningPipelineId("ompl");
  move_group.setPlannerId("RRTConnectkConfigDefault");
  move_group.setPlanningTime(5.0);
  move_group.setNumPlanningAttempts(10);

  const std::vector<double> joint_target = { 0.8, 0.5 };
  move_group.setJointValueTarget(joint_target);

  RCLCPP_INFO(node->get_logger(), "Planning group: %s", planning_group.c_str());
  RCLCPP_INFO(node->get_logger(), "Planner ID: %s", move_group.getPlannerId().c_str());
  RCLCPP_INFO(
      node->get_logger(),
      "Joint target: joint1=%.3f, joint2=%.3f",
      joint_target[0],
      joint_target[1]);

  moveit::planning_interface::MoveGroupInterface::Plan plan;
  const bool success = static_cast<bool>(move_group.plan(plan));

  if (success)
  {
    const auto point_count = plan.trajectory.joint_trajectory.points.size();
    RCLCPP_INFO(
        node->get_logger(),
        "Planning succeeded. Trajectory has %zu point(s).",
        point_count);
  }
  else
  {
    RCLCPP_ERROR(node->get_logger(), "Planning failed.");
  }

  executor.cancel();
  if (spinner.joinable())
  {
    spinner.join();
  }

  rclcpp::shutdown();
  return success ? 0 : 1;
}
