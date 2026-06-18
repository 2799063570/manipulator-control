#ifndef ROBOT_STATE
#define ROBOT_STATE

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

template<class T>
class RobotState{

public:
    explicit RobotState(size_t dof);

    void setPosition(const vector<T>& q);
    void setVelocity(const vector<T>& dq);
    const vector<T>& position();
    const vector<T>& velocity();
     
    size_t size();

private:
    vector<T> q_;
    vector<T> dq_;
    size_t dof_;
};


template<typename T>
RobotState<T>::RobotState(size_t dof) 
    : dof_(dof), q_(dof, T{}), dq_(dof, T{}) {}

template<typename T>
void RobotState<T>::setPosition(const vector<T>& q)
{
    if (q.size() != dof_)
        throw runtime_error("postion size does not match robot DOF.");
    q_ = q;
}
template<typename T>
void RobotState<T>::setVelocity(const vector<T>& dq)
{
    if (dq.size() != dof_)
        throw runtime_error("velocity size does not match robot DOF.");
    dq_ = dq;
}

template<typename T>
const vector<T>& RobotState<T>::position()
{
    return q_;
}
template<typename T>
const vector<T>& RobotState<T>::velocity()
{
    return dq_;
}
template<typename T>
size_t RobotState<T>::size()
{
    return dof_;
}

// template<typename T>
// void printVector(const vector<T>& v);
template<typename T>
void printVector(const vector<T>& v)
{
    for (auto value : v)
    {
        cout << value << " ";
    }cout << endl;
}

#endif