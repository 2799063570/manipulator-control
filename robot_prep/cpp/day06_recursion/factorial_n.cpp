#include <iostream>
#include <vector>
#include <string>

using namespace std;

int factorial(int n)
{
    if (n<=1) return 1;
    return n*factorial(n-1);
}
int vectorSum(const vector<int>& nums, int index)
{
    if (index == nums.size()) return 0;
    return nums[index] + vectorSum(nums, index+1);
}
void reverseString(string& s, int left, int right)
{
    if (right-left < 1) return;
    reverseString(s, left+1, right-1); 
    swap(s[left], s[right]);
}

int main()
{
    cout << factorial(5) << endl;
    vector<int> nums = {1,2,3,4,5};
    cout << vectorSum(nums, 1) << endl;
    string s = "hello";
    reverseString(s, 0, s.size()-1);
    cout << s << endl;
    return 0;
}