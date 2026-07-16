#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>

using namespace std;

/*
    滑动窗口法求解 一个字符串 不含重复字母的子字符串的最大长度
*/

int getMaxCharsLen(const string& chars)
{
    array<int, 26> count{};
    int size = chars.size(), cnt=-1;
    for (int r = 0, l = 0; r<size; r++)
    {
        count[chars[r]-'a']++;
        while (count[chars[r]-'a'] > 1)
        {
            count[chars[l]-'a']--;
            l++;
        } 
        cnt = max(cnt, r-l+1);
    }
    return cnt;
}
int getMaxCharsLen_Hash(const string& chars)
{
    unordered_map<char, int> count;
    int size = chars.size(), cnt=-1;
    for (int r = 0, l = 0; r<size; r++)
    {
        count[chars[r]]++;
        while (count[chars[r]] > 1)
        {
            count[chars[l]]--;
            l++;
        } 
        cnt = max(cnt, r-l+1);
    }
    return cnt;
}

int main()
{
    string s = "abcabcbb";
    cout << "chars max len = " << getMaxCharsLen(s) << endl;
    cout << "chars max len = " << getMaxCharsLen_Hash(s) << endl;

    return 0;
}