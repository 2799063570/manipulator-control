#include <iostream>
#include <stdexcept>
#include <cmath>
#include "cubic_interpolator.hpp"

CubicInterpolator::CubicInterpolator(size_t dof, double duration, double dt)
    : num_(0),
      dof_(dof),
      q_start_(dof, 0.0),
      q_goal_(dof, 0.0),
      maxVelocity_(0.0),
      duration_(duration),
      dt_(dt) {}

const std::vector<TrajectoryPoint>& CubicInterpolator::calculateTras()
{
    if (duration_ <= 0.0) throw std::runtime_error("duration must be positive");
    if (dt_ <= 0.0) throw std::runtime_error("dt must be positive");
    
    maxVelocity_ = 0.0;
    num_ = static_cast<int>(std::floor(duration_ / dt_)) + 1;
    tras_.clear();tras_.reserve(num_);
    for (int i = 0; i<num_; ++i)
    {
        double t = i * dt_;
        if (t > duration_) t = duration_;
        double tau = t / duration_;
        double s = 3*tau*tau - 2*tau*tau*tau;
        std::vector<double> q, dq;
        q.reserve(dof_);
        dq.reserve(dof_);   
        
        TrajectoryPoint pose;
        pose.time = t;
        for (size_t j = 0; j<dof_; j++)
        {
            double dq_j = ((6.0 * tau - 6.0 * tau * tau) / duration_) 
              * (q_goal_[j] - q_start_[j]);
            double q_j = q_start_[j] + s * (q_goal_[j] - q_start_[j]);
            q.push_back(q_j); 
            dq.push_back(dq_j);

            if (std::abs(dq_j) > maxVelocity_)
            {
                maxVelocity_ = std::abs(dq_j);
            }
        }
        pose.position = q;
        pose.velocity = dq;
        tras_.push_back(pose);
    }
    return tras_;
}

void CubicInterpolator::setQstart(const std::vector<double>& q)
{
    if (q.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    q_start_ = q;
}
void CubicInterpolator::setQgoal(const std::vector<double>& q)
{
    if (q.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    q_goal_ = q;
}
void CubicInterpolator::setDuration(double duration)
{
    if (duration <= 0.0) throw std::runtime_error("duration must be positive");
    duration_ = duration;
}
void CubicInterpolator::setDt(double dt)
{
    if (dt <= 0.0) throw std::runtime_error("dt must be positive");
    dt_ = dt;
}
const std::vector<TrajectoryPoint>& CubicInterpolator::getTras() const
{
    // if (index < 0 || index >= num_) throw std::runtime_error("index out of tras index");
    return tras_;
}
const TrajectoryPoint& CubicInterpolator::getTrasPoint(size_t index) const
{
    if (index >= num_) throw std::runtime_error("index out of tras index");
    return tras_[index];
}
int CubicInterpolator::getTraNum() const
{
    return num_;
}
const std::vector<double>& CubicInterpolator::getFirstPos() const
{
    return q_start_;
}
const std::vector<double>& CubicInterpolator::getGoalPos() const
{
    return q_goal_;
}
double CubicInterpolator::getMaxVelocity() const
{
    return maxVelocity_;
}