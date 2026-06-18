#include "safetyLimiter.hpp"
#include <iostream>
#include <stdexcept>


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

const std::vector<double>& SafetyLimiter::lowerLimites() const
{
    return low_;
}

const std::vector<double>& SafetyLimiter::upperLimites() const
{
    return upr_;
}

double SafetyLimiter::maxVelocity() const
{
    return maxVelocity_;
}

bool SafetyLimiter::is_safety(const std::vector<double>& positon, double vel) const
{
    if (positon.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    bool posIsSafety = true;
    bool velIsSafety = (vel <= maxVelocity_);
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