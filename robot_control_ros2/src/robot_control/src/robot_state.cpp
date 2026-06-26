#include "robot_state.hpp"

RobotState::RobotState(size_t dof) 
    : dof_(dof), q_(dof, double{}), dq_(dof, double{}) {}

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

bool RobotState::operator==(const RobotState& robotState) const
{
    if (this->dof_ != robotState.size()) return false;
    for (size_t i = 0; i<dof_; i++)
    {
        if (robotState.position()[i] != this->position()[i]) return false;
        if (robotState.velocity()[i] != this->velocity()[i]) return false;
    }
    return true;
}
bool RobotState::operator!=(const RobotState& robotState) const
{
    if (this->dof_ != robotState.size()) return true;
    for (size_t i = 0; i<dof_; i++)
    {
        if (robotState.position()[i] != this->position()[i]) return true;
        if (robotState.velocity()[i] != this->velocity()[i]) return true;
    }
    return false;
}
void RobotState::setPosition(size_t index, double position)
{
    if (index > dof_) throw std::runtime_error("index over the joint dof");
    this->q_[index] = position;
}
void RobotState::setVelocity(size_t index, double velocity)
{
    if (index > dof_) throw std::runtime_error("index over the joint dof");
    this->dq_[index] = velocity;
}