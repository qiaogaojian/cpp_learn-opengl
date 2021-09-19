#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_loader.h"
#include "stb_image.h"

#include <direct.h>
#include <iostream>
using namespace std;

const unsigned int SCR_WIDTH = 800;  // ��Ļ���
const unsigned int SCR_HEIGHT = 600; // ��Ļ�߶�

float vertices[] = {
    //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
    0.5f,  0.5f, 0.0f,        1.0f, 0.0f, 0.0f,   0.6f, 0.6f,  // ����
    0.5f, -0.5f, 0.0f,        0.0f, 1.0f, 0.0f,   0.6f, 0.5f,  // ����
   -0.5f, -0.5f, 0.0f,        0.0f, 0.0f, 1.0f,   0.5f, 0.5f,  // ����
   -0.5f,  0.5f, 0.0f,        1.0f, 1.0f, 0.0f,   0.5f, 0.6f   // ����
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

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
    char *vsPath = "/src/1.3 Textures/1.3.vs.glsl.vert";
    char *fsPath = "/src/1.3 Textures/1.3.fs.glsl.frag";
    ShaderLoader shaderLoader(vsPath, fsPath, nullptr);

    // ���ö������� ���ö�������
    //--------------------------------------------------------------------------------------
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    // ���� VAO VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // ���Ȱ�VAO
    glBindVertexArray(VAO);
    // Ȼ��󶨲�����VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Ȼ��󶨲�����EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Ȼ�����ö�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ���ز���
    //--------------------------------------------------------------------------------------
    // ��һ������
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Ϊ��ǰ�󶨵�����������û��� ���˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // ���ز���������
    int width;
    int height;
    int nrChannels;
    stbi_set_flip_vertically_on_load(true); // OpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ���,����������·�ת
    string texPath = shaderLoader.concatString(getcwd(NULL, 0), "/res/texture/box.jpg");
    unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "���ʼ���ʧ��" << endl;
    }
    stbi_image_free(data);

    // �ڶ�������
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Ϊ��ǰ�󶨵�����������û��� ���˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������

    texPath = shaderLoader.concatString(getcwd(NULL, 0), "/res/texture/smile.png");
    data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // ע������Ĳ�����͸���� ��Alphaͨ�� Ҫ��GL_RGBA��ʽ
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "���ʼ���ʧ��" << endl;
    }
    stbi_image_free(data);

    shaderLoader.use();
    shaderLoader.setInt("texture1", 0);
    shaderLoader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window))
    {
        // ��������
        processInput(window);

        //������Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ����״̬
        glClear(GL_COLOR_BUFFER_BIT);         // ʹ��״̬

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shaderLoader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // ��鲢�����¼�������������ɻ���
        glfwPollEvents();        // �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
        glfwSwapBuffers(window); // ˫���彻����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
    }

    // �ͷ���Դ
    //--------------------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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