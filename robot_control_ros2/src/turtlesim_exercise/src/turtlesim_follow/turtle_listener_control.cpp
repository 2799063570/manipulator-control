#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2/LinearMath/Quaternion.h"
#include "geometry_msgs/msg/twist.hpp"


class FollowTurtleControl : public rclcpp::Node
{
public:
    FollowTurtleControl() : Node("follow_turtle_contorl_node")
    {
        this->declare_parameter("target_frame", "turtle2");
        this->declare_parameter("source_frame", "turtle1");
        target_frame = this->get_parameter("target_frame").as_string();
        source_frame = this->get_parameter("source_frame").as_string();
        control_topic = target_frame + "/cmd_vel";

        buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        twist_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(control_topic, 10);
        transform_listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(50), 
            [this]()
            {
                this->timer_callback();
            });
    }
private:
    void timer_callback()
    {
        geometry_msgs::msg::TransformStamped transformStamped;
        try
        {
            transformStamped = buffer_->lookupTransform(target_frame, source_frame, tf2::TimePointZero);
        }
        catch(const tf2::LookupException& e)
        {
            RCLCPP_INFO(this->get_logger(),"坐标变换异常：%s",e.what());
            return;
        }
        geometry_msgs::msg::Twist cmd;
        static const double scaleRotationRate = 1.0;
        cmd.angular.z = scaleRotationRate * atan2(
        transformStamped.transform.translation.y,
        transformStamped.transform.translation.x);

        static const double scaleForwardSpeed = 0.5;
        cmd.linear.x = scaleForwardSpeed * sqrt(
            pow(transformStamped.transform.translation.x, 2) +
            pow(transformStamped.transform.translation.y, 2));

        twist_pub_->publish(cmd);
        
    }
    std::shared_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> transform_listener_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr twist_pub_;
    std::string target_frame, source_frame, control_topic;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FollowTurtleControl>());
    rclcpp::shutdown();
    return 0;

}