#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <rclcpp/rclcpp.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>

namespace
{
std::string getStringParameter(
  const rclcpp::Node::SharedPtr & node,
  const std::string & name,
  const std::string & default_value)
{
  if (!node->has_parameter(name)) {
    node->declare_parameter<std::string>(name, default_value);
  }
  return node->get_parameter(name).as_string();
}

std::vector<std::string> getStringArrayParameter(
  const rclcpp::Node::SharedPtr & node,
  const std::string & name,
  const std::vector<std::string> & default_value)
{
  if (!node->has_parameter(name)) {
    node->declare_parameter<std::vector<std::string>>(name, default_value);
  }
  return node->get_parameter(name).as_string_array();
}

std::vector<double> getDoubleArrayParameter(
  const rclcpp::Node::SharedPtr & node,
  const std::string & name,
  const std::vector<double> & default_value)
{
  if (!node->has_parameter(name)) {
    node->declare_parameter<std::vector<double>>(name, default_value);
  }
  return node->get_parameter(name).as_double_array();
}

int getIntParameter(
  const rclcpp::Node::SharedPtr & node,
  const std::string & name,
  int default_value)
{
  if (!node->has_parameter(name)) {
    node->declare_parameter<int>(name, default_value);
  }
  return static_cast<int>(node->get_parameter(name).as_int());
}

double getDoubleParameter(
  const rclcpp::Node::SharedPtr & node,
  const std::string & name,
  double default_value)
{
  if (!node->has_parameter(name)) {
    node->declare_parameter<double>(name, default_value);
  }
  return node->get_parameter(name).as_double();
}

struct RunResult
{
  std::string planner;
  int run_index{};
  bool success{};
  double planning_wall_time{};
  double trajectory_duration{};
  double joint_path_length{};
  double final_joint_error{};
  std::size_t point_count{};
};

struct Summary
{
  int runs{};
  int successes{};
  double planning_wall_time_sum{};
  double trajectory_duration_sum{};
  double joint_path_length_sum{};
  double point_count_sum{};
};

double durationToSeconds(const builtin_interfaces::msg::Duration & duration)
{
  return static_cast<double>(duration.sec) + static_cast<double>(duration.nanosec) * 1e-9;
}

double jointPathLength(const trajectory_msgs::msg::JointTrajectory & trajectory)
{
  const auto & points = trajectory.points;
  if (points.size() < 2) {
    return 0.0;
  }

  double length = 0.0;
  for (std::size_t i = 1; i < points.size(); ++i) {
    double squared_distance = 0.0;
    const auto & previous = points[i - 1].positions;
    const auto & current = points[i].positions;
    const auto joint_count = std::min(previous.size(), current.size());
    for (std::size_t joint_index = 0; joint_index < joint_count; ++joint_index) {
      const double delta = current[joint_index] - previous[joint_index];
      squared_distance += delta * delta;
    }
    length += std::sqrt(squared_distance);
  }
  return length;
}

double finalJointError(
  const trajectory_msgs::msg::JointTrajectory & trajectory,
  const std::map<std::string, double> & target)
{
  if (trajectory.points.empty()) {
    return 0.0;
  }

  const auto & final_positions = trajectory.points.back().positions;
  double squared_error = 0.0;
  for (std::size_t i = 0; i < trajectory.joint_names.size() && i < final_positions.size(); ++i) {
    const auto target_it = target.find(trajectory.joint_names[i]);
    if (target_it == target.end()) {
      continue;
    }
    const double error = final_positions[i] - target_it->second;
    squared_error += error * error;
  }
  return std::sqrt(squared_error);
}

std::string formatDouble(double value)
{
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(6) << value;
  return stream.str();
}

void writeCsv(const std::string & path, const std::vector<RunResult> & results)
{
  std::ofstream file(path);
  file << "planner,run,success,planning_wall_time_s,trajectory_duration_s,"
          "joint_path_length_rad,final_joint_error_rad,points\n";
  for (const auto & result : results) {
    file << result.planner << ',' << result.run_index << ',' << (result.success ? "true" : "false")
         << ',' << result.planning_wall_time << ',' << result.trajectory_duration << ','
         << result.joint_path_length << ',' << result.final_joint_error << ',' << result.point_count
         << '\n';
  }
}

std::string markdownTable(const std::map<std::string, Summary> & summaries)
{
  std::ostringstream table;
  table << "| Planner | Runs | Successes | Success rate | Avg planning time (s) | "
           "Avg trajectory duration (s) | Avg joint path length (rad) | Avg points |\n";
  table << "|---|---:|---:|---:|---:|---:|---:|---:|\n";

  for (const auto & [planner, summary] : summaries) {
    const double success_rate = summary.runs == 0 ? 0.0 :
      static_cast<double>(summary.successes) / static_cast<double>(summary.runs);

    const bool has_success = summary.successes > 0;
    table << "| " << planner << " | " << summary.runs << " | " << summary.successes << " | "
          << formatDouble(success_rate * 100.0) << "% | ";

    if (has_success) {
      table << formatDouble(summary.planning_wall_time_sum / summary.successes) << " | "
            << formatDouble(summary.trajectory_duration_sum / summary.successes) << " | "
            << formatDouble(summary.joint_path_length_sum / summary.successes) << " | "
            << formatDouble(summary.point_count_sum / summary.successes) << " |\n";
    } else {
      table << "- | - | - | - |\n";
    }
  }

  return table.str();
}

void writeMarkdown(
  const std::string & path,
  const std::map<std::string, Summary> & summaries,
  const std::map<std::string, double> & target,
  int runs)
{
  std::ofstream file(path);
  file << "# OMPL Planner Comparison\n\n";
  file << "Planning group: `arm`\n\n";
  file << "Repeated runs per planner: `" << runs << "`\n\n";
  file << "Target joint values:\n\n";
  for (const auto & [joint, value] : target) {
    file << "- `" << joint << "`: `" << value << "`\n";
  }
  file << "\n";
  file << markdownTable(summaries);
}
}  // namespace

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared(
    "planner_comparison",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  std::thread spinner([&executor]() { executor.spin(); });

  int return_code = 0;
  try {
    const std::string planning_group = getStringParameter(node, "planning_group", "arm");
    const std::vector<std::string> planners = getStringArrayParameter(
      node, "planners", {"RRTConnectkConfigDefault", "RRTkConfigDefault", "RRTstarkConfigDefault"});
    const std::vector<double> target_joint_values =
      getDoubleArrayParameter(node, "target_joint_values", {1.0, 0.6});
    const int runs = getIntParameter(node, "runs", 5);
    const double planning_time = getDoubleParameter(node, "planning_time", 5.0);
    const std::string output_prefix =
      getStringParameter(node, "output_prefix", "planner_comparison");

    moveit::planning_interface::MoveGroupInterface move_group(node, planning_group);
    move_group.setPlanningTime(planning_time);
    move_group.setNumPlanningAttempts(1);
    move_group.setMaxVelocityScalingFactor(0.3);
    move_group.setMaxAccelerationScalingFactor(0.3);

    const auto joint_names = move_group.getJointNames();
    if (target_joint_values.size() != joint_names.size()) {
      RCLCPP_ERROR(
        node->get_logger(),
        "target_joint_values has %zu entries, but planning group '%s' has %zu joints",
        target_joint_values.size(), planning_group.c_str(), joint_names.size());
      return_code = 2;
    } else {
      std::map<std::string, double> target;
      for (std::size_t i = 0; i < joint_names.size(); ++i) {
        target[joint_names[i]] = target_joint_values[i];
      }

      RCLCPP_INFO(node->get_logger(), "Planner comparison target:");
      for (const auto & [joint, value] : target) {
        RCLCPP_INFO(node->get_logger(), "  %s = %.6f", joint.c_str(), value);
      }

      std::vector<RunResult> results;
      std::map<std::string, Summary> summaries;

      for (const auto & planner : planners) {
        summaries[planner].runs = runs;
        move_group.setPlannerId(planner);

        for (int run_index = 1; run_index <= runs; ++run_index) {
          move_group.setStartStateToCurrentState();
          move_group.setJointValueTarget(target);

          moveit::planning_interface::MoveGroupInterface::Plan plan;
          const auto start_time = std::chrono::steady_clock::now();
          const auto planning_result = move_group.plan(plan);
          const auto end_time = std::chrono::steady_clock::now();

          RunResult result;
          result.planner = planner;
          result.run_index = run_index;
          result.planning_wall_time =
            std::chrono::duration<double>(end_time - start_time).count();
          result.success = planning_result == moveit::core::MoveItErrorCode::SUCCESS;

          if (result.success) {
            const auto & trajectory = plan.trajectory.joint_trajectory;
            result.point_count = trajectory.points.size();
            result.joint_path_length = jointPathLength(trajectory);
            result.final_joint_error = finalJointError(trajectory, target);
            if (!trajectory.points.empty()) {
              result.trajectory_duration = durationToSeconds(trajectory.points.back().time_from_start);
            }

            auto & summary = summaries[planner];
            summary.successes += 1;
            summary.planning_wall_time_sum += result.planning_wall_time;
            summary.trajectory_duration_sum += result.trajectory_duration;
            summary.joint_path_length_sum += result.joint_path_length;
            summary.point_count_sum += static_cast<double>(result.point_count);
          }

          results.push_back(result);
          RCLCPP_INFO(
            node->get_logger(),
            "[%s run %d/%d] success=%s planning_time=%.6fs points=%zu duration=%.6fs path=%.6f",
            planner.c_str(), run_index, runs, result.success ? "true" : "false",
            result.planning_wall_time, result.point_count, result.trajectory_duration,
            result.joint_path_length);
        }
      }

      const std::string csv_path = output_prefix + ".csv";
      const std::string markdown_path = output_prefix + ".md";
      writeCsv(csv_path, results);
      writeMarkdown(markdown_path, summaries, target, runs);

      std::cout << "\n" << markdownTable(summaries) << std::endl;
      RCLCPP_INFO(node->get_logger(), "Wrote %s", csv_path.c_str());
      RCLCPP_INFO(node->get_logger(), "Wrote %s", markdown_path.c_str());
    }
  } catch (const std::exception & error) {
    RCLCPP_ERROR(node->get_logger(), "Planner comparison failed: %s", error.what());
    return_code = 1;
  }

  executor.cancel();
  if (spinner.joinable()) {
    spinner.join();
  }
  rclcpp::shutdown();
  return return_code;
}
