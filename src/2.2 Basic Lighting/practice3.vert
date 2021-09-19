#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 lightPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat; // 法线矩阵 用来消除不同比例缩放对法线方向的影响
uniform vec3 lightPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = normalMat * aNormal;
    FragPos = vec3(view * model * vec4(aPos,1.0));
    vec4 lp = view * vec4(lightPos,1.0) ;
    lightPosition = vec3(lp.x,lp.y,lp.z);
}