#include "rclcpp/rclcpp.hpp"

/*
    总结一下参数服务器的设置方法
    标准流程是需要先声明(declare_parameter)再修改(declare_parameter)
    主要有4个方法 declare set get has undeclare(descriptor 动态参数才可以删除 声明的时候)
    1\ this->declare_parameter("dof", 10);
    2\ rclcpp::Parameter dof = this->get_parameter("dof");
        相应的处理 int dof_int = dof.as_int();
    3\ this->set_parameter(rclcpp::Parameter("dof", 40));
    4\ bool has = this->has_parameter("dof");
    5\ this->undeclare_parameter("dof");
*/


class MinimalParamServer : public rclcpp::Node
{
public:
    MinimalParamServer() : Node("param_server")
    {
        rcl_interfaces::msg::ParameterDescriptor descriptor;
        descriptor.dynamic_typing = true;
        this->declare_parameter("car_type", "tiger");
        this->declare_parameter("height", 1.90);
        this->declare_parameter("dof", 10, descriptor);
        rclcpp::Parameter dof = this->get_parameter("dof");
        // or 
        // rclcpp::Parameter dof;
        // bool dof_is_ok = this->get_parameter("dof", dof); // 传入引用
        int dof_int = dof.as_int();
        RCLCPP_INFO(this->get_logger(), "dof = %d", dof_int);
        this->set_parameter(rclcpp::Parameter("dof", 40));
        RCLCPP_INFO(this->get_logger(), "dof = %ld", this->get_parameter("dof").as_int());
        RCLCPP_INFO(this->get_logger(), "has dof? %s", this->has_parameter("dof") ? "yes" : "no");
        this->undeclare_parameter("dof");
        RCLCPP_INFO(this->get_logger(), "has dof? %s", this->has_parameter("dof") ? "yes" : "no");
    }
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalParamServer>());
    rclcpp::shutdown();

    return 0;
}