#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_loader.h>
#include <camera.h>
#include <model.h>
#include <tools.h>
#include <direct.h>
#include <iostream>

using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
vec3 cameraPos = vec3(1.0f, 1.0f, 3.0f);
Camera camera(cameraPos, vec3(0.0f, 1.0f, 0.0f), -100, 0);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

vec3 lightPos(1.2f, 1.0f, 2.0f);

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

int main()
{
    // ��ʼ�������� glfw
    //--------------------------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // �������ڶ���
    //--------------------------------------------------------------------------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // ���ô��ڻص��¼�
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // �������

    // ��ʼ�� glad  �������� opengl �ĺ���ָ��
    //--------------------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // �����ͱ��� shader ����
    //--------------------------------------------------------------------------------------
    char *vsPath = "/src/4.10 Instancing/planet.vert";
    char *fsPath = "/src/4.10 Instancing/planet.frag";
    ShaderLoader ourShader(vsPath, fsPath);
    char *fsLightPath = "/src/2.3 Materials/light.frag";
    ShaderLoader shaderLight(vsPath, fsLightPath, nullptr); // ��������shader����
    vsPath = "/src/4.10 Instancing/rock.vert";
    fsPath = "/src/4.10 Instancing/rock.frag";
    ShaderLoader shaderRock(vsPath, fsPath);

    // ����ģ��
    //--------------------------------------------------------------------------------------
    stbi_set_flip_vertically_on_load(true); // ����ģ��ǰ��ת����Y��
    char *modelPath = "/res/model/planet/planet.obj";
    Model modelPlanet(concatString(_getcwd(NULL, 0), modelPath));
    modelPath = "/res/model/rock/rock.obj";
    Model modelRock(concatString(_getcwd(NULL, 0), modelPath));

    unsigned int amout = 100000;
    mat4* modelMatrices;
    modelMatrices = new mat4[amout];
    srand(glfwGetTime());
    float radius = 50.0f;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amout; i++)
    {
        mat4 model = mat4(1.0f);
        // 1. λ��: �ֲ��뾶Ϊradius��Բ����, ƫ�Ʒ�Χ�� [-offset, offset]
        float angle = (float)i/(float)amout*360.0f;
        float displacement = (rand()%(int)(2*offset*100))/100.0f - offset;
        float x = cos(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = sin(angle) * radius + displacement;
        model = translate(model, vec3(x,y,z));

        // 2. ����: ��0.05 �� 0.25 ֮������
        float sca = (rand()%20) / 100.0f + 0.05;
        model = scale(model, vec3(sca));

        // 3. ��ת: ����һ�����ѡ�����ת������������ת
        float rot = (rand() % 360);
        model = rotate(model, rot, vec3(0.4f, 0.6f, 0.8f));

        // 4. ���ӵ������������
        modelMatrices[i] = model;
    }

    unsigned int modelVBO;
    glGenBuffers(1, &modelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, amout*sizeof(mat4), &modelMatrices[0],GL_STATIC_DRAW);

    for (unsigned int i = 0; i < modelRock.meshes.size(); i++)
    {
        unsigned int VAO = modelRock.meshes[i].VAO;
        glBindVertexArray(VAO);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE, sizeof(mat4),(void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE, sizeof(mat4),(void*)(1*sizeof(vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE, sizeof(mat4),(void*)(2*sizeof(vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6,4,GL_FLOAT,GL_FALSE, sizeof(mat4),(void*)(3*sizeof(vec4)));

        glVertexAttribDivisor(3,1);
        glVertexAttribDivisor(4,1);
        glVertexAttribDivisor(5,1);
        glVertexAttribDivisor(6,1);

        glBindVertexArray(0);
    }

    // ����������
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

    // ����OpenGLȫ��״̬
    //--------------------------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ourShader.use();
    ourShader.setFloat("light.constant", 1.0f);
    ourShader.setFloat("light.linear", 0.09f);
    ourShader.setFloat("light.quadratic", 0.032f);

    while (!glfwWindowShouldClose(window))
    {
        // ֡���ʱ��
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ��������
        processInput(window);

        // ������Ⱦ
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ���� uniform ֮ǰҪ�ȼ���shader
        ourShader.use();

        // ��������(�������͹��յ���ɫ�ͷ����)
        ourShader.setFloat("material.shininess", 0.25f * 32);
        // ��������(����λ�ú͹���ǿ��)
        ourShader.setVec3("light.ambient", vec3(1.0f));
        ourShader.setVec3("light.diffuse", vec3(1.0f));
        ourShader.setVec3("light.specular", vec3(1.0f));
        ourShader.setVec4("light.vector", vec4(lightPos, 1.0f));

        ourShader.setVec3("viewPos", camera.Position);
        mat4 projection = mat4(1.0f);
        projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", camera.GetViewMatrix());

        mat4 model = mat4(1.0f);
        model = translate(model, vec3(0.0f, 0.0f, 0.0f));
        model = scale(model, vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", model);
        ourShader.setMat3("normalMat", transpose(inverse(model)));

        modelPlanet.Draw(ourShader);

        // ����С����
        shaderRock.use();
        shaderRock.setMat4("projection", projection);
        shaderRock.setMat4("view", camera.GetViewMatrix());

        shaderRock.setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,modelRock.textures_loaded[0].id);
        for (unsigned int i = 0; i < modelRock.meshes.size(); i++)
        {
            glBindVertexArray(modelRock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, modelRock.meshes[i].indices.size(),GL_UNSIGNED_INT,0,amout);
            glBindVertexArray(0);
        }

        // ���Ƶ�
        shaderLight.use();

        vec3 lightColor = vec3(1.0f);
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        shaderLight.setVec3("lightColor", vec3(1.0f));
        shaderLight.setMat4("projection", projection);
        shaderLight.setMat4("view", camera.GetViewMatrix());
        mat4 modelLight = mat4(1.0f);
        modelLight = translate(modelLight, lightPos);
        modelLight = scale(modelLight, vec3(0.2f));
        shaderLight.setMat4("model", modelLight);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // �ͷ���Դ
    // ------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// ���ڴ�С�ı�ʱ �����ӿڴ�С
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
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