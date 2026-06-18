#include "rclcpp/rclcpp.hpp"
#include "robot_control_ros2/srv/student.hpp"

using Student = robot_control_ros2::srv::Student;

class Servicer : public rclcpp::Node
{
public:
    Servicer() : Node("Service_node")
    {
        server_ = this->create_service<Student>("student_service", 
            std::bind(&Servicer::serviceCallback, this, std::placeholders::_1, std::placeholders::_2));
        RCLCPP_INFO(this->get_logger(), "start get client...");
    }
private:
    void serviceCallback(const Student::Request::SharedPtr& req, const Student::Response::SharedPtr& res)
    {
        res->num = req->num1 + req->num2;
        res->is_ok = true;
        RCLCPP_INFO(this->get_logger(), "get service request : %d + %d = %d, name : %s, ok? = %s",
            req->num1, req->num2, res->num, req->name.c_str(), 
            res->is_ok ? "true" : "false");
    }
    // rclcpp::Timer_Base::SharedPtr timer_;
    rclcpp::Service<Student>::SharedPtr server_;
};



int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Servicer>());

    rclcpp::shutdown();

    return 0;
}