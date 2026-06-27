#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <turtlesim/msg/pose.hpp>

class MinimalDynamicFrameBroadcaster : public rclcpp::Node
{
public:
    MinimalDynamicFrameBroadcaster() : Node("dynamic_frame_broadcaster")
    {
        tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
        std::string topic_name = "/turtle1/pose";
        subscription_ = this->create_subscription<turtlesim::msg::Pose>(
            topic_name, 10, 
            std::bind(&MinimalDynamicFrameBroadcaster::handle_turtlesim_pose, this, 
            std::placeholders::_1));
    }
private:
    void handle_turtlesim_pose(const turtlesim::msg::Pose& msg)
    {
        RCLCPP_INFO(this->get_logger(), "get pose : xy[%.2f, %.2f], thera[%.2f]", msg.x, msg.y, msg.theta);
        geometry_msgs::msg::TransformStamped ts;
        rclcpp::Time now = this->get_clock()->now();

        ts.header.stamp = now;
        ts.header.frame_id = "world";
        ts.child_frame_id = "turtle1";

        ts.transform.translation.x = msg.x;
        ts.transform.translation.y = msg.y;
        ts.transform.translation.z = 0.0;

        tf2::Quaternion q;
        q.setRPY(0, 0, msg.theta);
        ts.transform.rotation.x = q.x();
        ts.transform.rotation.y = q.y();
        ts.transform.rotation.z = q.z();
        ts.transform.rotation.w = q.w();
        tf_broadcaster_->sendTransform(ts);
    }
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalDynamicFrameBroadcaster>());
    rclcpp::shutdown();
    return 0;

}