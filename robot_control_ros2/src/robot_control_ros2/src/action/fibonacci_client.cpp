#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_control_ros2/action/fibonacci.hpp"


using Fibonacci = robot_control_ros2::action::Fibonacci;
using GoalHandle = rclcpp_action::Client<Fibonacci>::GoalHandle;

static int feedback_index  = 0;

class ActionClient : public rclcpp::Node
{
public:
    ActionClient() : Node("action_client")
    {
        client_ = rclcpp_action::create_client<Fibonacci>(this, "fibonacci_action");
    }
    void send_goal()
    {
        if(!client_->wait_for_action_server())
        {
            RCLCPP_ERROR(get_logger(), "Action Server不存在");
            return;
        }
        auto goal = Fibonacci::Goal();
        goal.order = 10;

        auto options = rclcpp_action::Client<Fibonacci>::SendGoalOptions();
        options.feedback_callback =
            [](auto goal_handle, auto feedback)
            {
                RCLCPP_INFO(rclcpp::get_logger("client"), "收到feedback -- %d", feedback->partial_sequence[feedback_index++]);
            };
        
        options.result_callback =
            [](const auto &result)
            {
                RCLCPP_INFO(rclcpp::get_logger("client"), "任务结束 result = %d", result.result->sequence[--feedback_index]);
            };
        options.goal_response_callback = 
            [this](GoalHandle::SharedPtr goal_handle)
            {
                if(!goal_handle)
                {
                    RCLCPP_ERROR(get_logger(), "Goal rejected");
                    return;
                }
                RCLCPP_INFO(get_logger(), "Goal accepted");
                this->goal_handle_ = goal_handle;
            };
        
        client_->async_send_goal(goal, options);
    }
    void cancel_goal()
    {

        if(!goal_handle_)
        {
            RCLCPP_WARN(get_logger(), "没有正在执行的Goal");
            return;
        }
        client_->async_cancel_goal(goal_handle_, [](auto response)
            { RCLCPP_INFO(rclcpp::get_logger("client"), "cancel response received");}
        );
    }

private:
    rclcpp_action::Client<Fibonacci>::SharedPtr client_;
    GoalHandle::SharedPtr goal_handle_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ActionClient>();
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    std::thread spinner([&executor]()
    {
       executor.spin(); 
    });
    node->send_goal();

    sleep(6);
    node->cancel_goal();
    sleep(1);
    executor.cancel();

    if (spinner.joinable())
    {
        spinner.join();
    }
    return 0;
}