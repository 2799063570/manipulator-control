#include "jacobian_solver.hpp"
#include <stdexcept>
#include <limits>

Eigen::MatrixXd JacobianSolver::pseudoInverse(const Eigen::MatrixXd& J, double tolerance) const
{
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(J, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd singularValues = svd.singularValues();
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd V = svd.matrixV();

    const int r = singularValues.size();

    Eigen::MatrixXd S_inv = Eigen::MatrixXd::Zero(r, r);  // 对角矩阵 默认对角线元素为0
    for (int i = 0; i < singularValues.size(); ++i) {
        if (singularValues(i) > tolerance) {
            S_inv(i, i) = 1.0 / singularValues(i);
        }
    }
    return V * S_inv * U.transpose();
}
Eigen::VectorXd JacobianSolver::solvePseudoInverse(const Eigen::MatrixXd& J, const Eigen::VectorXd& b) const
{
    if (J.rows() != b.size()) throw std::runtime_error("Dimension mismatch: J.row() must equal b.size()");
    Eigen::MatrixXd Jplus = pseudoInverse(J);
    return Jplus*b;
}   
Eigen::VectorXd JacobianSolver::solveDampedLeastSquares(const Eigen::MatrixXd& J, const Eigen::VectorXd& b, double lambda) const
{
    if (J.rows() != b.size()) throw std::runtime_error("Dimention mismatch: J.row() must equal b.size()");
    if (lambda <= 0.0) throw std::runtime_error("lambda must be postive");

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(J, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd singularValues = svd.singularValues();
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd V = svd.matrixV();
    const int r = singularValues.size();
    Eigen::MatrixXd S_inv = Eigen::MatrixXd::Zero(r, r);
    for (int i = 0; i < singularValues.size(); ++i) {
        double singma = singularValues(i);
        S_inv(i, i) = singma / (singma * singma + lambda * lambda);
    }
    return V * S_inv * U.transpose() * b;
}
double JacobianSolver::conditionNumber(const Eigen::MatrixXd& J, double tolerance) const
{
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(J, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::VectorXd singularValues = svd.singularValues();
    
    if (singularValues.size() == 0) throw std::runtime_error("Empty matrix has no condition number");

    double sigma_min = 10000, sigma_max = -10000;
    for (int i = 0; i < singularValues.size(); ++i) {
        double sigma  = singularValues(i);
        if (sigma > sigma_max) sigma_max  = sigma ;
        if (sigma < sigma_min) sigma_min  = sigma ;
    }
    if (sigma_min < tolerance) return std::numeric_limits<double>::infinity();
    return sigma_max/sigma_min;
}
