#include "trajectory_buffer.hpp"
#include <stdexcept>

// TrajectoryBuffer::TrajectoryBuffer(std::deque<TrajectoryPoint>& buffer)
//     : buffer_(buffer) {}

void TrajectoryBuffer::pushTrajectory(const TrajectoryPoint& tra_point)
{
    buffer_.push_back(tra_point);
}
bool TrajectoryBuffer::hasNext() const
{
    return !buffer_.empty();
}
TrajectoryPoint TrajectoryBuffer::popNext()
{
    if (!hasNext()) throw std::runtime_error("trajectory has not point");
    TrajectoryPoint p = buffer_.front();
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

