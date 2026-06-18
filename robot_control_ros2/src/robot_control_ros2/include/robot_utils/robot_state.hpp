#ifndef ROBOT_STATE_HPP
#define ROBOT_STATE_HPP

#include <cstddef>
#include <vector>
#include <iostream>
#include <stdexcept>

class RobotState{
public:
    explicit RobotState(size_t dof = 6);
    void setPosition(const std::vector<double>& q);
    void setVelocity(const std::vector<double>& dq);
    const std::vector<double>& position() const;
    const std::vector<double>& velocity() const;
    void setJointPosition(size_t index, double position);
    void setJointVelocity(size_t index, double velocity);
    size_t size() const;
    void printState() const;
    bool operator==(const RobotState& robotState) const;
    bool operator!=(const RobotState& robotState) const;

private:
    std::vector<double> q_;
    std::vector<double> dq_;
    size_t dof_;
};

template<typename T>
void printVector(const std::vector<T>& v)
{
    for (auto value : v)
    {
        std::cout << value << " ";
    }std::cout << std::endl;
}

#endif