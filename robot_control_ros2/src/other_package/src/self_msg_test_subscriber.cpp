#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/msg/joint_state_command.hpp"

class SelfMsgSubscriber : public rclcpp::Node
{
public:
    SelfMsgSubscriber() : Node("self_msg_subscriber")
    {
        sub_ = this->create_subscription<robot_control_ros2::msg::JointStateCommand>("robot_state_command", 10, 
            std::bind(&SelfMsgSubscriber::sub_callback, this, std::placeholders::_1));
    }
private:
    rclcpp::Subscription<robot_control_ros2::msg::JointStateCommand>::SharedPtr sub_;
    void sub_callback(const robot_control_ros2::msg::JointStateCommand& msg)
    {
        RCLCPP_INFO(this->get_logger(), "get msg : position[%.2f, %.2f, %.2f], velocity[%.2f, %.2f, %.2f], excution is %s",
            msg.position[0], msg.position[1], msg.position[2], msg.velocity[0], msg.velocity[1], msg.velocity[2],
            msg.is_exc ? "true" : "false");
    }
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SelfMsgSubscriber>());
    rclcpp::shutdown();
    return 0;
}