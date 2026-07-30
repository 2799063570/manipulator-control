#ifndef CUBIC_INTERPOLATOR_HPP
#define CUBIC_INTERPOLATOR_HPP

#include <vector>
#include <cstddef>
#include "robot_state.hpp"

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

    void setStateStart(const RobotState& start_state);// 设置初始状态
    void setStateGoal(const RobotState& goal_state);// 设置目标状态
    void setDuration(double duration);// 设置持续时间
    void setDt(double dt);// 设置时间间隔
    const std::vector<RobotState>& getTrajectory() const;
    const RobotState& getRobotState(size_t index) const;
    int getStateNum() const;
    const RobotState& getStateStart() const;
    const RobotState& getStateGoal() const;
    double getMaxVelocity() const;
    void calculate3Times();
    void calculate5Times();

private:
    size_t num_;        // 轨迹点数目
    size_t dof_;        // 自由度
    RobotState robot_start_;
    RobotState robot_goal_;
    double maxVelocity_;
    double duration_;
    double dt_;
    std::vector<RobotState> trajectory_;
};

#endif