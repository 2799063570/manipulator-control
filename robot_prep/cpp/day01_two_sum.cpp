#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

/*
    给你一个整数数组 `vector<int> nums`，以及一个目标值 `target`。
    找到数组中两个数字，它们相加等于 target，返回它们的下标。
*/

// 方法1 暴力求解
vector<int> findSumIndex(const vector<int>& nums, int target)
{
    vector<int> ret;
    for (int i = 0, size = nums.size(); i<size-1; i++)
    {
        for (int j = i+1; j<size; j++)
        {
            if (nums[i] + nums[j] == target)
            {
                ret.push_back(i);
                ret.push_back(j);
            }
        }
    }
    return ret;
}
// 方法2 借助哈希表
vector<int> findSumIndex_hash(const vector<int>& nums, int target)
{
    vector<int> ret;
    unordered_map<int, int> map;

    for (int i = 0, size = nums.size(); i<size; ++i)
    {
        int value = nums[i];
        if (map.find(target-value) != map.end())
        {
            ret.push_back(i);
            ret.push_back(map[target-value]);
        }
        else map[value] = i;
    }
    return ret;
}
// 方法3 借助vector 地址映射法
vector<int> findSumIndex_vector(const vector<int>& nums, int target, int max)
{
    vector<int> ret;
    vector<int> map(max+1, -1);
    for (int i = 0, size = nums.size(); i<size; ++i)
    {
        map[nums[i]] = i;
    }
    for (int i = 0, size = nums.size()/2; i<size; ++i)
    {
        int value = nums[i];
        if (target-value > max) continue;
        if (map[target-value] != -1)
        {
            ret.push_back(i);
            ret.push_back(map[target-value]);
        }
    }
    return ret;
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
    cout << "find index == target" << endl;
    vector<int> nums = {1, 2, 3, 4, 5, 6};
    int target = 7;
    vector<int> ans1 = findSumIndex(nums, target);
    printVector(ans1);
    vector<int> ans2 = findSumIndex_hash(nums, target);
    printVector(ans2);
    vector<int> ans3 = findSumIndex_vector(nums, target, 6);
    printVector(ans3);

    return 0;
}