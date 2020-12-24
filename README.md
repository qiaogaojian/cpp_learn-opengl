## VSCode使用MSVC编译器搭建OpenGL开发环境

因为windows环境下使用g++编译器assimp库链接失败,报错"undefined reference to 'Assimp::Importer::Importer()",后来发现MSVC可以正常使用,踩了几个坑终于搭建成功,在此记录一下.

另外,mingw/g++开发环境在g++分支,visual studio开发环境在vs2019分支.

### 1.安装

- VSCode 安装 C++ extension

- 下载安装 [assimp-sdk-3.3.1-setup_vs2017.exe
](https://github.com/assimp/assimp/releases/tag/v3.3.1/)

- 安装带 C++ build tools 的Visual Studio, 下载独立的 [C++ build tools](https://visualstudio.microsoft.com/zh-hant/visual-cpp-build-tools/) 也可以.

- 其他所用的库在[项目](https://github.com/qiaogaojian/learn-opengl.git)中的include文件夹里

### 2.环境变量

- Path中添加assimp库 cl编译器

  ``` sh
  D:\Program Files\VS2019\VS\VC\Tools\MSVC\14.28.29333\bin\Hostx86\x86 # 这里使用的是x86架构
  D:\Opengl\Assimp\bin\x86  # 这样就不需要把assimp.dll放到build目录了
  ```

- 新建INCLUDE环境变量,添加以下目录

  ```sh
  D:\Windows Kits\10\Include\10.0.19041.0\ucrt
  D:\Windows Kits\10\Include\10.0.19041.0\um
  D:\Windows Kits\10\Include\10.0.19041.0\shared
  D:\Windows Kits\10\Include\10.0.19041.0\winrt
  D:\Program Files\VS2019\VS\VC\Tools\MSVC\14.28.29333\include
  ```

- 新建LIB环境变量,添加以下目录

  ``` sh
  D:\Windows Kits\10\lib\10.0.19041.0\ucrt\x86
  D:\Windows Kits\10\lib\10.0.19041.0\um\x86
  D:\Program Files\VS2019\VS\VC\Tools\MSVC\14.28.29333\lib\x86
  ```

### 3.设置vscode

- 新建并使用VSCode打开目录OpenGl,.vscode目录下(没有就新建)创建settings.json,配置一线内容

  ``` json
  {
    "terminal.integrated.shell.windows":   "C:\\Windows\\Sysnative\\cmd.exe",
    "terminal.integrated.shellArgs.windows": [
      "/k",
      "D:/Program Files/VS2019/VS/Common7/Tools/VsDevCmd.bat"
    ],
    "files.encoding": "gb2312",
    "files.associations": {
      "*.json": "jsonc",
      "*.cfg": "ini",
      "*.fsh": "glsl",
      "stack": "cpp",
      "iostream": "cpp",
      "ostream": "cpp",
      "*.tcc": "cpp",
      "cctype": "cpp",
      "clocale": "cpp",
      "cmath": "cpp",
      "cstdint": "cpp",
      "cstdio": "cpp",
      "cstdlib": "cpp",
      "cwchar": "cpp",
      "cwctype": "cpp",
      "exception": "cpp",
      "initializer_list": "cpp",
      "iosfwd": "cpp",
      "istream": "cpp",
      "limits": "cpp",
      "new": "cpp",
      "stdexcept": "cpp",
      "streambuf": "cpp",
      "system_error": "cpp",
      "type_traits": "cpp",
      "typeinfo": "cpp",
      "utility": "cpp",
      "iomanip": "cpp"
    }
  }
  ```

- .vscode下创建c_cpp_properties.json

  ``` json
  {
    "configurations": [
      {
        "name": "Win32",
        "includePath": [
          "${workspaceFolder}/**",
          "${workspaceFolder}/include/utils",
          "${workspaceFolder}/include/glm/glm",
          "${workspaceFolder}/include/glad/include",
          "${workspaceFolder}/include/glfw/include",
          "${workspaceFolder}/include/assimp/include",
          "${workspaceFolder}/include/glad/include"
        ],
        "defines": ["_DEBUG", "gb2312", "_gb2312"],
        "intelliSenseMode": "msvc-x64",
        "browse": {
          "path": [
              "${workspaceFolder}/**",
              "${workspaceFolder}/include/utils",
              "${workspaceFolder}/include/glm/glm",
              "${workspaceFolder}/include/glad/include",
              "${workspaceFolder}/include/glfw/include",
              "${workspaceFolder}/include/assimp/include",
              "${workspaceFolder}/include/glad/include"
          ],
          "limitSymbolsToIncludedHeaders": true,
          "databaseFilename": ""
        }
      }
    ],
    "version": 4
  }
  ```

- .vscode下创建 tasks.json

  ``` json
  {
    "version": "2.0.0",
    "tasks": [
      {
        "type": "cppbuild",
        "label": "compile",
        "command": "cl",
        "args": [
          "/Od",
          "/Zi",
          "/EHsc",
          "/MD",
          "/W4",
          "/Fd:",
          "build/",
          "/Fo:",
          "build/",
          "/Fe:",
          "build/",
          "${file}",
          "include/utils/shader_loader.cpp",
          "include/utils/stb_image.cpp",
          "include/assimp/lib/assimp.lib",
          "include/glfw/lib-vc2019/glfw3.lib",
          "include/glad/lib/glad.o",
          "include/msvc/OpenGL32.Lib",
          "include/msvc/User32.Lib",
          "include/msvc/Gdi32.Lib",
          "include/msvc/shell32.lib",
          "/Iinclude/glfw/include",
          "/Iinclude/glad/include",
          "/Iinclude/glm",
          "/Iinclude/utils",
          "/Iinclude/assimp/include",
          "/Link /NODEFAULTLIB:msvcrt.lib"
        ],
        "options": {
          "cwd": "${workspaceFolder}"
        },
        "problemMatcher": ["$msCompile"],
        "group": {
          "kind": "build",
          "isDefault": true
        },
        "detail": "cl.exe"
      }
    ]
  }

  ```

  .vscode下创建 launch.json

  ``` json
  {
      "version": "0.2.0",
      "configurations": [
          {
              "name": "(Windows) Launch",
              "type": "cppvsdbg",
              "request": "launch",
              "program": "${workspaceFolder}\\build\\$  {fileBasenameNoExtension}.exe",
              "args": [],
              "stopAtEntry": false,
              "cwd": "${workspaceFolder}",
              "environment": [],
              "externalConsole": true,
              "preLaunchTask": "compile"
          }
      ]
  }
  ```

### 遇到的几个坑

1. 要确保环境变量和项目中的库保持一致 要么都是x86 要么都是x64

2. 确保使用vsdevcmd进行编译

3. 遇到 "undefined reference to xxx", 先检查 tasks.json 链接缺失的库, 再检查环境变量