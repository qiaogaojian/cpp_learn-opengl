#share #opengl #�������� #cpp #mscv #vscode

> ԭ�ĵ�ַ [github.com](https://github.com/qiaogaojian/cpp_learn-opengl)

��Ϊ windows ������ʹ�� g++ ������ assimp ������ʧ��, ���� "undefined reference to'Assimp::Importer::Importer()", ��������ʹ�� MSVC ������û���������, ���˼��������ڴ�ɹ�, �ڴ˼�¼һ�»���VSCode + MSVC�������� OpenGL ������������.

����, mingw/g++ ���������� g++ ��֧ 
visual studio ���������� vs2019 ��֧.

### 1. ��װ�������

*   VSCode ��װ C++ extension

*   ���ذ�װ [assimp-sdk-3.3.1-setup_vs2017.exe](https://github.com/assimp/assimp/releases/tag/v3.3.1/)
    
*   ��װ�� C++ build tools �� Visual Studio, ���ض����� [C++ build tools](https://visualstudio.microsoft.com/zh-hant/visual-cpp-build-tools/) Ҳ����.
    
*   �������õĿ���[��Ŀ](https://github.com/qiaogaojian/learn-opengl.git)�е� include �ļ�����
    

### 2. ���û�������

*   Path ����� assimp ��
    
```sh
D:\Opengl\Assimp\bin\x86  # �����Ͳ���Ҫ��assimp.dll�ŵ�buildĿ¼��
```
    
*   �½� INCLUDE ��������, �������Ŀ¼
    
```sh
D:\Program\VS2019\VS\VC\Tools\MSVC\14.28.29333\include
D:\Program\VS2019\VS\VC\Tools\MSVC\14.28.29333\atlmfc\include
D:\Program\VS2019\VS\Common7\Tools
D:\Windows Kits\10\Include\10.0.19041.0\ucrt
D:\Windows Kits\10\Include\10.0.19041.0\um
D:\Windows Kits\10\Include\10.0.19041.0\shared
D:\Windows Kits\10\Include\10.0.19041.0\winrt
D:\Windows Kits\10\Include\10.0.19041.0\cppwinrt
```
    
*   �½� LIB ��������, �������Ŀ¼
    
```sh
D:\Program\VS2019\VS\VC\Tools\MSVC\14.28.29333\lib\x86
D:\Program\VS2019\VS\VC\Tools\MSVC\14.28.29333\atlmfc\lib\x86
D:\Program\VS2019\VS\VC\Auxiliary\VS\lib\x86
D:\Program\VS2019\VS\VC\Auxiliary\VS\UnitTest\lib
D:\Windows Kits\10\lib\10.0.19041.0\ucrt\x86
D:\Windows Kits\10\lib\10.0.19041.0\um\x86
```
    

### 3. ���� vscode

*   �½�Ŀ¼ OpenGl ��ʹ�� VSCode ��, `.vscode` Ŀ¼�� (û�о��½�) ���� `settings.json`, ������������
    
```json
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
    
*   .vscode �´��� `c_cpp_properties.json`
    
```json
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
    
*   .vscode �´��� `tasks.json`
    
```json
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
    
.vscode �´��� `launch.json`
    
```json
{
	"version": "0.2.0",
	"configurations": [
		{
			"name": "(Windows) Launch",
			"type": "cppvsdbg",
			"request": "launch",
			"program": "${workspaceFolder}\\build\\${fileBasenameNoExtension}.exe",
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
    

### ע������

1.  Ҫȷ��������������Ŀ�еĿⱣ��һ�� Ҫô���� x86 Ҫô���� x64
    
2.  ȷ��ʹ�� vsdevcmd ���б���
    
3.  ���� "undefined reference to xxx", �ȼ�� tasks.json ����ȱʧ�Ŀ�, �ټ�黷������
    
4.  ����ʱ��Ҫ���� build �ļ���, ���� build �ļ��� git �Ѻ���, �����Ҫ�Լ���ǰ�½���