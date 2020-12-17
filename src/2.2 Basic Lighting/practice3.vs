#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 lightPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat; // ���߾��� ����������ͬ�������ŶԷ��߷����Ӱ��
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = normalMat * aNormal;
    FragPos = vec3(view * model * vec4(aPos,1.0));
    vec4 lp = view * vec4(lightPos,1.0) ;
    vec4 vp = view * vec4(viewPos,1.0);
    lightPosition = vec3(lp.x,lp.y,lp.z);
}