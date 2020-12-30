#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_loader.h"
#include "camera.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <direct.h>
#include <iostream>
using namespace std;
using namespace glm;

const unsigned int SCR_WIDTH = 800;  // ��Ļ���
const unsigned int SCR_HEIGHT = 600; // ��Ļ�߶�
const unsigned int SHADOW_WIDTH = 1000; // ��Ļ�߶�
const unsigned int SHADOW_HEIGHT = 1000; // ��Ļ�߶�

vec3 cameraPos = vec3(1.0f, 0, 5.0f);
Camera camera(cameraPos, vec3(0.0f, 1.0f, 0.0f), -100, 0);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
bool isFirstCursor = true;

float verticesFloor[] = {
    // positions            // normals         // texcoords
    10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
    -10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,

    10.0f, -0.5f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
    -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
    10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f};

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};

vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int loadTexture(char const * path);
void renderFloor(ShaderLoader shaderObject);
void renderCube (ShaderLoader shaderObject);
void renderLight(ShaderLoader shaderLight);

unsigned int texture;
unsigned int texture_specular;
unsigned int texture_floor;
unsigned VAO;
unsigned VBO;
unsigned VAOfloor;
unsigned VBOfloor;

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
    char *vsPath = "/src/5.3 Shadow Mapping/shadow_mapping_directional_light.vs";
    char *fsPath = "/src/5.3 Shadow Mapping/shadow_mapping_directional_light.fs";
    char *fsLightPath = "/src/2.3 Materials/light.fs";
    ShaderLoader shaderObject(vsPath, fsPath, nullptr);     // �ܹ�����shader����
    ShaderLoader shaderLight(vsPath, fsLightPath, nullptr); // ��������shader����

    // ���ö������� ���ö�������
    //--------------------------------------------------------------------------------------
    // �ܹ����建������
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
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ��һ������ ��������ͼ
    string texPath = shaderObject.concatString(getcwd(NULL, 0), "/res/texture/box2.png");
    texture = loadTexture(texPath.c_str());

    // �ڶ������� ���淴����ͼ
    texPath = shaderObject.concatString(getcwd(NULL, 0), "/res/texture/box2_specular.png");
    texture_specular = loadTexture(texPath.c_str());

    texPath = shaderObject.concatString(getcwd(NULL, 0), "/res/texture/wood.png");
    texture_floor = loadTexture(texPath.c_str());

    glGenVertexArrays(1, &VAOfloor);
    glGenBuffers(1, &VBOfloor);
    glBindVertexArray(VAOfloor);
    glBindBuffer(GL_ARRAY_BUFFER, VBOfloor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFloor), verticesFloor, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    unsigned depthFramebuffer;
    glGenFramebuffers(1, &depthFramebuffer);

    unsigned depthTexMap;
    glGenTextures(1, &depthTexMap);
    glBindTexture(GL_TEXTURE_2D, depthTexMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);
    // glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexMap,0);
    // glDrawBuffer(GL_NONE);
    // glReadBuffer(GL_NONE);

    shaderObject.use();
    shaderObject.setInt("material.diffuse",0);  // �������������id
    shaderObject.setInt("material.specular",1); // ���ø߹ⷴ�����id

    shaderObject.setFloat("light.constant",  1.0f);
    shaderObject.setFloat("light.linear",    0.09f);
    shaderObject.setFloat("light.quadratic", 0.032f);

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

        // glViewport(0,0,SHADOW_WIDTH,SHADOW_HEIGHT);
        // glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);

        renderFloor(shaderObject);
        renderCube(shaderObject);
        renderLight(shaderLight);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ʹ��״̬
        // glBindTexture(GL_TEXTURE_2D,depthTexMap);

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

void renderCube(ShaderLoader shaderObject){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        // �����˼���ڶ�������
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_specular);

        // ��������
        shaderObject.use();

        vec3 lightColor = vec3(1.0f);
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        vec3 ambientColor = lightColor * 0.2f;
        vec3 spotColor = vec3(0.5f, 0.5f, 0.5f);

        // ��������(�������͹��յ���ɫ�ͷ����)
        shaderObject.setFloat("material.shininess", 0.25f * 128);
        // ��������(����λ�ú͹���ǿ��)
        shaderObject.setVec3("light.ambient", vec3(1.0f));
        shaderObject.setVec3("light.diffuse", vec3(1.0f));
        shaderObject.setVec3("light.specular", vec3(1.0f));
        shaderObject.setVec4("light.direction", -vec4(lightPos, 0.0f));

        shaderObject.setVec3("viewPos", camera.Position);
        mat4 projection = mat4(1.0f);
        projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        shaderObject.setMat4("projection", projection);
        shaderObject.setMat4("view", camera.GetViewMatrix());

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; i++)
        {
            mat4 model = mat4(1.0f);
            model = translate(model, cubePositions[i]);
            model = rotate(model, radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderObject.setMat4("model", model);
            shaderObject.setMat3("normalMat", transpose(inverse(model)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
}


void renderFloor(ShaderLoader shaderObject){
        // ��������
        shaderObject.use();

        vec3 lightColor = vec3(1.0f);
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        vec3 ambientColor = lightColor * 0.2f;
        vec3 spotColor = vec3(0.5f, 0.5f, 0.5f);

        // ��������(�������͹��յ���ɫ�ͷ����)
        shaderObject.setFloat("material.shininess", 0.25f * 128);
        // ��������(����λ�ú͹���ǿ��)
        shaderObject.setVec3("light.ambient", vec3(1.0f));
        shaderObject.setVec3("light.diffuse", vec3(1.0f));
        shaderObject.setVec3("light.specular", vec3(1.0f));
        shaderObject.setVec4("light.direction", -vec4(lightPos, 0.0f));

        shaderObject.setVec3("viewPos", camera.Position);
        mat4 projection = mat4(1.0f);
        projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        shaderObject.setMat4("projection", projection);
        shaderObject.setMat4("view", camera.GetViewMatrix());

        shaderObject.use();
        mat4 model = mat4(1.0f);
        shaderObject.setMat4("model", model);
        shaderObject.setMat3("normalMat", transpose(inverse(model)));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_floor);

        glBindVertexArray(VAOfloor);
        glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderLight(ShaderLoader shaderLight){
        vec3 lightColor = vec3(1.0f);
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        vec3 ambientColor = lightColor * 0.2f;
        vec3 spotColor = vec3(0.5f, 0.5f, 0.5f);

        // ���Ƶ�
        mat4 projection = mat4(1.0f);
        projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        shaderLight.use();
        shaderLight.setVec3("lightColor", vec3(1.0f));
        shaderLight.setMat4("projection", projection);
        shaderLight.setMat4("view", camera.GetViewMatrix());
        mat4 modelLight = mat4(1.0f);
        modelLight = translate(modelLight, lightPos);
        modelLight = scale(modelLight, vec3(0.2f));
        shaderLight.setMat4("model", modelLight);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
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

// ��·�����ز��� ���ز���ID
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}