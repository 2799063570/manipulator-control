#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/msg/joint_position_cmd.hpp"

using JointPositionCmd = robot_control_ros2::msg::JointPositionCmd;

class PositionCmdPub : public rclcpp::Node
{
public:
    PositionCmdPub() : Node("position_cmd_publisher"),
        position1(0.0), position2(0.0),
        add1(true), add2(true)
    {
        publisher_ = this->create_publisher<JointPositionCmd>("/joint_position_cmd", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(200), 
            [this]()
            {
                this->timer_callback();
            }
        );
    }
private:
    void timer_callback()
    {
        JointPositionCmd cmd;
        if (add1)
        {
            position1 += 0.2;
            if (position1 > 3.14)
            {
                position1 = 3.14; add1 = false;
            }
            
        }
        else
        {
            position1 -= 0.2;
            if (position1 < -3.14)
            {
                position1 = -3.14; add1 = true;
            }
        }

        if (add2)
        {
            position2 += 0.2;
            if (position2 > 1.57)
            {
                position2 = 1.57; add2 = false;
            }
        }
        else 
        {
            position2 -= 0.2;
            if (position2 < -1.57)
            {
                position1 = -1.57; add2 = true;
            }
        }
        cmd.position_cmd = {position1, position2};
        RCLCPP_INFO(this->get_logger(), "pub position{%.2f, %.2f}", position1, position2);
        publisher_->publish(cmd);
    }
    float position1, position2;
    bool add1, add2;
    rclcpp::Publisher<JointPositionCmd>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PositionCmdPub>());
    rclcpp::shutdown();
    return 0;
}