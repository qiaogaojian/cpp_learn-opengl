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

const unsigned int SCR_WIDTH = 800;  // 屏幕宽度
const unsigned int SCR_HEIGHT = 600; // 屏幕高度

vec3 cameraPos = vec3(1.0f, 0, 5.0f);
Camera camera(cameraPos, vec3(0.0f, 1.0f, 0.0f), -100, 0);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
bool isFirstCursor = true;

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

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
    char *vsPath = "/src/2.3 Materials/materials.vert";
    char *fsPath = "/src/2.3 Materials/materials.frag";
    char *fsLightPath = "/src/2.3 Materials/light.frag";
    ShaderLoader shaderObject(vsPath, fsPath, nullptr);     // 受光物体shader程序
    ShaderLoader shaderLight(vsPath, fsLightPath, nullptr); // 发光物体shader程序

    // 设置顶点数据 配置顶点属性
    //--------------------------------------------------------------------------------------
    // 受光物体缓冲数据
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 发光物体缓冲数据 // 不用也行 只要 shader program是独立的就不会影响到发光物体
    // unsigned int lightVAO;
    // unsigned int lightVBO;
    // // 生成 VAO VBO
    // glGenVertexArrays(1, &lightVAO);
    // glGenBuffers(1, &lightVBO);
    // // 首先绑定VAO
    // glBindVertexArray(lightVAO);
    // // 然后绑定并设置VBO
    // glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // // 然后设置顶点属性
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0));
    // glEnableVertexAttribArray(0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);
        float time = (float)glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;

        //处理渲染
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // 设置状态
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 使用状态
        glEnable(GL_DEPTH_TEST);

        // 绘制物体
        shaderObject.use();

        vec3 lightColor = vec3(1.0f);
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        vec3 ambientColor = lightColor * 0.2f;
        vec3 spotColor = vec3(0.5f, 0.5f, 0.5f);
        // 材质表格                       环境光                                   漫反射                                   镜面反射                    反光度
        // 绿宝石
        // emerald	        0.0215,	    0.1745	    0.0215	        0.07568	    0.61424	    0.07568	        0.633	    0.727811	0.633	        0.6
        // 翡翠
        // jade	            0.135	    0.2225	    0.1575	        0.54	    0.89	    0.63	        0.316228	0.316228	0.316228	    0.1
        // 黑曜石
        // obsidian	        0.05375	    0.05	    0.06625	        0.18275	    0.17	    0.22525	        0.332741	0.328634	0.346435	    0.3
        // 珍珠
        // pearl	        0.25	    0.20725	    0.20725	        1	        0.829	    0.829	        0.296648	0.296648	0.296648	    0.088
        // 红宝石
        // ruby	            0.1745	    0.01175	    0.01175	        0.61424	    0.04136	    0.04136	        0.727811	0.626959	0.626959	    0.6
        // 绿松石
        // turquoise	    0.1	        0.18725	    0.1745	        0.396	    0.74151	    0.69102	        0.297254	0.30829	    0.306678	    0.1
        // 黄铜
        // brass	        0.329412	0.223529	0.027451	    0.780392	0.568627	0.113725	    0.992157	0.941176	0.807843	    0.21794872
        // 青铜
        // bronze	        0.2125	    0.1275	    0.054	        0.714	    0.4284	    0.18144	        0.393548	0.271906	0.166721	    0.2
        // 铬合金
        // chrome	        0.25	    0.25	    0.25	        0.4	        0.4	        0.4	            0.774597	0.774597	0.774597	    0.6
        // 铜
        // copper	        0.19125	    0.0735	    0.0225	        0.7038	    0.27048	    0.0828	        0.256777	0.137622	0.086014	    0.1
        // 金
        // gold	            0.24725	    0.1995	    0.0745	        0.75164	    0.60648	    0.22648	        0.628281	0.555802	0.366065	    0.4
        // 银
        // silver	        0.19225	    0.19225	    0.19225	        0.50754	    0.50754	    0.50754	        0.508273	0.508273	0.508273	    0.4
        // 黑色塑料
        // black plastic    0.0	        0.0	        0.0	            0.01	    0.01	    0.01	        0.50	    0.50	    0.50	        0.25
        // 青色塑料
        // cyan plastic	    0.0	        0.1	        0.06	        0.0	        0.50980392	0.50980392	    0.50196078	0.50196078	0.50196078	    0.25
        // 绿色塑料
        // green plastic	0.0	        0.0	        0.0	            0.1	        0.35	    0.1	            0.45	    0.55	    0.45	        0.25
        // 红色塑料
        // red plastic	    0.0	        0.0	        0.0	            0.5	        0.0	        0.0	            0.7	        0.6	        0.6	            0.25
        // 白色塑料
        // white plastic	0.0	        0.0	        0.0	            0.55	    0.55	    0.55	        0.70	    0.70	    0.70	        0.25
        // 黄色塑料
        // yellow plastic	0.0	        0.0	        0.0	            0.5	        0.5	        0.0	            0.60	    0.60	    0.50	        0.25
        // 黑色橡胶
        // black rubber	    0.02	    0.02	    0.02	        0.01	    0.01	    0.01	        0.4	        0.4	        0.4	            0.078125
        // 青色橡胶
        // cyan rubber	    0.0	        0.05	    0.05	        0.4	        0.5	        0.5	            0.04	    0.7	        0.7	            0.078125
        // 绿色橡胶
        // green rubber	    0.0	        0.05	    0.0	            0.4	        0.5	        0.4	            0.04	    0.7	        0.04	        0.078125
        // 红色橡胶
        // red rubber	    0.05	    0.0	        0.0	            0.5	        0.4	        0.4	            0.7	        0.04	    0.04	        0.078125
        // 白色橡胶
        // white rubber	    0.05	    0.05	    0.05	        0.5	        0.5	        0.5	            0.7	        0.7	        0.7	            0.078125
        // 黄色橡胶
        // yellow rubber	0.05	    0.05	    0.0	            0.5	        0.5	        0.4	            0.7	        0.7	        0.04	        0.078125

        // 材质设置(各个类型光照的颜色和反光度)
        shaderObject.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
        shaderObject.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
        shaderObject.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
        shaderObject.setFloat("material.shininess", 0.25f * 128);
        // 光照设置(光照位置和光照颜色)
        shaderObject.setVec3("light.ambient", vec3(1.0f));
        shaderObject.setVec3("light.diffuse", vec3(1.0f));
        shaderObject.setVec3("light.specular", vec3(1.0f));
        shaderObject.setVec3("light.position", lightPos);

        shaderObject.setVec3("viewPos", camera.Position);
        mat4 projection = mat4(1.0f);
        projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        shaderObject.setMat4("projection", projection);
        shaderObject.setMat4("view", camera.GetViewMatrix());

        glBindVertexArray(VAO);
        for (int i = 0; i < 1; i++)
        {
            mat4 model = mat4(1.0f);
            model = translate(model, cubePositions[i]);
            model = rotate(model, radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderObject.setMat4("model", model);
            shaderObject.setMat3("normalMat", transpose(inverse(model)));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 绘制灯
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

        // 检查并调用事件，交换缓冲完成绘制
        glfwPollEvents();        // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwSwapBuffers(window); // 双缓冲交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
    }

    // 释放资源
    //--------------------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteVertexArrays(1, &lightVAO);
    // glDeleteBuffers(1, &lightVBO);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    // 注意，我们对右向量进行了标准化。如果我们没对这个向量进行标准化，最后的叉乘结果会根据cameraFront变量返回大小不同的向量。
    // 如果我们不对向量进行标准化，我们就得根据摄像机的朝向不同加速或减速移动了，但如果进行了标准化移动就是匀速的。
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
