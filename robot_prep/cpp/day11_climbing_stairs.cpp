#include <iostream>
#include <vector>

using namespace std;

/*
    假设你正在爬楼梯，需要 n 阶才能到达楼顶。
    每次你可以：
        爬 1 阶 或者 爬 2 阶
    问：
        有多少种不同的方法可以爬到楼顶？
*/
vector<int> getClimbingStairsVector(int n)
{
    vector<int> ret;
    ret.push_back(1);
    if (n == 1) return ret;
    ret.push_back(2);
    if (n == 2) return ret;
    for (int i = 2; i<n; i++)
    {
        ret.push_back(
            ret[i-1]+ret[i-2]
        );
    }
    return ret;
}

/*
    有一排房子：[2,7,9,3,1] 每个房子有金额。
    但是：
        不能偷相邻的两个房子。
        求最大收益。
*/
vector<int> getHouseMaxVector(const vector<int>& houseValue, int n)
{
    vector<int> ret;
    ret.push_back(houseValue[0]);
    ret.push_back(max(houseValue[0], houseValue[1]));
    for(int i = 2; i<n; i++)
    {
        // 状态转移关系 上一个 上上一个 这个位置的最大价值
        ret.push_back(max(ret[i-1], ret[i-2] + houseValue[i]));
    }
    return ret;
}
int main()
{
    vector<int> stairsVector = getClimbingStairsVector(4);
    cout << stairsVector[3] << endl;

    return 0;
}
