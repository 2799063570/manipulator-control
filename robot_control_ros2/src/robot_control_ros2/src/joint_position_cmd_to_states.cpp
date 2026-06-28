#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "robot_control_ros2/msg/joint_position_cmd.hpp"
#include "robot_state.hpp"
#include "trajectory_buffer.hpp"
#include "cubic_interpolator.hpp"

using JointPositionCmd = robot_control_ros2::msg::JointPositionCmd;

class Position2States : public rclcpp::Node
{
public:
    Position2States(size_t dof = 2) : Node("position_to_states"), 
        dof_(dof), control_time_(50), max_a_(2.0),
        buffer_(dof), last_state_(dof),
        cubic_interpolator_(dof, 0.2, 0.05)
    {
        last_state_.setPosition(std::vector<double>(dof, double{}));
        last_state_.setVelocity(std::vector<double>(dof, double{}));
        publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);
        subscriber_ = this->create_subscription<JointPositionCmd>("/joint_position_cmd", 10,
            [this](const JointPositionCmd& msg)
            {
                this->position_cmd_callback(msg);
            }
        );
        timer_ = this->create_wall_timer(std::chrono::milliseconds(control_time_),
            [this](){
                this->timer_callback();
            }
        );
    }
private:
    void position_cmd_callback(const JointPositionCmd& msg)
    {
        const std::vector<double> q = last_state_.position();
        const std::vector<double> dq = last_state_.velocity();
        std::vector<double> position(dof_), velocity(dof_);
        double dt = static_cast<double>(control_time_)/1000;
        for (size_t i = 0; i<dof_; i++)
        {
            double desired_velocity = (msg.position_cmd[i] - q[i]) / dt;

            double min_velocity = dq[i] - max_a_ * dt;
            double max_velocity = dq[i] + max_a_ * dt;

            velocity[i] = std::clamp(desired_velocity, min_velocity, max_velocity);
            position[i] = q[i] + velocity[i] * dt;
        }
        RobotState current_state(dof_);
        current_state.setPosition(position);
        current_state.setVelocity(velocity);
        cubic_interpolator_.setStateStart(last_state_);
        cubic_interpolator_.setStateGoal(current_state);
        cubic_interpolator_.calculate3Times();
        int trajectory_num = cubic_interpolator_.getStateNum();
        for (int i = 1; i<trajectory_num; i++)
        {
            buffer_.pushRobotState(cubic_interpolator_.getRobotState(i));
        } 
        last_state_ = current_state;
    }
    void timer_callback()
    {
        if (!buffer_.hasNext())
            return;

        RobotState state = buffer_.popNext();

        if (state.position().size() != dof_){
            RCLCPP_ERROR(this->get_logger(), "dof is not match");
            return;
        }
            
        sensor_msgs::msg::JointState joint_state;
        joint_state.header.stamp = this->get_clock()->now();
        joint_state.name = {"joint1", "joint2"};
        joint_state.position = state.position();
        joint_state.velocity = state.velocity();
        
        RCLCPP_INFO(this->get_logger(), "pub position{%.2f, %.2f}, velocity{%.2f, %.2f}",
            state.position()[0], state.position()[1],
            state.velocity()[0], state.velocity()[1]);

        publisher_->publish(joint_state);
    }

    size_t dof_;
    int control_time_;
    double max_a_;
    TrajectoryBuffer buffer_;
    RobotState last_state_; 
    CubicInterpolator cubic_interpolator_;

    rclcpp::Subscription<JointPositionCmd>::SharedPtr subscriber_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Position2States>());
    rclcpp::shutdown();
    return 0;
}