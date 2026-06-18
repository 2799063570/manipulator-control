#ifndef SAFETY_LIMITER_HPP
#define SAFETY_LIMITER_HPP

#include <vector>
#include <cstddef>

class SafetyLimiter{

public:
    explicit SafetyLimiter(size_t dof, double maxVelocity);

    void setLowerLimits(const std::vector<double>& low);
    void setUpperLimits(const std::vector<double>& upr);
    void setMaxVelocity(double v);
    const std::vector<double>& lowerLimites() const;
    const std::vector<double>& upperLimites() const;
    double maxVelocity() const; 
    size_t size() const;
    bool is_safety(const std::vector<double>& positon, double vel) const;

private:
    size_t dof_;
    std::vector<double> low_;
    std::vector<double> upr_;
    double maxVelocity_;
};

#endif 

