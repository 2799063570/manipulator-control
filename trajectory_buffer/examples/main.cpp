#include "trajectory_buffer.hpp"
#include <iostream>
using namespace std;

template<typename T>
void printVector(const vector<T>& v)
{
    for (auto value : v)
    {
        cout << value << " ";
    }cout << endl;
}
int main()
{
    TrajectoryBuffer tb;
    TrajectoryPoint p1, p2;
    p1.time = 1.2; p2.time = 1.4;
    p1.position = {1.1, 2.4, 2.2}; p2.position = {2.0, 1.1, 7.8};
    p1.velocity = {8.0, 0.1, -4.2}; p2.velocity = {5.1, 55, -89};
    tb.pushTrajectory(p1);
    cout << "trajectory buffer size is " << tb.size() << endl;
    if (tb.hasNext()) {
        cout << "has nest is :" << endl;
        TrajectoryPoint point = tb.popNext();
        printVector(point.position);
        printVector(point.velocity);
    }
    tb.pushTrajectory(p2); tb.pushTrajectory(p1);
    cout << "trajectory buffer size is " << tb.size() << endl;
    if (tb.hasNext()) {
        cout << "has nest is :" << endl;
        TrajectoryPoint point = tb.popNext();
        printVector(point.position);
        printVector(point.velocity);
    }
    return 0;
}