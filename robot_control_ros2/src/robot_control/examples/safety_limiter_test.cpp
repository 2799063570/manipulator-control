#include "safety_limiter.hpp"
#include "robot_state.hpp"

#include <iostream>

using namespace std;

int main()
{
    SafetyLimiter safer(6, 1.2);

    vector<double> u = {5.1, 5.1, 1.8, 1.4, 5.2, 13};
    vector<double> l = {-2.1, -5.1, -1.8, -1.4, -5.2, -13};
    vector<double> p = {1.1, 2.1, 0.5, 0.8, 5.0, 8.0};

    RobotState start_state(6), goal_state(6), current_state(6);
    start_state.setPosition(u);
    start_state.setVelocity(u);
    goal_state.setPosition(l);
    goal_state.setVelocity(l);
    current_state.setPosition(p);
    current_state.setVelocity(p);



    cout << "init low : ";
    start_state.printState();
    cout << "init upr : ";
    goal_state.printState();
    cout << "init maxVelocity : " << safer.maxVelocity() << endl;

    safer.setLowerLimits(l);
    safer.setUpperLimits(u);
    safer.setMaxVelocity(4.5);

    cout << "set low : ";
    printVector(safer.lowerLimits());
    cout << "set upr : ";
    printVector(safer.upperLimits());
    cout << "init maxVelocity : " << safer.maxVelocity() << endl;

    cout << "our robot state : ";
    current_state.printState();
    cout << "is safe " << (safer.isSafe(p, p) ? "ok" : "no") << endl;

    cout << "our robot state : ";
    current_state.printState();
    cout << "is safe " << (safer.isSafe(current_state) ? "ok" : "no") << endl;
    
    return 0;
}