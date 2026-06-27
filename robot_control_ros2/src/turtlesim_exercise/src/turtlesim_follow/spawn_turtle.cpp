#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

class SpawnNode : public rclcpp::Node
{
public:
    SpawnNode() : Node("spwan_node")
    {
        this->declare_parameter("x", 1.0);// 设置这些参数的默认参数 外部没有参数的话使用这里的默认参数
        this->declare_parameter("y", 0.5);
        this->declare_parameter("theta", 0.0);
        this->declare_parameter("turtle_name", "turtle2");
        
        x = this->get_parameter("x").as_double();
        y = this->get_parameter("y").as_double();
        theta = this->get_parameter("theta").as_double();
        turtle_name = this->get_parameter("turtle_name").as_string();

        client_ = this->create_client<turtlesim::srv::Spawn>("/spawn");
    }
    bool connect_server()
    {
        while(!client_->wait_for_service(std::chrono::milliseconds(100)))
        {
            if(!rclcpp::ok())
            {
                RCLCPP_INFO(this->get_logger(), "user cancel!");
                return false;
            }RCLCPP_INFO(this->get_logger(), "waitting service connecting....");
        }
        return true;
    }
    rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId spawn_turtle()
    {
        auto turtle = std::make_shared<turtlesim::srv::Spawn::Request>();
        turtle->x = x;
        turtle->y = y;
        turtle->theta = theta;
        turtle->name = turtle_name;
        return client_->async_send_request(turtle);
    }

private:
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr client_;
    float_t x, y, theta;
    std::string turtle_name;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    std::shared_ptr<SpawnNode> node = std::make_shared<SpawnNode>();
    bool flag = node->connect_server();
    if (flag)
    {
        auto response = node->spawn_turtle();
        if (rclcpp::spin_until_future_complete(node,response) == rclcpp::FutureReturnCode::SUCCESS)
        {
            RCLCPP_INFO(node->get_logger(),"请求正常处理");
            std::string name = response.get()->name;
            if (name.empty())
            {
                RCLCPP_INFO(node->get_logger(),"乌龟重名导致生成失败！");
            } else {
                RCLCPP_INFO(node->get_logger(),"乌龟%s生成成功！", name.c_str());
            }
        }
        else {
            RCLCPP_INFO(node->get_logger(),"请求异常");
        }
    }else
    {
        RCLCPP_INFO(node->get_logger(),"服务连接失败!");
    }

    rclcpp::shutdown();
    return 0;
}   
