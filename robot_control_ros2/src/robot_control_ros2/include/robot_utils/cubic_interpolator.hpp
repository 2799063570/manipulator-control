#ifndef CUBIC_INTERPOLATOR_HPP
#define CUBIC_INTERPOLATOR_HPP

#include <vector>
#include <cstddef>
#include "robot_utils/robot_state.hpp"

struct QuinticCoeff
{
    double a0;
    double a1;
    double a2;
    double a3;
    double a4;
    double a5;
};

class CubicInterpolator
{
public:
    explicit CubicInterpolator(size_t dof, double duration, double dt);

    void setStateStart(const RobotState& start_state);
    void setStateGoal(const RobotState& goal_state);
    void setDuration(double duration);
    void setDt(double dt);
    const std::vector<RobotState>& getTrajectory() const;
    const RobotState& getRobotState(size_t index) const;
    int getTraNum() const;
    const RobotState& getStateStart() const;
    const RobotState& getStateGoal() const;
    double getMaxVelocity() const;
    void calculate3Times();
    void calculate5Times();

private:
    size_t num_;
    size_t dof_;
    RobotState robot_start_;
    RobotState robot_goal_;
    double maxVelocity_;
    double duration_;
    double dt_;
    std::vector<RobotState> trajectory_;
};

#endif