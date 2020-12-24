#include <direct.h>
#include <string>
#include <iostream>
using namespace std;

string concatString(char *s1, const char *s2);
string subreplace(string resource_str, string sub_str, string new_str);

int main()
{
    string buildPath = concatString(getcwd(NULL, 0), "/build");
    string cmd = "del \"" + buildPath + "\" /f/s/q/a";
    system(cmd.c_str());

    return 0;
}

string concatString(char *s1, const char *s2)
{
    string temp = subreplace(string(s1), "\\", "/") + string(s2) + "\0";
    return temp;
}

string subreplace(string resource_str, string sub_str, string new_str)
{
    string::size_type pos = 0;
    while ((pos = resource_str.find(sub_str)) != string::npos) //替换所有指定子串
    {
        resource_str.replace(pos, sub_str.length(), new_str);
    }
    return resource_str;
}
