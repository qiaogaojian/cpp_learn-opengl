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

const unsigned int SCR_WIDTH = 800;  // ��Ļ���
const unsigned int SCR_HEIGHT = 600; // ��Ļ�߶�

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
    unsigned int TextureID; // ���������ID
    vec2 Size;              // ���δ�С
    vec2 Bearing;           // �ӻ�׼�ߵ���/������ƫ��ֵ
    unsigned int Advance;   // ԭ�㵽��һ�����εľ���
};

map<char, Character> characters;
GLuint textVAO,textVBO;

int main()
{
    // ��ʼ�������� glfw
    //--------------------------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // ���ʹ�õ���Mac OS Xϵͳ ��Ҫ��ע�����д���

    // �������ڶ���
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ��ʼ�� glad  �������� opengl �ĺ���ָ��
    //--------------------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // �����ͱ��� shader ����
    //--------------------------------------------------------------------------------------

    // ����shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);        // ����shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // ����Դ����
    glCompileShader(vertexShader);                              // ����shader
    // ��� ����shader �������
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "����::����Shader::�������\n"
             << infoLog << endl;
    }

    // ƬԪ shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // ��� ƬԪshader �������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "����::ƬԪShader::�������\n"
             << infoLog << endl;
    }

    // ���� shader
    int shaderProgram = glCreateProgram();         // ���� shader ����
    glAttachShader(shaderProgram, vertexShader);   // ���ö���shader
    glAttachShader(shaderProgram, fragmentShader); // ����ƬԪshader
    glLinkProgram(shaderProgram);                  // ���� shader ����

    // ������Ӵ���
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "����::Shader::����::����ʧ��\n"
             << infoLog << endl;
    }

    // ���Ӻ��ͷ��Ѿ�����Ҫ�� shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ���ö������� ���ö�������
    //--------------------------------------------------------------------------------------
    unsigned int VAO;
    unsigned int VBO;
    // ���� VAO VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // ���Ȱ�VAO
    glBindVertexArray(VAO);
    // Ȼ��󶨲�����VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Ȼ�����ö�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // �Ѿ���VBOע�ᵽ��������,������԰�ȫ�Ľ��
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // ������Խ�� VAO ��ֹ��� VAO �����޸���� VAO, ���������һ�㲻�ᷢ��,��Ϊ�޸� VAO ��Ҫ���� glBindVertexArray(), ������Ǳ����һ�㲻��� VAO �� VBO
    glBindVertexArray(0);
    // ���ػ����߿�
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    char *vsPath = "/src/7.2 Text Rendering/text_rendering.vert";
    char *fsPath = "/src/7.2 Text Rendering/text_rendering.frag";
    ShaderLoader shader(vsPath, fsPath, nullptr);     // �ܹ�����shader����

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
        // ��������
        processInput(window);

        //������Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ����״̬
        glClear(GL_COLOR_BUFFER_BIT);         // ʹ��״̬

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        RenderText(shader,"Hello Triangle",10,550,1,toFloatColor("FFFFFF"));
        // ��鲢�����¼�������������ɻ���
        glfwPollEvents();        // �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
        glfwSwapBuffers(window); // ˫���彻����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
    }

    // �ͷ���Դ
    //--------------------------------------------------------------------------------------

    // �ͷ�GLFW��Դ
    glfwTerminate(); // ����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ ���������������е���Դ����ȷ���˳�Ӧ�ó���
    return 0;
}

// ���ڴ�С�ı�ʱ �����ӿڴ�С
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// ��������
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void RenderText(ShaderLoader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // �����Ӧ����Ⱦ״̬
    s.use();
    glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    // �����ı������е��ַ�
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // ��ÿ���ַ�����VBO
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // ���ı����ϻ�����������
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // ����VBO�ڴ������
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // �����ı���
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
        x += (ch.Advance >> 6) * scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}