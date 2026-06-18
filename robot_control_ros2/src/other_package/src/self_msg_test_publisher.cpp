#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/msg/joint_state_command.hpp"


class SelfMsgPublisher : public rclcpp::Node
{
public:
    SelfMsgPublisher() : Node("self_node_publisher")
    {
        pub_ = this->create_publisher<robot_control_ros2::msg::JointStateCommand>("robot_state_command", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), 
            std::bind(&SelfMsgPublisher::timer_callback, this));
    }
private:
    void timer_callback()
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
    rclcpp::spin(std::make_shared<SelfMsgPublisher>());
    rclcpp::shutdown();

    return 0;
}