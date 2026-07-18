#include <iostream>
#include <vector>
#include <algorithm>

/*
    我们使用sort对容器内的元素进行排序
    sort(first, last, comparater)
    comparater用于指定顺序，对于pair两个元素 我们来制定他的排序比较规则
    先比较第二个元素再比较第一个元素    
*/

using namespace std;

void printVectorPair(const vector<pair<int, int>>& nums)
{
    for (pair<int, int> n : nums)
    {
        cout << "{" << n.first << ", " << n.second << "} ";
    }cout << endl;
}
int main()
{
    vector<pair<int, int>> nums2 = {
        {1, 3}, 
        {2, 4},
        {4, 1},
        {7, 4},
        {5, 9},
        {1, 3},
        {3, 5}
    };
    printVectorPair(nums2);
    sort(nums2.begin(), nums2.end(), 
        [](pair<int, int> a, pair<int, int> b)->bool
        {
            if (a.second == b.second)
                return a.first < b.first;
            return a.second < b.second;
        });
    printVectorPair(nums2);
    
    return 0;
}