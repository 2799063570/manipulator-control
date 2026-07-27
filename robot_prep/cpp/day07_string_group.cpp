#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

vector<vector<string>> getStringGroup(const vector<string>& strs)
{
    unordered_map<string, vector<string>> map;
    for (const string& str : strs)
    {
        string s = str;
        sort(s.begin(), s.end());
        map[s].push_back(str);
    }
    vector<vector<string>> ret;
    for (auto m : map)
    {
        ret.push_back(m.second);
    }
    return ret;
}
void printStringVector(const vector<vector<string>>& strs)
{
    for (const auto& vs : strs)
    {
        for (const string& s : vs)
        {
            cout << s << " ";
        }cout << endl;
    }
}
int main()
{
    vector<string> strs = {"eat","tea","tan","ate","nat","bat"};
    
    vector<vector<string>> s =  getStringGroup(strs);

    printStringVector(s);

    return 0;
}