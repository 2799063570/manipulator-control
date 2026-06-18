#include "robot_state.hpp"
#include <iostream>

RobotState::RobotState(size_t dof) 
    : dof_(dof), q_(dof, double{}), dq_(dof, 0.0) {}

void RobotState::setPosition(const std::vector<double>& q)
{
    if (q.size() != dof_)
        throw std::runtime_error("position size does not match robot DOF.");
    q_ = q;
}

void RobotState::setVelocity(const std::vector<double>& dq)
{
    if (dq.size() != dof_)
        throw std::runtime_error("velocity size does not match robot DOF.");
    dq_ = dq;
}

const std::vector<double>& RobotState::position() const
{
    return q_;
}

const std::vector<double>& RobotState::velocity() const
{
    return dq_;
}

size_t RobotState::size() const
{
    return dof_;
}

void RobotState::printState() const
{
    std::cout << "Position: ";
    printVector(q_);
    std::cout << "Velocity: ";
    printVector(dq_);
}
