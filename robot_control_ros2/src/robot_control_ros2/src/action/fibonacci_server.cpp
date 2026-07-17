#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/action/fibonacci.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using Fibonacci = robot_control_ros2::action::Fibonacci;
using GoalHandle = rclcpp_action::ServerGoalHandle<Fibonacci>;

class ActionServer : public rclcpp::Node
{
public:
    ActionServer() : Node("action_server")
    {
        server_ = rclcpp_action::create_server<Fibonacci>(
            this, "fibonacci_action",
            [this](const rclcpp_action::GoalUUID& uuid, 
                std::shared_ptr<const Fibonacci::Goal> goal)
            { return this->handle_goal(uuid, goal); },

            [this](const std::shared_ptr<GoalHandle> goal_handle)
            { return this->handle_cancel(goal_handle); },

            [this](const std::shared_ptr<GoalHandle> goal_handle)
            { this->handle_accept(goal_handle); }
        );
    }
private:
    rclcpp_action::Server<Fibonacci>::SharedPtr server_;
    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID& uuid, 
        std::shared_ptr<const Fibonacci::Goal> goal)
    {
        RCLCPP_INFO(this->get_logger(), "get goal order = %d", goal->order);
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandle> goal_handle)
    {
        RCLCPP_INFO(this->get_logger(), "get cancel request");
        return rclcpp_action::CancelResponse::ACCEPT;  
    }
    void handle_accept(const std::shared_ptr<GoalHandle> goal_handle)
    {
        std::thread(&ActionServer::execute, this, goal_handle).detach();
    }

    void execute(const std::shared_ptr<GoalHandle> goal_handle)
    {
        auto feedback = std::make_shared<Fibonacci::Feedback>();
        auto result = std::make_shared<Fibonacci::Result>();
        int order = goal_handle->get_goal()->order;
        for (int i = 0; i<order; i++)
        {
            if (goal_handle->is_canceling())
            {
                RCLCPP_INFO(this->get_logger(), "任务取消");
                goal_handle->canceled(result);
                return;
            }
            feedback->partial_sequence.push_back(i);

            goal_handle->publish_feedback(feedback);
            RCLCPP_INFO(this->get_logger(), "发送feedback");

            std::this_thread::sleep_for(
                std::chrono::seconds(1)
            );
        }
        result->sequence =
            feedback->partial_sequence;
        goal_handle->succeed(result);
        RCLCPP_INFO(
            get_logger(),
            "任务完成"
        );
    }
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ActionServer>());
    rclcpp::shutdown();
    
    return 0;
}