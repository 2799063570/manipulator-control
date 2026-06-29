#include "rclcpp/rclcpp.hpp"

/*
    获取参数服务器中的数据
    这里主要借助于 rclcpp::SyncParametersClient 对应对方节点的名称
    用法和server中的相似
    需要注意的get需要显式说明数据类型同时可以加入默认值
    1/ std::string car_type =
        client_->get_parameter<std::string>("car_type", "unknown");
*/
class ParamClient : public rclcpp::Node
{
public:
    ParamClient() : Node("param_client")
    {
        client_ = std::make_shared<rclcpp::SyncParametersClient>(this, "param_server");
    }
    bool connect_server()
    {
        while(!client_->wait_for_service(std::chrono::seconds(1)))
        {
            if (!rclcpp::ok())
                return false;
            RCLCPP_INFO(this->get_logger(), "waitting service connecting...");
        }
        return true;
    }
    void param_test()
    {
        RCLCPP_INFO(this->get_logger(), "car_type = %s", client_->get_parameter<std::string>("car_type").c_str());

        client_->set_parameters({rclcpp::Parameter("car_type", "new")});
        RCLCPP_INFO(this->get_logger(), "car_type = %s", client_->get_parameter<std::string>("car_type").c_str());

        RCLCPP_INFO(this->get_logger(), "height = %.2f", client_->get_parameter<double>("height"));
        RCLCPP_INFO(this->get_logger(), "has height? %s", client_->has_parameter("height") ? "ok" : "no");

        auto params = client_->get_parameters({"car_type", "height"});
        RCLCPP_INFO(this->get_logger(), "%s = %s", params[0].get_name().c_str(), params[0].value_to_string().c_str());
        RCLCPP_INFO(this->get_logger(), "%s = %.2f", params[1].get_name().c_str(), params[1].as_double());

        client_->set_parameters({
            rclcpp::Parameter("car_type", "1111"),
            rclcpp::Parameter("height", 2.2)
        });
        auto params_2 = client_->get_parameters({"car_type", "height"});
        RCLCPP_INFO(this->get_logger(), "%s = %s", params[0].get_name().c_str(), params_2[0].value_to_string().c_str());
        RCLCPP_INFO(this->get_logger(), "%s = %.2f", params[1].get_name().c_str(), params_2[1].as_double());
    }
private:
    rclcpp::SyncParametersClient::SharedPtr client_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto param_client = std::make_shared<ParamClient>();
    if (param_client->connect_server())
    {
        param_client->param_test();
    }
    rclcpp::shutdown();
    return 0;
}