#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

class JointVelocitySubscriber : public rclcpp::Node
{
public:
    JointVelocitySubscriber() : Node("joint_velocity_subscriber")
    {
        sub_ = this->create_subscription<std_msgs::msg::Float64MultiArray>("topic", 10, 
                    std::bind(&JointVelocitySubscriber::sub_callback, this, std::placeholders::_1));
    }
private:
    void sub_callback(const std_msgs::msg::Float64MultiArray& message)
    {
        RCLCPP_INFO(this->get_logger(), "get velocity : [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f]",
            message.data[0], message.data[1], message.data[2], message.data[3], message.data[4], message.data[5]);
    }
    rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr sub_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JointVelocitySubscriber>());
    rclcpp::shutdown();

    return 0;
}