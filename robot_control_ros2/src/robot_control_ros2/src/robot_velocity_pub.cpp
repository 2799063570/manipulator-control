#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/msg/joint_velocity_cmd.hpp"
#include <cstddef>
// #include "robot_state.hpp"

using JointVelocityCMD = robot_control_ros2::msg::JointVelocityCmd;


class VelocityCmdPub : public rclcpp::Node
{
public:
    VelocityCmdPub() : Node("velocity_cmd_pub"),
        dof_(this->declare_parameter<int>("dof", 6)),
        count_(0),
        pub_time_(this->declare_parameter<double>("velocity_cmd_time", 0.1)*1000)
    {
        publisher_ = this->create_publisher<JointVelocityCMD>("joint_velocity_cmd", 10);

        timer_ = this->create_wall_timer(std::chrono::milliseconds(pub_time_), 
                std::bind(&VelocityCmdPub::timer_callback, this));
    }

private:
    void timer_callback()
    {
        JointVelocityCMD cmd;
        for (std::size_t i = 0; i<dof_; ++i)
        {
            cmd.velocity_cmd[i] = 0.01 * count_;
        }
        
        count_++;
        RCLCPP_INFO(this->get_logger(), "pub velocity cmd [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f]",
            cmd.velocity_cmd[0], cmd.velocity_cmd[1], cmd.velocity_cmd[2], cmd.velocity_cmd[3],
            cmd.velocity_cmd[4], cmd.velocity_cmd[5]);
        publisher_->publish(cmd);
    }
    std::size_t dof_;
    int count_;
    int pub_time_;
    rclcpp::Publisher<JointVelocityCMD>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<VelocityCmdPub>());
    rclcpp::shutdown();
    return 0;

}
