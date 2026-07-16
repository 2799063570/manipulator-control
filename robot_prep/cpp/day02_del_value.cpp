#include <iostream>
#include <vector>

using namespace std;

// 快慢指针
int delValueFS(vector<int>& nums, int target)
{
    int slow = 0, fast = 0, size = nums.size();
    while(fast < size)
    {
        if (nums[fast] == target)
            fast++;
        else 
        {
            if (fast != slow) 
            {
                int temp = nums[fast];
                nums[fast] = nums[slow];
                nums[slow] = temp;
            }
            fast++;slow++;
        }
    }
    
    return fast-slow;
}
// 首尾指针
int delValueSE(vector<int>& nums, int target)
{
    int start = 0, end = nums.size()-1;
    while(start < end)
    {
        if (nums[start] == target)
        {
            while (nums[end] == target && end >= start)
            {
                end--;
            }
            if(start <= end)
            {
                swap(nums[start],nums[end]);
                end--;
            }
        }
        start++;
    }
    cout << start << " " << end << endl;
    return nums.size() - start;
}
int delValueVector(vector<int>& nums, int target)
{
    vector<int> result;
    for (int n : nums)
    {
        if (n != target)
            result.push_back(n);
    }
    nums = result;
    return result.size();
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
    vector<int> nums = {2, 2, 3, 2, 3, 2, 2, 2, 5, 3, 4};
    printVector(nums);
    cout << delValueFS(nums, 2) << endl;
    printVector(nums);

    nums = {2, 2, 3, 2, 3, 2, 2, 2, 5, 3, 4};
    printVector(nums);
    cout << delValueSE(nums, 2) << endl;
    printVector(nums);

    nums = {2, 2, 3, 2, 3, 2, 2, 2, 5, 3, 4};
    printVector(nums);
    cout << delValueVector(nums, 2) << endl;
    printVector(nums);

    return 0;
}