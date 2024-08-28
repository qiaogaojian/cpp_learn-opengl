#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat; // ���߾��� ����������ͬ�������ŶԷ��߷����Ӱ��

void main()
{
	gl_Position = projection * view * model * vec4(aPos + aNormal * 0.1, 1.0);  // �ط��߷���ƽ��
    Normal = normalMat * aNormal;
    FragPos = vec3(model * vec4(aPos,1.0));
}