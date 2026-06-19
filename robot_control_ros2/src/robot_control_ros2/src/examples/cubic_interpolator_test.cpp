#include "rclcpp/rclcpp.hpp"
#include "robot_utils/cubic_interpolator.hpp"
#include "robot_utils/robot_state.hpp"


class Interploator : public rclcpp::Node
{
public:
    Interploator() : Node("interploator"), dof_(7), interploator_(dof_, 1, 0.1)
    {
        RCLCPP_INFO(this->get_logger(), "finish node init!");
    }
    void compulate3tra()
    {
        RobotState robot_start(dof_), robot_goal(dof_);
        std::vector<double> startQ = {0, 0, 0, 0, 0, 0, 0};
        std::vector<double> goalQ = {1.2, 1.2, 1.2, 1.4, 2.4, 6.3, 1};
        std::vector<double> startDQ = {2.1, 1.0, 4.0, 1.0, 6.0, 1.30, 0};
        std::vector<double> goalDQ = {0.2, 1.12, 1.12, 1.14, 2.14, 1.3, 0};
        robot_start.setPosition(startQ);
        robot_start.setVelocity(startDQ);
        robot_goal.setPosition(goalQ);
        robot_goal.setVelocity(goalDQ);

        interploator_.setStateStart(robot_start);
        interploator_.setStateGoal(robot_goal);
        interploator_.calculate3Times();

        int num = interploator_.getTraNum();
        RCLCPP_INFO(this->get_logger(), "get %d points", num);
        std::vector<RobotState> robotStates = interploator_.getTrajectory();
        for (int i = 0; i<num; i++)
        {
            RCLCPP_INFO(this->get_logger(), "point%d position： 【%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f】", i+1, 
                robotStates[i].position()[0], robotStates[i].position()[1], robotStates[i].position()[2],
                robotStates[i].position()[3], robotStates[i].position()[4], robotStates[i].position()[5], robotStates[i].position()[6]);
            RCLCPP_INFO(this->get_logger(), "point%d velocity： 【%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f】", i+1, 
                robotStates[i].velocity()[0], robotStates[i].velocity()[1], robotStates[i].velocity()[2],
                robotStates[i].velocity()[3], robotStates[i].velocity()[4], robotStates[i].velocity()[5], robotStates[i].position()[6]);
        }

    }
    void compulate5tra()
    {
        RobotState robot_start(dof_), robot_goal(dof_);
        std::vector<double> startQ = {0, 0, 0, 0, 0, 0};
        std::vector<double> goalQ = {1.2, 1.2, 1.2, 1.4, 2.4, 6.3};
        std::vector<double> startDQ = {2.1, 1.0, 4.0, 1.0, 6.0, 1.30};
        std::vector<double> goalDQ = {0.2, 1.12, 1.12, 1.14, 2.14, 1.3};
        robot_start.setPosition(startQ);
        robot_start.setVelocity(startDQ);
        robot_goal.setPosition(goalQ);
        robot_goal.setVelocity(goalDQ);

        interploator_.setStateStart(robot_start);
        interploator_.setStateGoal(robot_goal);
        interploator_.calculate5Times();

        int num = interploator_.getTraNum();
        RCLCPP_INFO(this->get_logger(), "get %d points", num);
        std::vector<RobotState> robotStates = interploator_.getTrajectory();
        for (int i = 0; i<num; i++)
        {
            RCLCPP_INFO(this->get_logger(), "point%d position： 【%.2f, %.2f, %.2f, %.2f, %.2f, %.2f】", i+1, 
                robotStates[i].position()[0], robotStates[i].position()[1], robotStates[i].position()[2],
                robotStates[i].position()[3], robotStates[i].position()[4], robotStates[i].position()[5]);
            RCLCPP_INFO(this->get_logger(), "point%d velocity： 【%.2f, %.2f, %.2f, %.2f, %.2f, %.2f】", i+1, 
                robotStates[i].velocity()[0], robotStates[i].velocity()[1], robotStates[i].velocity()[2],
                robotStates[i].velocity()[3], robotStates[i].velocity()[4], robotStates[i].velocity()[5]);
        }

    }
private:
    int dof_;
    CubicInterpolator interploator_;

};
int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    Interploator i;
    i.compulate3tra();
    // i.compulate5tra();

    rclcpp::shutdown();
    return 0;

}