#include "robot_state.hpp"
#include <iostream>

using namespace std;

int main()
{
    RobotState rs(2);

    rs.setPosition({1.1, 1.2});
    rs.setVelocity({1.2, -1.2});

    rs.printState();

    return 0;
}
