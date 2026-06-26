#include <iostream>
#include "cubic_interpolator.hpp"


int main()
{
    CubicInterpolator ci(6, 2, 0.2);        // dof duration dt
    std::cout << "init first state: ";
    ci.getStateStart().printState();


    std::cout << "init goal state: ";
    ci.getStateGoal().printState();

    std::vector<double> fp = {0, 2, 5, 2, 5, 8};
    std::vector<double> gp = {12, 8, 0, -8, -5, 58};
    std::vector<double> fv = {0, 2, 5, 2, 5, 8};
    std::vector<double> gv = {12, 8, 0, -8, -5, 58};

    RobotState start_state(6), goal_state(6);
    start_state.setPosition(fp);
    start_state.setVelocity(fv);
    goal_state.setPosition(gp);
    goal_state.setVelocity(gv);
    ci.setStateGoal(goal_state);
    ci.setStateStart(start_state);

    std::cout << "set first state position: ";
    ci.getStateStart().printState();
    std::cout << "set goal state position: ";
    ci.getStateGoal().printState();

    ci.calculate3Times();

    const std::vector<RobotState>& robot_vector = ci.getTrajectory();

    std::cout << "trans has " << ci.getStateNum() << std::endl;

    return 0;

}