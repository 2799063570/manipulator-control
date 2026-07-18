#include <iostream>
#include <vector>

using namespace std;

/*
    找第一个大于等于 target 的位置
*/

int getGeaterIndex(const vector<int>& nums, int target)
{
    int l = -1, r = nums.size();
    while (r - l > 1)
    {
        int m=l+(r-l)/2;

        if (nums[m] < target) l = m;
        else  r = m;
    }
    return r;
}

int main()
{
    vector<int> nums = {1,3,5,7,9,11};
    cout << "get index : " << getGeaterIndex(nums, 3) << endl; 

    return 0;
}