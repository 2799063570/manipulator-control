#ifndef CUBIC_INTERPOLATOR_HPP
#define CUBIC_INTERPOLATOR_HPP

#include <vector>
#include <cstddef>

struct TrajectoryPoint{
    double time;
    std::vector<double> position;
    std::vector<double> velocity;
};

class CubicInterpolator
{
public:
    explicit CubicInterpolator(size_t dof, double duration, double dt);

    void setQstart(const std::vector<double>& q);
    void setQgoal(const std::vector<double>& q);
    void setDuration(double duration);
    void setDt(double dt);
    const std::vector<TrajectoryPoint>& getTras() const;
    const TrajectoryPoint& getTrasPoint(size_t index) const;
    int getTraNum() const;
    const std::vector<double>& getFirstPos() const;
    const std::vector<double>& getGoalPos() const;
    double getMaxVelocity() const;
    const std::vector<TrajectoryPoint>& calculateTras();

private:
    int num_;
    size_t dof_;
    std::vector<double> q_start_;
    std::vector<double> q_goal_;
    double maxVelocity_;
    double duration_;
    double dt_;
    // std::vector<std::vector<double>> tras_q_;
    // std::vector<std::vector<double>> tras_dq_;
    std::vector<TrajectoryPoint> tras_;
};

#endif