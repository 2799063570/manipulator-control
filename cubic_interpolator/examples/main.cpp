#include <iostream>
#include "cubic_interpolator.hpp"

template<typename T>
void printVector(const std::vector<T>& v)
{
    for (auto value : v)
    {
        std::cout << value << " ";
    }std::cout << std::endl;
}

int main()
{
    CubicInterpolator ci(6, 2, 0.2);
    std::cout << "init first state: ";
    printVector(ci.getFirstPos());

    std::cout << "init goal state: ";
    printVector(ci.getGoalPos());

    std::vector<double> fp = {0, 2, 5, 2, 5, 8};
    std::vector<double> gp = {12, 8, 0, -8, -5, 58};

    ci.setQstart(fp);
    ci.setQgoal(gp);

    std::cout << "set first state: ";
    printVector(ci.getFirstPos());

    std::cout << "set goal state: ";
    printVector(ci.getGoalPos());

    const std::vector<TrajectoryPoint>& tras = ci.calculateTras();

    std::cout << "trans has " << ci.getTraNum() << std::endl;

    std::cout << "test trans frist position is ";
    printVector(tras[1].position);
    printVector(ci.getTras()[1].position);
    printVector(ci.getTrasPoint(1).position);
    std::cout << "test trans frist velocity is ";
    printVector(tras[1].velocity);
    printVector(ci.getTras()[1].velocity);
    printVector(ci.getTrasPoint(1).velocity);

    return 0;

}