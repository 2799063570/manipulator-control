#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> grid{
    {1,1,0,0,0},
    {1,1,0,0,1},
    {1,0,1,0,0},
    {0,0,0,1,1}
};

void dfs(vector<vector<int>>& grid, int i, int j)
{
    int m = grid.size();
    int n = grid[0].size();

    // 越界或者不是陆地
    if(i < 0 || i >= m ||
       j < 0 || j >= n ||
       grid[i][j] == 0)
    {
        return;
    }

    if (grid[i][j] == 1) 
    {
        grid[i][j] = 0;
        dfs(grid, i-1, j);
        dfs(grid, i, j-1);
        dfs(grid, i+1, j);
        dfs(grid, i, j+1);
    }
}

int getIslandsNum(vector<vector<int>>& grid)
{
    int ret = 0;
    for (int i = 0, size_i = grid.size(), size_j = grid[0].size(); i<size_i; i++)
    {
        for (int j = 0; j<size_j; j++)
        {
            if (grid[i][j] == 1)
            {
                dfs(grid, i, j);
                ret++;
            }
        }
    }
    return ret;
}

void bfs(
    vector<vector<int>>& grid,
    int i,
    int j)
{
    int m = grid.size();
    int n = grid[0].size();

    queue<pair<int,int>> q;

    q.push({i,j});

    // 入队立即标记
    grid[i][j]=0;

    int dirs[4][2]={
        {-1,0},
        {1,0},
        {0,-1},
        {0,1}
    };

    while(!q.empty())
    {
        auto node=q.front();
        q.pop();

        for(auto& dir:dirs)
        {
            int ni=node.first+dir[0];
            int nj=node.second+dir[1];

            if(ni>=0 && ni<m &&
               nj>=0 && nj<n &&
               grid[ni][nj]==1)
            {
                grid[ni][nj]=0;
                q.push({ni,nj});
            }
        }
    }
}
int getIslandsNumBFS(vector<vector<int>>& grid)
{
    int ret = 0;
    for (int i = 0, size_i = grid.size(), size_j = grid[0].size(); i<size_i; i++)
    {
        for (int j = 0; j<size_j; j++)
        {
            if (grid[i][j] == 1)
            {
                bfs(grid, i, j);
                ret++;
            }

        }
    }
    return ret;
}
int main()
{
    cout << getIslandsNumBFS(grid) << endl;
    return 0;
}