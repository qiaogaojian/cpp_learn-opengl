#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
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

float vertices[] =
{
    // ��һ��������
     -0.5f, -0.5f,  0.0f,
    -0.5f,  0.5f,  0.0f,
     0.0f,  0.0f,  0.0f
};

float vertices2[] =
{
    // �ڶ���������
     0.5f, -0.5f,  0.0f,
     0.5f,  0.5f,  0.0f,
     0.0f,  0.0f,  0.0f
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

    // ��һ��������
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

    // �ڶ���������
    unsigned int VAO2;
    unsigned int VBO2;
    glGenVertexArrays(1,&VAO2);
    glGenBuffers(1,&VBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER,VBO2);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices2),vertices2,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
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

        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

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