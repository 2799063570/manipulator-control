#include <iostream>
#include <stdexcept>
#include <cmath>
#include "robot_utils/cubic_interpolator.hpp"

CubicInterpolator::CubicInterpolator(size_t dof, double duration, double dt)
    : num_(0),
      dof_(dof),
      robot_start_(dof),
      robot_goal_(dof),
      maxVelocity_(0.0),
      duration_(duration),
      dt_(dt) {}

void CubicInterpolator::calculate3Times()
{
    if (duration_ <= 0.0) throw std::runtime_error("duration must be positive");
    if (dt_ <= 0.0) throw std::runtime_error("dt must be positive");
    trajectory_.clear(); // 清除缓存的轨迹状态列表
    maxVelocity_ = 0.0;
    num_ = static_cast<int>(std::floor(duration_ / dt_)) + 1;// 计算点数
    trajectory_.reserve(num_); 
    std::vector<double> q_start_ = robot_start_.position();
    std::vector<double> q_goal_ = robot_goal_.position();
    // std::vector<double> dq_start_ = robot_start_.velocity();
    // std::vector<double> dq_goal_ = robot_goal_.velocity();
    for (int i = 0; i<num_; ++i)
    {
        double t = i * dt_;
        if (t > duration_) t = duration_;
        double tau = t / duration_;
        double s = 3*tau*tau - 2*tau*tau*tau;
        std::vector<double> q, dq;
        q.reserve(dof_);
        dq.reserve(dof_);   
        
        RobotState rs(dof_);
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
        rs.setPosition(q);
        rs.setVelocity(dq);
        trajectory_.push_back(rs);
    }
}
void computeQuinticCoeff(
    double q0,
    double v0,
    double a0,
    double qt,
    double vt,
    double at,
    double T,
    QuinticCoeff& coeff
)
{
    coeff.a0 = q0;
    coeff.a1 = v0;
    coeff.a2 = 0.5 * a0;
    double Dq = qt - (coeff.a0 + coeff.a1*T + coeff.a2*T*T);
    double Dv = vt - (coeff.a1 + 2*coeff.a2*T);
    double Da = at - (2 * coeff.a2);

    coeff.a3 = 10.0 * Dq / (T * T * T)
             - 4.0  * Dv / (T * T)
             + 0.5  * Da / T;

    coeff.a4 = -15.0 * Dq / (T * T * T * T)
             + 7.0   * Dv / (T * T * T)
             - Da / (T * T);

    coeff.a5 = 6.0 * Dq / (T * T * T * T * T)
             - 3.0 * Dv / (T * T * T * T)
             + 0.5 * Da / (T * T * T);
}
double computePosition(const QuinticCoeff& coeff, double t)
{
    return coeff.a0 + coeff.a1*t + coeff.a2*t*t + coeff.a3*t*t*t +
        coeff.a4*t*t*t*t + coeff.a5*t*t*t*t*t;
}
double computeVelocity(const QuinticCoeff& coeff, double t)
{
    return coeff.a1 + 2*coeff.a2*t + 3*coeff.a3*t*t 
        + 4*coeff.a4*t*t*t + 5*coeff.a5*t*t*t*t;
}
void CubicInterpolator::calculate5Times()
{
    if (duration_ <= 0.0) throw std::runtime_error("duration must be positive");
    if (dt_ <= 0.0) throw std::runtime_error("dt must be positive");
    trajectory_.clear(); // 清除缓存的轨迹状态列表
    maxVelocity_ = 0.0;
    num_ = static_cast<int>(std::floor(duration_ / dt_)) + 1;// 计算点数
    trajectory_.reserve(num_); 
    std::vector<double> q_start_ = robot_start_.position();
    std::vector<double> q_goal_ = robot_goal_.position();
    std::vector<double> dq_start_ = robot_start_.velocity();
    std::vector<double> dq_goal_ = robot_goal_.velocity();

    std::vector<QuinticCoeff> joints_coeff(dof_);
    for (int i = 0; i<dof_; i++)
    {
        computeQuinticCoeff(q_start_[i], dq_start_[i], 0.0,
            q_goal_[i], dq_goal_[i], 0.0, duration_, joints_coeff[i]);
    }


    for (int i = 0; i<num_; ++i)
    {
        double t = i * dt_;
        std::vector<double> q, dq;
        q.reserve(dof_);
        dq.reserve(dof_);   
        
        RobotState rs;
        for (size_t j = 0; j<dof_; j++)
        {
            double q_j = computePosition(joints_coeff[j], t);
            double dq_j = computeVelocity(joints_coeff[j], t);
            q.push_back(q_j); 
            dq.push_back(dq_j);

            if (std::abs(dq_j) > maxVelocity_)
            {
                maxVelocity_ = std::abs(dq_j);
            }
        }
        rs.setPosition(q);
        rs.setVelocity(dq);
        trajectory_.push_back(rs);
    }
}

void CubicInterpolator::setStateStart(const RobotState& q)
{
    if (q.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    robot_start_ = q;
}
void CubicInterpolator::setStateGoal(const RobotState& q)
{
    if (q.size() != dof_)
        throw std::runtime_error("robot DOF does not match");
    robot_goal_ = q;
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
const std::vector<RobotState>& CubicInterpolator::getTrajectory() const
{
    // if (index < 0 || index >= num_) throw std::runtime_error("index out of tras index");
    if (num_ <= 0) throw std::runtime_error("trajectory have not state");
    return trajectory_;
}
const RobotState& CubicInterpolator::getRobotState(size_t index) const
{
    if (index >= num_) throw std::runtime_error("index out of tras index");
    return trajectory_[index];
}
int CubicInterpolator::getTraNum() const
{
    return num_;
}
const RobotState& CubicInterpolator::getStateStart() const
{
    return robot_start_;
}
const RobotState& CubicInterpolator::getStateGoal() const
{
    return robot_goal_;
}
double CubicInterpolator::getMaxVelocity() const
{
    return maxVelocity_;
}