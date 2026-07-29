#include <iostream>
#include <vector>
using namespace std;

bool dfs(vector<vector<int>>& map, int node, vector<int>& visited)
{
    if (visited[node] == 1) return true;

    if (visited[node] == 2) return false;

    visited[node] = 1;

    for (int i : map[node])
    {
        if (dfs(map, i, visited) == 1) return true;
    }
    visited[node] = 2;
    return false;
}
bool isFinishAllCourse(int numCourses, const vector<vector<int>>& prerequisites)
{
    vector<vector<int>> map(numCourses);
    for (const vector<int>& v : prerequisites)
    {
        map[v[1]].push_back(v[0]); 
    }
    vector<int> state(numCourses, 0);
    for (int i = 0; i<numCourses; ++i)
    {
        if (dfs(map, i, state)) return false;
    }
    return true;
}

int main()
{
    vector<vector<int>> prereq = {
        {1, 0},
        {0, 3}
    };
    cout << "is finish all course ? " << (isFinishAllCourse(3, prereq) ? "yes" : "no") << endl;


    return 0;
}