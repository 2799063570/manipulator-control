#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int getKNum(const vector<int>& nums, int k)
{
    priority_queue<int, vector<int>, greater<int>> q;
    for (int n : nums)
    {
        q.push(n);
        if (q.size() > k) q.pop();
    }
    return q.top();
}
int main()
{
    vector<int> nums = {3, 2, 1, 5, 6, 4};
    cout << getKNum(nums, 3) << endl;
    return 0;
}