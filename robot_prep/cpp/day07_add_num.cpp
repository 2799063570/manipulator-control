#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

vector<int> getAddnumsIndex(const vector<int>& nums, int target)
{
    unordered_map<int, int> map;
    for (int i = 0, size = nums.size(); i<size; i++)
    {
        int temp = target - nums[i];
        if (map.find(temp) != map.end())
        {
            return {i, map[temp]};
        }
        map[nums[i]] = i;
    }
    return {0, 0};
}

void printVector(const vector<int>& nums)
{
    for (int i : nums)
    {
        cout << i << " ";
    }cout << endl;
}


int main()
{
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7};
    vector<int> nums1 = getAddnumsIndex(nums, 3);
    printVector(nums1);
    
    return 0;
}