#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>

using namespace std;

/*
    有效的括号 给定一个只包括：'('  ')'  '{'  '}'  '['  ']'
    判断字符串是否有效。
    有效字符串满足：
        1. 左括号必须用相同类型的右括号闭合。
        2. 左括号必须以正确的顺序闭合。
        3. 每个右括号都有一个对应的左括号。
*/

bool isValid_Stack(const string& chars)
{
    stack<char> judgment;
    unordered_map<char, char> map = {
        {'}', '{'},
        {']', '['},
        {')', '('}
    };
    for (char c :  chars)
    {
        if (map.find(c) != map.end())
        {
            if (judgment.top() != map[c]) return false;
            judgment.pop();
        }
        else
        {
            judgment.push(c);
        }
    }
    if (judgment.size() == 0) return true;
    return false;
}


int main()
{
    string s = "()[]{}";
    s = "(]";
    cout << "judg 括号正确 = " << (isValid_Stack(s) ? "true" : "false") << endl;

    return 0;
}