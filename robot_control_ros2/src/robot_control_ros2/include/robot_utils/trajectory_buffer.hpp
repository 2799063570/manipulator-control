#ifndef TRAJECTORY_BUFFER_HPP
#define TRAJECTORY_BUFFER_HPP


#include <vector>
#include <cstddef>
#include <deque>
#include "robot_utils/robot_state.hpp"

class TrajectoryBuffer{

public:
    explicit TrajectoryBuffer(size_t dof);
    void pushRobotState(const RobotState& robot_state);
    bool hasNext() const;
    RobotState popNext();
    std::size_t size() const;
    void clear();

private:
    std::deque<RobotState> buffer_;
    size_t dof_;
};

#endif