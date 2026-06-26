#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"


// 使用标准信息 一个浮点数的顺序表

class TimerControlNode : public rclcpp::Node
{
public:
    // TimerControlNode() : Node("TimerControlNode"), count_(0)
    TimerControlNode() : Node("vel_to_pos_node"), count_(0)
    {
        this->declare_parameter<int>("dof", 5);
        dof_ = this->get_parameter("dof").as_int();
        RCLCPP_INFO(this->get_logger(), "get dof : %d", dof_);
        pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("joint_postion_cmd", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), 
            std::bind(&TimerControlNode::timeCallBack, this));// 创建一个时间循环的线程
    }
private:
    void timeCallBack()
    {
        std_msgs::msg::Float64MultiArray msg;// 进入回调函数
        count_++;
        msg.data = {count_*0.01, count_*0.01, count_*0.01, count_*0.01, count_*0.01, count_*0.01};
        RCLCPP_INFO(this->get_logger(), "pub postion :  [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f]",
                msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5]);
        pub_->publish(msg);
    }
    size_t count_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr pub_;
    int dof_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TimerControlNode>());
    rclcpp::shutdown();
    return 0;
}