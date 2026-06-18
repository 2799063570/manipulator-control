#include "robot_utils/safety_limiter.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

SafetyLimiter::SafetyLimiter(size_t dof, double maxVelocity)
    : dof_(dof), maxVelocity_(maxVelocity), low_(dof, -3.14), upr_(dof, 3.14) {}

void SafetyLimiter::setLowerLimits(const std::vector<double>& low)
{
    if (low.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    low_ = low;
}

void SafetyLimiter::setUpperLimits(const std::vector<double>& upr)
{
    if (upr.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    upr_ = upr;
}

void SafetyLimiter::setMaxVelocity(double vel)
{
    maxVelocity_ = vel;
}

const std::vector<double>& SafetyLimiter::lowerLimits() const
{
    return low_;
}

const std::vector<double>& SafetyLimiter::upperLimits() const
{
    return upr_;
}

double SafetyLimiter::maxVelocity() const
{
    return maxVelocity_;
}

bool SafetyLimiter::isSafe(const std::vector<double>& positon, const std::vector<double>& vel) const
{
    if (positon.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    bool posIsSafety = true;
    bool velIsSafety = true;
    for (int i = 0; i < dof_; i++)
    {
        if (vel[i] < -maxVelocity_ || vel[i] > maxVelocity_)
        {
            velIsSafety = false;
            break;
        }
    }
    for (int i = 0; i<dof_; i++)
    {
        if (positon[i] < low_[i] || positon[i] > upr_[i])
        {
            posIsSafety = false;
            break;
        }
    }

    if (!posIsSafety) std::cout << "position over" << std::endl;
    if (!velIsSafety) std::cout << "velocity over" << std::endl;
    return posIsSafety&&velIsSafety;
}

std::vector<double> SafetyLimiter::clampPosition(const std::vector<double>& position) const
{
    if (position.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    std::vector<double> clampedPosition = position;
    for (int i = 0; i < dof_; i++)
    {
        if (clampedPosition[i] < low_[i])
            clampedPosition[i] = low_[i];
        else if (clampedPosition[i] > upr_[i])
            clampedPosition[i] = upr_[i];
    }
    return clampedPosition;
}

std::vector<double> SafetyLimiter::clampVelocity(const std::vector<double>& velocity) const
{
    if (velocity.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    std::vector<double> clampedVelocity = velocity;
    for (int i = 0; i < dof_; i++)
    {
        if (clampedVelocity[i] < -maxVelocity_)
            clampedVelocity[i] = -maxVelocity_;
        else if (clampedVelocity[i] > maxVelocity_)
            clampedVelocity[i] = maxVelocity_;
    }
    return clampedVelocity;
}

void SafetyLimiter::clampRobotState(RobotState& robot_state)
{
    if (robot_state.size() != dof_) throw std::runtime_error("robot DOF does not match");
    std::vector<double> clampedVelocity = robot_state.velocity();
    std::vector<double> clampedPosition = robot_state.position();
    for (int i = 0; i<dof_; i++)
    {
        if (clampedVelocity[i] < -maxVelocity_)
            robot_state.setJointVelocity(i, -maxVelocity_);
        else if (clampedVelocity[i] > maxVelocity_)
            robot_state.setJointVelocity(i, maxVelocity_);
        
        if (clampedPosition[i] < low_[i])
            robot_state.setJointPosition(i, low_[i]);
        else if (clampedPosition[i] > upr_[i])
            robot_state.setJointPosition(i, upr_[i]);
    }
}