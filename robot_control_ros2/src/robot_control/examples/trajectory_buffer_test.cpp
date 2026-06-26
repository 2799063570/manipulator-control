#include "trajectory_buffer.hpp"
#include "robot_state.hpp"

#include <iostream>
using namespace std;


int main()
{
    TrajectoryBuffer tb(2);
    RobotState start_state(2), goal_state(2), current_state(2);

    vector<double> sv = {1.1, 2.2}, sp = {2.0, 1.1};
    vector<double> gv = {2.0, 7.8}, gp = {1.1, 7.8};
    vector<double> cv = {0.2, 0.3}, cp = {1.2, 1.2};
    start_state.setPosition(sp);
    start_state.setVelocity(sv);
    goal_state.setPosition(gp);
    goal_state.setVelocity(gv);
    current_state.setPosition(cp);
    current_state.setVelocity(cv);

    tb.pushRobotState(start_state);
    cout << "trajectory buffer size is " << tb.size() << endl;
    if (tb.hasNext()) {
        cout << "has nest is :" << endl;
        RobotState state = tb.popNext();
        state.printState();
    }
    tb.pushRobotState(current_state); tb.pushRobotState(goal_state);
    cout << "trajectory buffer size is " << tb.size() << endl;
    if (tb.hasNext()) {
        cout << "has nest is :" << endl;
        RobotState state = tb.popNext();
        state.printState();
    }
    return 0;
}