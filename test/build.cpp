#include <direct.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

// shell命令构建工具 需要把相应的脚本拖到根目录
string concatString(char *s1, const char *s2);
string subreplace(string resource_str, string sub_str, string new_str);
vector<string> split(const string &s, char delim);

string cmdStart = "cl /Zi /EHsc /MD /Fd: build/ /Fo: build/ /Fe: build/ \"";

string cmdEnd = "\" include/utils/shader_loader.cpp include/utils/stb_image.cpp include/glfw/lib-vc2019/glfw3.lib include/glad/lib/glad.o include/assimp/lib/assimp.lib include/msvc/OpenGL32.Lib include/msvc/User32.Lib include/msvc/Gdi32.Lib include/msvc/shell32.lib /Iinclude/glad/include /Iinclude/glfw/include /Iinclude/glm /Iinclude/utils /Iinclude/assimp/include /Link /NODEFAULTLIB:msvcrt.lib";

int main(int argc, char *argv[])
{
    system("clear.exe");

    string filePath = argv[1];
    string cmd = cmdStart + filePath + cmdEnd;
    system(cmd.c_str());

    vector<string> paths = split(filePath, '\\');
    string fileName = paths[paths.size() - 1];
    string buildName = "/build/" + subreplace(fileName, ".cpp", ".exe");
    string cmdRun = concatString(getcwd(NULL, 0), buildName.c_str());
    system(cmdRun.c_str());
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

vector<string> split(const string &s, char delim)
{
    stringstream ss(s);
    string item;
    vector<string> elems;
    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}
