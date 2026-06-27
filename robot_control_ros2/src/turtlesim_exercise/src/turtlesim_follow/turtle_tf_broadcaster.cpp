#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"
#include "turtlesim/msg/pose.hpp"


class FrameBroadcaster : public rclcpp::Node
{
public:
    FrameBroadcaster() : Node("frame_broadcaster_node")
    {
        this->declare_parameter("turtle_name", "turtle1");
        turtle_name_ = this->get_parameter("turtle_name").as_string();
        turtle_topic_ = turtle_name_ + "/pose";
        broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

        // subscriber_ = this->create_subscription<turtlesim::msg::Pose>(turtle_topic_, 10, 
        //     std::bind(&FrameBroadcaster::handle_turtle_pose, this, std::placeholders::_1));
        subscriber_ = this->create_subscription<turtlesim::msg::Pose>(turtle_topic_, 10, 
            [this](const turtlesim::msg::Pose& msg)
            {
                this->handle_turtle_pose(msg);
            });
        RCLCPP_INFO(this->get_logger(), "init broadcaster from world to %s", turtle_name_.c_str());
    }
private:
    void handle_turtle_pose(const turtlesim::msg::Pose& msg)
    {
        geometry_msgs::msg::TransformStamped t;
        t.header.stamp = this->get_clock()->now();
        t.header.frame_id = "world";
        t.child_frame_id = turtle_name_;
        t.transform.translation.x = msg.x;
        t.transform.translation.y = msg.y;
        t.transform.translation.z = 0.0;
        tf2::Quaternion q;
        q.setRPY(0,0,msg.theta);
        t.transform.rotation.x = q.x();
        t.transform.rotation.y = q.y();
        t.transform.rotation.z = q.z();
        t.transform.rotation.w = q.w();
        broadcaster_->sendTransform(t);
        RCLCPP_INFO(this->get_logger(), "get %s to world transform : xy{%.2f, %.2f}, theta{%.2f}",
            turtle_name_.c_str(), msg.x, msg.y, msg.theta);
    }

    std::string turtle_name_, turtle_topic_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscriber_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FrameBroadcaster>());
    rclcpp::shutdown();
    return 0;
}