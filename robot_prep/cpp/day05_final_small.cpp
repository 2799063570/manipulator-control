#include <iostream>
#include <vector>

using namespace std;

int getSmallIndex(const vector<int>& nums, int target)
{
    int l = -1, r = nums.size();
    while (r - l > 1)
    {
        int m = (l+r)/2;
        if (nums[m] <= target) l = m;
        else  r = m;
    }
    return l;
}

int main()
{
    vector<int> nums = {1,3,5,5, 5,7,9,11};
    cout << "get index : " << getSmallIndex(nums, 7) << endl; 

    return 0;
}