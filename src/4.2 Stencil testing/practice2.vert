#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat; // 法线矩阵 用来消除不同比例缩放对法线方向的影响
uniform float scale;

void main()
{
    // 模型的原点不在中心点时, 放大后的描边和原来模型会有误差,需要沿法线方向平移每个顶点
	gl_Position = projection * view * model * vec4(aPos + aNormal * scale, 1.0);
    Normal = normalMat * aNormal;
    FragPos = vec3(model * vec4(aPos,1.0));
    TexCoord = aTexCoord;
}