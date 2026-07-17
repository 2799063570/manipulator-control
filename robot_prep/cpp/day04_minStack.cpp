#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class StackSelf
{
public:
    void push(T x)
    {
        nums_.push_back(x);
        if (mins_.empty() || x<mins_.back())
            mins_.push_back(x);
        else mins_.push_back(mins_.back());
    }
    void pop()
    {
        nums_.pop_back();
        mins_.pop_back();
    }
    T top()
    {
        return nums_.back();
    }
    T getMin()
    {
        return mins_.back();
    }
private:
    vector<T> nums_;
    vector<T> mins_;
};

int main()
{
    StackSelf<int> s;
    s.push(1);
    s.push(2);
    cout << s.top() << endl;
    s.pop();
    cout << s.top() << endl;
    s.push(12);
    s.push(234);
    cout << s.getMin() << endl;

    return 0;
}