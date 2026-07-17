#include <iostream>
#include <stack>
#include <vector>


using namespace std;

vector<int> getTemMaxDay(const vector<int>& tems)
{
    int size = tems.size();
    if (size == 1) return {0};
    vector<int> nums(size, 0);
    stack<pair<int, int>> temperatures;
    temperatures.push({tems[0], 0});
    for (int i = 1; i<size; i++)
    {
        while (!temperatures.empty() && tems[i] > temperatures.top().first)
        {
            nums[temperatures.top().second] = i - temperatures.top().second;
            temperatures.pop();
        }
        temperatures.push({tems[i], i});
    }
    return nums;

}
vector<int> getTemMaxDayIndex(const vector<int>& tems)
{
    int size = tems.size();
    vector<int> nums(size, 0);
    stack<int> temperatures;
    temperatures.push(0);
    for (int i = 1; i<size; i++)
    {
        while (!temperatures.empty() && tems[i] > tems[temperatures.top()])
        {
            nums[temperatures.top()] = i - temperatures.top();
            temperatures.pop();
        }
        temperatures.push(i);
    }
    return nums;

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
    vector<int> test = {4, 3, 2, 4, 3, 2, 5};
    printVector(getTemMaxDay(test));
    printVector(getTemMaxDayIndex(test));


    return 0;
}