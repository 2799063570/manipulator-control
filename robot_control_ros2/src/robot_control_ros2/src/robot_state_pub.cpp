#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/msg/joint_state_command.hpp"

class RobotStatePub : public rclcpp::Node
{
public:
    RobotStatePub() : Node("robot_state_pub")
    {
        pub_ = this->create_publisher<robot_control_ros2::msg::JointStateCommand>("robot_state", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(500), 
            std::bind(&RobotStatePub::time_callback, this));
    }
private:
    void time_callback()
    {
        robot_control_ros2::msg::JointStateCommand msg;
        msg.position = {1.0, 2.3, 3.1};
        msg.velocity = {2.8, 3.1, 3.3};
        msg.is_exc = true;
        RCLCPP_INFO(this->get_logger(), "pub msg : position[%.2f, %.2f, %.2f], velocity[%.2f, %.2f, %.2f], excution is %s",
            msg.position[0], msg.position[1], msg.position[2], msg.velocity[0], msg.velocity[1], msg.velocity[2],
            msg.is_exc ? "true" : "false");
        pub_->publish(msg);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<robot_control_ros2::msg::JointStateCommand>::SharedPtr pub_;
};
int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RobotStatePub>());
    rclcpp::shutdown();
    return 0;
}