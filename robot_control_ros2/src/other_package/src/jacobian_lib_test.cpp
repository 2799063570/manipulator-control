#include "jacobian_solver.hpp"

#include <iostream>
#include <Eigen/Dense>

using namespace std;

int main()
{
    JacobianSolver solver;

    Eigen::MatrixXd J(2, 3);
    J << 1, 0, 1,
         0, 1, 1;
    
    Eigen::VectorXd b(2);
    b << 1, 2;

    Eigen::VectorXd x_pinv = solver.solvePseudoInverse(J, b);
    Eigen::VectorXd x_dls = solver.solveDampedLeastSquares(J, b, 0.1);

    std::cout << "Pseudo inverse solution:\n" << x_pinv << std::endl;
    std::cout << "DLS solution:\n" << x_dls << std::endl;
    std::cout << "Condition number: " << solver.conditionNumber(J) << std::endl;

    return 0;
}