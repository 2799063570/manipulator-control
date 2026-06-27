#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/point_stamped.h>
#include <message_filters/subscriber.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/create_timer_ros.h>
#include <tf2_ros/message_filter.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

class MessageFilterPointListener : public rclcpp::Node
{
public:
    MessageFilterPointListener() : Node("message_filter")
    {
        target_frame_ = "base_link";
        tf2_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        auto timer_interface = std::make_shared<tf2_ros::CreateTimerROS>(
            this->get_node_base_interface(),
            this->get_node_timers_interface()
        );
        tf2_buffer_->setCreateTimerInterface(timer_interface);

        tf2_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf2_buffer_);
        point_sub_.subscribe(this, "point");
        tf2_filter_ = std::make_shared<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>>(
            point_sub_, *tf2_buffer_, target_frame_, 100, this->get_node_logging_interface(),
            this->get_node_clock_interface(), std::chrono::seconds(1)
        );
        tf2_filter_->registerCallback(&MessageFilterPointListener::msgCallback, this);
    }
private:
    void msgCallback(const geometry_msgs::msg::PointStamped::SharedPtr point_ptr)
    {
        geometry_msgs::msg::PointStamped point_out;
        try{
            tf2_buffer_->transform(*point_ptr, point_out, target_frame_);
            RCLCPP_INFO(
                this->get_logger(), "坐标点相对于base_link的坐标:(%.2f,%.2f,%.2f)",
                point_out.point.x,
                point_out.point.y,
                point_out.point.z);
        }catch (tf2::TransformException & ex) {
            RCLCPP_WARN(
                // Print exception which was caught
                this->get_logger(), "Failure %s\n", ex.what());
        }
    }
    std::string target_frame_;
    std::shared_ptr<tf2_ros::Buffer> tf2_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf2_listener_;
    message_filters::Subscriber<geometry_msgs::msg::PointStamped> point_sub_;
    std::shared_ptr<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>> tf2_filter_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MessageFilterPointListener>());
    rclcpp::shutdown();
    return 0;
}