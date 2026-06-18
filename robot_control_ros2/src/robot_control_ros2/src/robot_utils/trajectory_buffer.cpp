#include "robot_utils/trajectory_buffer.hpp"
#include <stdexcept>

TrajectoryBuffer::TrajectoryBuffer(size_t dof) : dof_(dof) {  }

void TrajectoryBuffer::pushRobotState(const RobotState& robot_state)
{
    if (robot_state.size() != dof_) throw std::runtime_error("robot state dof not match buffer dof set");
    buffer_.push_back(robot_state);
}
bool TrajectoryBuffer::hasNext() const
{
    return !buffer_.empty();
}
RobotState TrajectoryBuffer::popNext()
{
    if (!hasNext()) throw std::runtime_error("trajectory has not point");
    RobotState p = buffer_.front();
    buffer_.pop_front();
    return p;
}
std::size_t TrajectoryBuffer::size() const
{
    return buffer_.size();
}
void TrajectoryBuffer::clear()
{
    buffer_.clear();
}

