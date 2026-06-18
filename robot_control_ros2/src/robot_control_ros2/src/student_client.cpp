#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/srv/student.hpp"

using Student = robot_control_ros2::srv::Student;

class ServiceClient : public rclcpp::Node
{
public:
    ServiceClient() : Node("service_client")
    {
        client_ = this->create_client<Student>("student_service");
        RCLCPP_INFO(this->get_logger(), "client finished create, waiting connecting....");
    }
    bool waitConnect()
    {
        while(!client_->wait_for_service(std::chrono::milliseconds(10)))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Exit!!");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "waiting connecting ...");
        }
        return true;
    }
    rclcpp::Client<Student>::FutureAndRequestId send_request(int32_t num1, int32_t num2)
    {
        std::shared_ptr<Student::Request> msg = std::make_shared<Student::Request>();
        msg->num1 = num1;
        msg->num2 = num2;
        msg->name = "hehe";
        return client_->async_send_request(msg);
    }
private:  
    rclcpp::Client<Student>::SharedPtr client_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    std::shared_ptr<ServiceClient> client_node = std::make_shared<ServiceClient>();
    bool flag = client_node->waitConnect();
    if (!flag)
    {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service connecting fail..");
        return 0;
    }
    rclcpp::Client<Student>::FutureAndRequestId response = client_node->send_request(1, 2);
    if (rclcpp::spin_until_future_complete(client_node, response) == rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(client_node->get_logger(),"请求正常处理");
        RCLCPP_INFO(client_node->get_logger(),"响应结果:%d!, ok? = %s", 
                response.get()->num, response.get()->is_ok ? "true" : "false");
        
    } else {
        RCLCPP_INFO(client_node->get_logger(),"请求异常");
    }
    rclcpp::shutdown();

    return 0;
}