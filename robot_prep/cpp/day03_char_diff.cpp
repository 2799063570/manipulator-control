#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

/*
    给你两个字符串 判断两个字符串是否是异位字符串
     字母种类相同、每种字母出现次数相同、顺序可以不同
*/

bool isValidAnagram(const string& s, const string& t)
{
    if (s.size() != t.size()) return false;

    vector<int> s_count(26, 0), t_count(26, 0);
    for (int i = 0, size = s.size(); i<size; ++i)
    {
        // s_count[(int)s[i] - 97]++;
        s_count[s[i] - 'a']++;
        // t_count[(int)t[i] - 97]++;
        t_count[t[i] - 'a']++;
    }
    for (int i = 0; i<26; ++i)
    {
        if (s_count[i] != t_count[i]) return false;
    }
    return true;
}
bool isValidAnagram_hash(const string& s, const string& t)
{
    if (s.size() != t.size()) return false;
    unordered_map<char, int> s_count, t_count;
    for (int i = 0, size = s.size(); i<size; ++i)
    {
        // if (s_count.find(s[i]) == s_count.end())
        //     s_count[s[i]] = 1;
        // else
        //     s_count[s[i]]++;
        // if (t_count.find(t[i]) == t_count.end())
        //     t_count[t[i]] = 1;
        // else
        //     t_count[t[i]]++;
        // 优化
        s_count[s[i]]++;
        t_count[t[i]]++;
    }
    for (auto item : s_count)
    {
        if (item.second != t_count[item.first])
            return false;
    }
    return true;
}
bool isValidAnagram_hash_pro(const string& s, const string& t)
{
    if (s.size() != t.size()) return false;
    unordered_map<char, int> count;
    for (int i = 0, size = s.size(); i<size; ++i)
    {
        count[s[i]]++;
        count[t[i]]--;
    }
    for (auto item : count)
    {
        if (item.second != 0) return false;
    }
    return true;
}
int main()
{
    string s = "anagram";

    string t = "nagaraa";
    cout << "s == t ? : " << (isValidAnagram(s, t) ? "true" : "false") << endl;
    cout << "s == t ? : " << (isValidAnagram_hash(s, t) ? "true" : "false") << endl;
    cout << "s == t ? : " << (isValidAnagram_hash_pro(s, t) ? "true" : "false") << endl;
    
    return 0;
}