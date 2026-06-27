#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2/LinearMath/Quaternion.h"


class MinimalFrameListener : public rclcpp::Node
{
public:
    MinimalFrameListener() : Node("frame_listener")
    {
        tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        transform_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), 
            std::bind(&MinimalFrameListener::timer_callback, this));
    }
private:
    void timer_callback()
    {
        try{

            geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform("world", "turtle1", tf2::TimePointZero);
            RCLCPP_INFO(this->get_logger(), "get transform stamped");
            RCLCPP_INFO(this->get_logger(), "frame_id: %s", transform_stamped.header.frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "child_frame_id: %s", transform_stamped.child_frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "坐标: (%.2f, %.2f, %.2f)", 
                transform_stamped.transform.translation.x, 
                transform_stamped.transform.translation.y,
                transform_stamped.transform.translation.z);
        }
        catch(const tf2::LookupException& e)
        {
            RCLCPP_INFO(this->get_logger(), "坐标变换异常: %s", e.what());
        }
    }
    rclcpp::TimerBase::SharedPtr timer_;
    std::shared_ptr<tf2_ros::TransformListener> transform_listener_;
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalFrameListener>());
    rclcpp::shutdown();
    return 0;
}