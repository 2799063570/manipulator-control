#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// 暴力求解
vector<int> getTwoIndex(const vector<int>& nums, int target)
{
    for (int i = 0, size = nums.size(); i<size-1; ++i)
    {
        for (int j = i+1; j<size; ++j)
        {
            if (nums[i] + nums[j] == target)
                return {i, j};
        }
    }
    return {};
}
// 双指针
vector<int> getTwoIndex_twoPointer(const vector<int>& nums, int target)
{
    for (int i = 0, j = nums.size()-1; j > i;)
    {
        if (nums[i] + nums[j] == target)
            return {i, j};
        else if (nums[i] + nums[j] > target)
            j--;
        else i++;
    }
    return {};
}
// 借助哈希表
vector<int> getTwoIndex_hash(const vector<int>& nums, int target)
{
    unordered_map<int, int> map;
    for (int i = 0, size = nums.size(); i<size; i++)
    {
        int otherNums = target - nums[i];
        if (map.find(otherNums) != map.end())
        {
            return {i, map[otherNums]};
        }
        map[nums[i]] = i;
    }
    return {};
}

void printVector(const vector<int>& nums)
{
    for (int n : nums)
    {
        cout << n << " ";
    }cout << endl;
}

int main()
{   
    vector<int> nums = {1, 2, 4, 5, 6, 12, 15, 23};
    int target = 11;
    vector<int> indexs1 = getTwoIndex(nums, target);
    printVector(indexs1);
    vector<int> indexs2 = getTwoIndex_twoPointer(nums, target);
    printVector(indexs2);
    vector<int> indexs3 = getTwoIndex_hash(nums, target);
    printVector(indexs3);


    return 0;
}