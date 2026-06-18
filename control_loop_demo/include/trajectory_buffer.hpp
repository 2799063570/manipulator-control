#ifndef TRAJECTORY_BUFFER_HPP
#define TRAJECTORY_BUFFER_HPP


#include <vector>
#include <cstddef>
#include <deque>
#include "cubic_interpolator.hpp"

// struct TrajectoryPoint{
//     double time;
//     std::vector<double> position;
//     std::vector<double> velocity;
// };

class TrajectoryBuffer{

public:
    // TrajectoryBuffer(std::deque<TrajectoryPoint>& buffer);
    void pushTrajectory(const TrajectoryPoint& tra_point);
    bool hasNext() const;
    TrajectoryPoint popNext();
    std::size_t size() const;
    void clear();

private:
    std::deque<TrajectoryPoint> buffer_;
};

#endif