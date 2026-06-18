#include "robot_state.hpp"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> q = {12, 51, 84, 65, 32, 5};
    vector<int> dq = {21, 23, 34, 22, 5, 34};

    RobotState<int> robot(6);
    robot.setPosition(q);
    robot.setVelocity(dq);

    cout << "robot DOF = " << robot.size() << endl;

    cout << "robot position: ";
    printVector(robot.position());

    cout << "robot velocity: ";
    printVector(robot.velocity());

    vector<double> q1 = {12.2, 5.1, 8.4, 6.5, 3.2, 5.45};
    vector<double> dq1 = {21.111, 52.23, 3.04, 21.2, 5.45, 1.34};

    RobotState<double> robot1(6);
    robot1.setPosition(q1);
    robot1.setVelocity(dq1);

    cout << "robot DOF = " << robot1.size() << endl;

    cout << "robot position: ";
    printVector(robot1.position());

    cout << "robot velocity: ";
    printVector(robot1.velocity());

    return 0;
}