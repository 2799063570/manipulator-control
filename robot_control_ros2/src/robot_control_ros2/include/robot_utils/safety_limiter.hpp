#ifndef SAFETY_LIMITER_HPP
#define SAFETY_LIMITER_HPP

#include <vector>
#include <cstddef>
#include "robot_utils/robot_state.hpp"

class SafetyLimiter{

public:
    explicit SafetyLimiter(size_t dof, double maxVelocity);

    void setLowerLimits(const std::vector<double>& low);
    void setUpperLimits(const std::vector<double>& upr);
    void setMaxVelocity(double v);
    const std::vector<double>& lowerLimits() const;
    const std::vector<double>& upperLimits() const;
    double maxVelocity() const; 
    size_t size() const;
    bool isSafe(const std::vector<double>& positon, const std::vector<double>& vel) const;
    std::vector<double> clampPosition(const std::vector<double>& position) const;
    std::vector<double> clampVelocity(const std::vector<double>& velocity) const;
    void clampRobotState(RobotState& robot_state);

private:
    size_t dof_;
    std::vector<double> low_;
    std::vector<double> upr_;
    double maxVelocity_;
};

#endif 
