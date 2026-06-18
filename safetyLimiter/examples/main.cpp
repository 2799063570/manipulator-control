#include "safetyLimiter.hpp"
#include <iostream>

using namespace std;

void printVector(const vector<double>& v)
{
    for (auto value : v)
     cout << value << " ";
    cout << endl;
}
int main()
{
    SafetyLimiter safer(6, 1.2);

    vector<double> u = {5.1, 5.1, 1.8, 1.4, 5.2, 13};
    vector<double> l = {-2.1, -5.1, -1.8, -1.4, -5.2, -13};
    vector<double> p = {1.1, 2.1, 0.5, 0.8, 5.0, 8.0};

    cout << "init low : ";
    printVector(safer.lowerLimites());
    cout << "init upr : ";
    printVector(safer.upperLimites());
    cout << "init maxVelocity : " << safer.maxVelocity() << endl;

    safer.setLowerLimits(l);
    safer.setUpperLimits(u);
    safer.setMaxVelocity(4.5);

    cout << "set low : ";
    printVector(safer.lowerLimites());
    cout << "set upr : ";
    printVector(safer.upperLimites());
    cout << "init maxVelocity : " << safer.maxVelocity() << endl;

    cout << "our position : ";
    printVector(p);
    cout << "is safe " << (safer.is_safety(p, 1.2) ? "ok" : "no") << endl;

    cout << "our position : ";
    printVector(p);
    cout << "is safe " << (safer.is_safety(p, 51.2) ? "ok" : "no") << endl;
    
    return 0;
}