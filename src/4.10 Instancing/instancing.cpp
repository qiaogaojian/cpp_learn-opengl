#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_loader.h"

#include <direct.h>
#include <iostream>
using namespace std;

const unsigned int SCR_WIDTH = 800;  // 屏幕宽度
const unsigned int SCR_HEIGHT = 600; // 屏幕高度

float vertices[] = {
    // 位置          // 颜色
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
     0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    // 初始化和配置 glfw
    //--------------------------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // 如果使用的是Mac OS X系统 需要解注释这行代码

    // 创建窗口对象
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化 glad  加载所有 opengl 的函数指针
    //--------------------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 构建和编译 shader 程序
    //--------------------------------------------------------------------------------------
    char *vsPath = "/src/4.10 Instancing/instancing.vs";
    char *fsPath = "/src/4.10 Instancing/instancing.fs";
    ShaderLoader shaderLoader(vsPath, fsPath, nullptr);

    // 设置顶点数据 配置顶点属性
    //--------------------------------------------------------------------------------------
    unsigned int VAO;
    unsigned int VBO;
    // 生成 VAO VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 首先绑定VAO
    glBindVertexArray(VAO);
    // 然后绑定并设置VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 然后设置顶点属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for(int y = -10; y < 10; y += 2)    // 从(0,0)开始
    {
        for(int x = -10; x < 10; x += 2)
        {
            vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    // 使用实例化数组 避免uniform数据传输限制
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 100, &translations[0], GL_STATIC_DRAW);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2,1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shaderLoader.use();
    // for (int i = 0; i < 100; i++)
    // {
    //     stringstream offset;
    //     offset << "offsets[" << i << "]";
    //     shaderLoader.setVec2(offset.str(),translations[i]);
    // }                       // shaderLoader.use()之后设置
    vec4 colorC = vec4(0.68f, 0.51f, 1.0f, 1.0f);
    shaderLoader.setVec4("colorC", colorC);         // uniform 在 while 循环之前和循环中都要设置

    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        //处理渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置状态
        glClear(GL_COLOR_BUFFER_BIT);         // 使用状态

        shaderLoader.use();
        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

        // 检查并调用事件，交换缓冲完成绘制
        glfwPollEvents();        // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwSwapBuffers(window); // 双缓冲交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
    }

    // 释放资源
    //--------------------------------------------------------------------------------------

    // 释放GLFW资源
    glfwTerminate(); // 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源 这样便能清理所有的资源并正确地退出应用程序
    return 0;
}

// 窗口大小改变时 调整视口大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 处理输入
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}