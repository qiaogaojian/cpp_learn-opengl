#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "tools.h"
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "direct.h"
#include "shader_loader.h"
#include "ft2build.h"
#include FT_FREETYPE_H

using namespace std;

const unsigned int SCR_WIDTH = 800;  // 屏幕宽度
const unsigned int SCR_HEIGHT = 600; // 屏幕高度

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "	FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}\n\0";

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void RenderText(ShaderLoader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

struct Character{
    unsigned int TextureID; // 字形纹理的ID
    vec2 Size;              // 字形大小
    vec2 Bearing;           // 从基准线到左部/顶部的偏移值
    unsigned int Advance;   // 原点到下一个字形的距离
};

map<char, Character> characters;
GLuint textVAO,textVBO;

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

    // 顶点shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);        // 创建shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 设置源代码
    glCompileShader(vertexShader);                              // 编译shader
    // 检查 顶点shader 编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "错误::顶点Shader::编译错误\n"
             << infoLog << endl;
    }

    // 片元 shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 检查 片元shader 编译错误
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "错误::片元Shader::编译错误\n"
             << infoLog << endl;
    }

    // 链接 shader
    int shaderProgram = glCreateProgram();         // 创建 shader 程序
    glAttachShader(shaderProgram, vertexShader);   // 设置顶点shader
    glAttachShader(shaderProgram, fragmentShader); // 设置片元shader
    glLinkProgram(shaderProgram);                  // 链接 shader 程序

    // 检查链接错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "错误::Shader::程序::链接失败\n"
             << infoLog << endl;
    }

    // 链接后释放已经不需要的 shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 已经把VBO注册到顶点属性,这里可以安全的解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 这里可以解绑 VAO 防止别的 VAO 调用修改这个 VAO, 但这种情况一般不会发生,因为修改 VAO 需要调用 glBindVertexArray(), 如果不是必须的一般不解绑 VAO 或 VBO
    glBindVertexArray(0);
    // 开关绘制线框
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    char *vsPath = "/src/7.2 Text Rendering/text_rendering.vert";
    char *fsPath = "/src/7.2 Text Rendering/text_rendering.frag";
    ShaderLoader shader(vsPath, fsPath, nullptr);     // 受光物体shader程序

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    FT_Face face;
    string facePath = concatString(getcwd(NULL, 0), "/res/font/msyh.ttc");
    if (FT_New_Face(ft, facePath.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face,c,FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RED,face->glyph->bitmap.width,face->glyph->bitmap.rows,0,GL_RED,GL_UNSIGNED_BYTE,face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        Character character = {
            texture,
            vec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
            vec2(face->glyph->bitmap_left,face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(pair<char,Character>(c,character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // mat4 projection = ortho(0.0f, 800.0f, 0.0f, 600.0f);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER,textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        //处理渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置状态
        glClear(GL_COLOR_BUFFER_BIT);         // 使用状态

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        RenderText(shader,"Hello Triangle",10,550,1,toFloatColor("FFFFFF"));
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

void RenderText(ShaderLoader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // 激活对应的渲染状态
    s.use();
    glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    // 遍历文本中所有的字符
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // 在四边形上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}