#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/msg/joint_velocity_cmd.hpp"

using JointVelocityCmd = robot_control_ros2::msg::JointVelocityCmd;

class JointVelocityPublisher : public rclcpp::Node
{
    public:
        JointVelocityPublisher() : Node("joint_velocity_publisher")
        {
            pub_ = this->create_publisher<JointVelocityCmd>("joint_velocity_cmd", 10);
            timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&JointVelocityPublisher::time_callback, this));
        }
    private:
        void time_callback()
        {
            JointVelocityCmd message;
            message.velocity_cmd = {0.1, 0.2, 0.3, 0.5, 0.1, 0.7};
            RCLCPP_INFO(this->get_logger(), "pub velocity : [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f]",
                message.velocity_cmd[0], message.velocity_cmd[1], message.velocity_cmd[2], 
                message.velocity_cmd[3], message.velocity_cmd[4], message.velocity_cmd[5]);
            pub_->publish(message);
        }
        rclcpp::Publisher<JointVelocityCmd>::SharedPtr pub_;
        rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JointVelocityPublisher>());
    rclcpp::shutdown();
    return 0;
}
