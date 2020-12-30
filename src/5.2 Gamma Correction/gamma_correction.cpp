#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_loader.h"
#include "camera.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <direct.h>
#include <iostream>
#include "tools.h"
using namespace std;
using namespace glm;

const unsigned int SCR_WIDTH = 800;  // ��Ļ���
const unsigned int SCR_HEIGHT = 600; // ��Ļ�߶�

vec3 cameraPos = vec3(1.0f, 0, 5.0f);
Camera camera(cameraPos, vec3(0.0f, 1.0f, 0.0f), -100, 0);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
bool isFirstCursor = true;
bool isBlinn = false;
bool isPressing = false;

float vertices[] = {
    // positions            // normals         // texcoords
     10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
    -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

     10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
    -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
     10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
};

vec3 lightPos(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

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
    char *vsPath = "/src/5.2 Gamma Correction/gamma_correction.vs";
    char *fsPath = "/src/5.2 Gamma Correction/gamma_correction.fs";
    char *fsLightPath = "/src/2.2 Basic Lighting/2.2.fs.lamp.glsl";
    ShaderLoader shaderObject(vsPath, fsPath, nullptr);     // �ܹ�����shader����
    ShaderLoader shaderLight(vsPath, fsLightPath, nullptr); // ��������shader����

    // ���ö������� ���ö�������
    //--------------------------------------------------------------------------------------
    // �ܹ����建������
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    string texPath = shaderObject.concatString(getcwd(NULL, 0), "/res/texture/wood.png");
    unsigned int texture = loadTexture(texPath.c_str());

    shaderObject.use();
    shaderObject.setVec3("lightColor", vec3(1.0f, 1.0f, 1.0f));
    shaderObject.setVec3("lightPos",lightPos);
    shaderObject.setInt("texture_diffuse",0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // �������
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        // ��������
        processInput(window);
        float time = (float)glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        //������Ⱦ
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // ����״̬
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ʹ��״̬
        glEnable(GL_DEPTH_TEST);

        // ��������
        shaderObject.use();
        shaderObject.setBool("blinn",isBlinn);
        shaderObject.setVec3("viewPos",camera.Position);
        mat4 projection = mat4(1.0f);
        projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        shaderObject.setMat4("projection", projection);
        shaderObject.setMat4("view", camera.GetViewMatrix());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // ��鲢�����¼�������������ɻ���
        glfwPollEvents();        // �����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
        glfwSwapBuffers(window); // ˫���彻����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ��
    }

    // �ͷ���Դ
    //--------------------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteVertexArrays(1, &lightVAO);
    // glDeleteBuffers(1, &lightVBO);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    // ע�⣬���Ƕ������������˱�׼�����������û������������б�׼�������Ĳ�˽�������cameraFront�������ش�С��ͬ��������
    // ������ǲ����������б�׼�������Ǿ͵ø���������ĳ���ͬ���ٻ�����ƶ��ˣ�����������˱�׼���ƶ��������ٵġ�
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!isPressing)
        {
            isBlinn = !isBlinn;
        }
        isPressing = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        isPressing = false;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (isFirstCursor)
    {
        lastX = xpos;
        lastY = ypos;
        isFirstCursor = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
