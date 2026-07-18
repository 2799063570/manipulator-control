#include <iostream>
#include <vector>

using namespace std;

int binarySearch(const vector<int>& nums, int target)
{
    int l = -1, r = nums.size();
    while (r - l > 1)
    {
        int m = (l + r)/2;
        if (nums[m] > target)
        {
            r = m;
        }
        else if (nums[m] == target)
        {
            return m;
        }
        else
        {
            l = m;
        }
    }
    return -1;
}

int main()
{
    vector<int> nums = {1,3,5,7,9,11};
    cout << "get index : " << binarySearch(nums, 121) << endl; 
    return 0;
}