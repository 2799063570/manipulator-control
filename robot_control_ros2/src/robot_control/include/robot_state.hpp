#ifndef ROBOT_STATE_HPP
#define ROBOT_STATE_HPP

#include <cstddef>
#include <vector>
#include <iostream>
#include <stdexcept>

class RobotState{
public:
    explicit RobotState(size_t dof = 6);// 构造函数 输入自由度 初始化变量
    void setPosition(const std::vector<double>& q); // 设置位置 使用顺序表
    void setVelocity(const std::vector<double>& dq);
    void setPosition(size_t index, double position);   // 设置速度 索引+值
    void setVelocity(size_t index, double velocity);
    const std::vector<double>& position() const;
    const std::vector<double>& velocity() const;
    
    size_t size() const;
    void printState() const;
    bool operator==(const RobotState& robotState) const;
    bool operator!=(const RobotState& robotState) const;

private:
    size_t dof_;
    std::vector<double> q_;
    std::vector<double> dq_;   
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