cl /Zi /EHsc /MD /Fd: build/ /Fo: build/ /Fe: build/ src/"3.3 Model"/model.cpp include/utils/shader_loader.cpp include/utils/stb_image.cpp include/glfw/lib-vc2019/glfw3.lib include/glad/lib/glad.o include/assimp/lib/assimp.lib include/msvc/OpenGL32.Lib include/msvc/User32.Lib include/msvc/Gdi32.Lib include/msvc/shell32.lib /Iinclude/glad/include /Iinclude/glfw/include /Iinclude/glm /Iinclude/utils /Iinclude/assimp/include /Link /NODEFAULTLIB:msvcrt.lib

cl /Zi /EHsc /MD /Fd: build/ /Fo: build/ /Fe: build/ src/"3.3 Model"/model.cpp include/utils/shader_loader.cpp include/utils/stb_image.cpp include/glfw/lib-vc2019/glfw3.lib include/glad/lib/glad.o include/assimp/lib/assimp.lib D:/"Windows Kits"/10/Lib/10.0.18362.0/um/x86/OpenGL32.Lib D:/"Windows Kits"/10/Lib/10.0.18362.0/um/x86/User32.Lib D:/"Windows Kits"/10/Lib/10.0.18362.0/um/x86/Gdi32.Lib D:/"Windows Kits"/10/Lib/10.0.18362.0/um/x86/shell32.lib D:/Program/VS/vs2019/VC/Tools/MSVC/14.28.29333/lib/x86/msvcprt.lib /Iinclude/GLFW/include /Iinclude/glad/include /Iinclude/glfw-3.3.2.bin.WIN32/include /Iinclude/glm /Iinclude/utils /Iinclude/assimp/include /Link /NODEFAULTLIB:msvcrt.lib