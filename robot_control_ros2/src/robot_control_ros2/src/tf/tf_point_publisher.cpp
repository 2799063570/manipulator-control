#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"


class PointPublisher : public rclcpp::Node
{
public:
    PointPublisher() : Node("point_publisher"), count_(0)
    {
        point_pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), 
            std::bind(&PointPublisher::timer_back, this));
    }

private:
    void timer_back()
    {
        geometry_msgs::msg::PointStamped point;
        point.header.stamp = this->now();
        point.header.frame_id = "base_link";
        point.point.x = 0.01;
        point.point.y = 0.01 * (count_++);
        point.point.z = 0.01 * count_;
        point_pub_->publish(point);
        RCLCPP_INFO(this->get_logger(), "frame id: %s, xyz: {%.2f, %.2f, %.2f}",
            point.header.frame_id.c_str(), point.point.x, point.point.y, point.point.z);
    }
    rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    int count_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PointPublisher>());
    rclcpp::shutdown();
    return 0;
}