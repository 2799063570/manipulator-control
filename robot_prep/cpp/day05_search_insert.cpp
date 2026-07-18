#include <iostream>
#include <vector>

using namespace std;

/*
    搜索插入位置（第一个大于等于）
*/

int searchInsertIndex(vector<int>& nums, int target)
{
    int l = -1, r = nums.size();
    while (r - l > 1)
    {
        int m = (l+r)/2;
        if (nums[m] < target) l = m;
        else if (nums[m] > target) r = m;
        else r = m;
    }
    nums.insert(nums.begin() + r, target);
    return r;
}
void printVector(const vector<int>& nums)
{
    for (auto n : nums)
    {
        cout << n << " ";
    }cout << endl;
}
int main()
{
    vector<int> nums = {1,3,3,3,9,11};
    printVector(nums);
    cout << searchInsertIndex(nums, 3) << endl;
    printVector(nums);
    return 0;
}