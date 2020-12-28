#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 fragPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat; // 法线矩阵 用来消除不同比例缩放对法线方向的影响

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.normal = normalMat * aNormal;
    vs_out.fragPos = vec3(model * vec4(aPos,1.0));
    vs_out.texCoords = aTexCoord;
}