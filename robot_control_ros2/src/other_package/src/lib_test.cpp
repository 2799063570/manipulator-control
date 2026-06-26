#include "robot_state.hpp"
#include "safety_limiter.hpp"
#include "trajectory_buffer.hpp"

#include <iostream>

using namespace std;

int main()
{
    SafetyLimiter limiter(6, 1.0);
    limiter.setLowerLimits({-3.14, -3.14, -3.14, -3.14, -3.14, -3.14});
    limiter.setUpperLimits({3.14, 3.14, 3.14, 3.14, 3.14, 3.14});
    limiter.setMaxVelocity(2.0);

    vector<double> q_start = {0, 0, 0, 0, 0, 0};
    vector<double> q_goal = {1, 15, -1, 1.5, 6, 21};
    RobotState start_state(6), goal_state(6);
    start_state.setPosition(q_start);
    goal_state.setPosition(q_goal);

    TrajectoryBuffer buffer(6);
    buffer.pushRobotState(start_state);
    buffer.pushRobotState(goal_state);
    

    while (buffer.hasNext())
    {
        RobotState state = buffer.popNext();
        if (limiter.isSafe(state))
        {
            state.printState();
        }
        else
        {
            cout << "Trajectory point is not safe. Stopping execution." << endl;
            limiter.clampRobotState(state);
            state.printState();
        }
    }
}