#ifndef ROBOT_STATE
#define ROBOT_STATE

#include <cstddef>
#include <vector>
#include <iostream>

class RobotState{
public:
    explicit RobotState(size_t dof);
    void setPosition(const std::vector<double>& q);
    void setVelocity(const std::vector<double>& dq);
    const std::vector<double>& position() const;
    const std::vector<double>& velocity() const;
    size_t size() const;
    void printState() const;

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
