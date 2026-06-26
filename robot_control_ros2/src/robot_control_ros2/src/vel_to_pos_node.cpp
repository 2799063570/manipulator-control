#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/msg/joint_velocity_cmd.hpp"
#include "robot_control_ros2/msg/joint_position_cmd.hpp"
#include "robot_utils/robot_state.hpp"
#include "robot_utils/safety_limiter.hpp"
#include "robot_utils/cubic_interpolator.hpp"
#include "robot_utils/trajectory_buffer.hpp"
#include <vector>

using JointVelocityCmd = robot_control_ros2::msg::JointVelocityCmd;
using JointPositionCmd = robot_control_ros2::msg::JointPositionCmd;

class Vel2PosNode : public rclcpp::Node
{
    public:
        Vel2PosNode() : Node("Vel_to_Pos_node"), 
            dof_(this->declare_parameter<int>("dof", 6)),
            max_velocity_(this->declare_parameter<double>("max_velocity", 1.2)),
            dt_(this->declare_parameter<double>("dt", 0.02)),
            duration_(this->declare_parameter<double>("duration", 1)),
            control_rate_(this->declare_parameter<double>("control_rate", 50)),
            control_time_(1000/control_rate_),
            robot_state_start_(dof_), robot_state_goal_(dof_),
            safety_limiter_(dof_, max_velocity_),
            trajectory_buffer_(dof_),
            cubic_interpolator_(dof_, duration_, dt_)
        {

            std::vector<double> lower_limits_ = this->declare_parameter<std::vector<double>>(
                "lower_limits",
                std::vector<double>(dof_, -3.14)
            );
            std::vector<double> upper_limits_ = this->declare_parameter<std::vector<double>>(
                "upper_limits",
                std::vector<double>(dof_, 3.14)
            );
            safety_limiter_.setLowerLimits(lower_limits_);
            safety_limiter_.setUpperLimits(upper_limits_);

            trajectory_buffer_.clear();

            sub_ = this->create_subscription<JointVelocityCmd>("joint_velocity_cmd", 10, 
                std::bind(&Vel2PosNode::sub_callback, this, std::placeholders::_1));
            pub_ = this->create_publisher<JointPositionCmd>("joint_position_cmd", 10);

            timer_ = this->create_wall_timer(std::chrono::milliseconds(control_time_), 
                std::bind(&Vel2PosNode::timer_callback, this));
            RCLCPP_INFO(this->get_logger(), "Finish node init...");
        }
    private:
        void sub_callback(const JointVelocityCmd& vel)
        {
            std::vector<double> robot_velocity;
            std::vector<double> robot_position;
            for (int i = 0; i<dof_; i++)
            {
                robot_velocity.push_back(vel.velocity_cmd[i]);
                robot_position.push_back(robot_state_goal_.position()[i] + vel.velocity_cmd[i]*0.5*duration_);
            }
            

            robot_state_goal_.setVelocity(robot_velocity);
            robot_state_goal_.setPosition(robot_position);
            safety_limiter_.clampRobotState(robot_state_goal_);// 位置速度限幅
            RCLCPP_INFO(this->get_logger(), "get velocity cmd: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f]", 
                robot_state_goal_.velocity()[0], robot_state_goal_.velocity()[1], robot_state_goal_.velocity()[2], 
                robot_state_goal_.velocity()[3], robot_state_goal_.velocity()[4], robot_state_goal_.velocity()[5]);
            RCLCPP_INFO(this->get_logger(), "set position cmd: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f]", 
                robot_state_goal_.position()[0], robot_state_goal_.position()[1], robot_state_goal_.position()[2], 
                robot_state_goal_.position()[3], robot_state_goal_.position()[4], robot_state_goal_.position()[5]);
            
        }
        void timer_callback()
        {
            JointPositionCmd position_cmd;// 位置指令

            if (robot_state_goal_ != robot_state_start_)
            {
                cubic_interpolator_.setStateStart(robot_state_start_);
                cubic_interpolator_.setStateGoal(robot_state_goal_);
                cubic_interpolator_.calculate5Times();

                int trajectory_nums = cubic_interpolator_.getStateNum();
                std::vector<RobotState> trajectory = cubic_interpolator_.getTrajectory();
                for (int i  = 1; i < trajectory_nums; ++i)
                {
                    trajectory_buffer_.pushRobotState(trajectory[i]);
                }
            }
            
            if (trajectory_buffer_.hasNext())
            {
                RobotState robotState = trajectory_buffer_.popNext();
                for (int i = 0; i<dof_; i++)
                {           
                    position_cmd.position_cmd[i] = robotState.position()[i];
                }
                RCLCPP_INFO(this->get_logger(), "pub [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f] to position node", 
                    robotState.position()[0], robotState.position()[1], robotState.position()[2], 
                    robotState.position()[3], robotState.position()[4], robotState.position()[5]);
                pub_->publish(position_cmd);
                robot_state_start_ = robot_state_goal_;
            } 
        }

        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Subscription<JointVelocityCmd>::SharedPtr sub_;
        rclcpp::Publisher<JointPositionCmd>::SharedPtr pub_;
        
        int dof_;
        double max_velocity_;
        double dt_;
        double duration_;
        int control_rate_;
        int control_time_;      
        
        RobotState robot_state_start_, robot_state_goal_;
        SafetyLimiter safety_limiter_;
        TrajectoryBuffer trajectory_buffer_;
        CubicInterpolator cubic_interpolator_;
};
int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Vel2PosNode>());
    rclcpp::shutdown();
    return 0;
}