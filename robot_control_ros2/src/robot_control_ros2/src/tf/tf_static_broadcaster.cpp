#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/static_transform_broadcaster.h>


class MinimalStaticFrameBroadcaster : public rclcpp::Node
{
public:
    explicit MinimalStaticFrameBroadcaster(const char* const cmd[]) : Node("static_frame_broadcaster")
    {
        tf_publisher_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        this->make_transforms(cmd);
    }

private:

    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_publisher_;

    void make_transforms(const char* const cmd[])
    {
        geometry_msgs::msg::TransformStamped ts;

        rclcpp::Time now = this->get_clock()->now();
        ts.header.stamp = now;
        ts.header.frame_id = cmd[7];
        ts.child_frame_id = cmd[8];

        ts.transform.translation.x = std::atof(cmd[1]);
        ts.transform.translation.y = std::atof(cmd[2]);
        ts.transform.translation.z = std::atof(cmd[3]);

        tf2::Quaternion q;
        q.setRPY(
            std::atof(cmd[4]),
            std::atof(cmd[5]),
            std::atof(cmd[6])
        );
        ts.transform.rotation.x = q.x();
        ts.transform.rotation.y = q.y();
        ts.transform.rotation.z = q.z();
        ts.transform.rotation.w = q.w();

        tf_publisher_->sendTransform(ts);
        RCLCPP_INFO(this->get_logger(), "finished transform send");
    }
};

int main(int argc, char* argv[])
{
    bool is_legal = true;
    rclcpp::init(argc, argv);
    auto args = rclcpp::remove_ros_arguments(argc, argv);
    if (args.size() != 9)
    {
        RCLCPP_INFO(
            rclcpp::get_logger("logger"),
            "输入参数有问题！运行程序时请按照：x y z roll pitch yaw frame_id child_frame_id 的格式传入参数"
        );
        RCLCPP_INFO(
            rclcpp::get_logger("logger"),
            "输入参数个数：%d", argc
        );
        is_legal = false;
    }
    const char* cmd[9] = {"_", "1", "1", "1", "1", "1", "1", "base_link", "laser"}; 

    rclcpp::spin(std::make_shared<MinimalStaticFrameBroadcaster>(is_legal ? argv : cmd));
    rclcpp::shutdown();
    return 0;

}