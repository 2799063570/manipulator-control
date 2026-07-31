#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/*
    给定数组：nums = [3,2,1,5,6,4]
    要求：找出第 k 大的元素。
*/
int getKNum(const vector<int>& nums, int k)
{
    // 利用最小堆 将大的元素弹出来
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