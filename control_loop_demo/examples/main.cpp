#include "robot_state.hpp"
#include "safety_limiter.hpp"
#include "cubic_interpolator.hpp"
#include "trajectory_buffer.hpp"
#include <iostream>

using namespace std;

int main()
{
    RobotState robot(6);
    SafetyLimiter limiter(6, 1.0);
    limiter.setLowerLimits({-3.14, -3.14, -3.14, -3.14, -3.14, -3.14});
    limiter.setUpperLimits({3.14, 3.14, 3.14, 3.14, 3.14, 3.14});
    limiter.setMaxVelocity(2.0);

    vector<double> q_start = {0, 0, 0, 0, 0, 0};
    vector<double> q_goal = {1, 15, -1, 1.5, 6, 21};

    robot.setPosition(q_start);


    CubicInterpolator interpolator(6, 5.0, 0.04);// 6 DOF, 5s duration, 0.04s time step
    interpolator.setQstart(q_start);
    interpolator.setQgoal(q_goal);
    auto trajectory = interpolator.calculateTras();
    TrajectoryBuffer buffer;
    for (const auto& point : trajectory)
    {
        buffer.pushTrajectory(point);
    }
    

    while (buffer.hasNext())
    {
        TrajectoryPoint point = buffer.popNext();
        if (limiter.is_safety(point.position, point.velocity))
        {
            robot.setPosition(point.position);
            robot.setVelocity(point.velocity);
            robot.printState();
        }
        else
        {
            cout << "Trajectory point is not safe. Stopping execution." << endl;
            point.position = limiter.clampPosition(point.position);
            point.velocity = limiter.clampVelocity(point.velocity);
            robot.setPosition(point.position);
            robot.setVelocity(point.velocity);
            robot.printState();
        }
    }


}