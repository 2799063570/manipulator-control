#ifndef JACOBIAN_SOLVER_HPP
#define JACOBIAN_SOLVER_HPP

#include <Eigen/Dense>

class JacobianSolver{

public:
    Eigen::VectorXd solvePseudoInverse(const Eigen::MatrixXd& J, const Eigen::VectorXd& b) const;
    Eigen::VectorXd solveDampedLeastSquares(const Eigen::MatrixXd& J, const Eigen::VectorXd& b, double lambda) const;
    double conditionNumber(const Eigen::MatrixXd& J, double tolerance = 1e-5) const;
private:
    Eigen::MatrixXd pseudoInverse(const Eigen::MatrixXd& J, double tolerance = 1e-6) const;

};

#endif