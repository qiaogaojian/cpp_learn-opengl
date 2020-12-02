#version 330 core
layout(location = 0) in vec3 aPos;		// λ�ñ���������λ��ֵΪ0
layout(location = 1) in vec3 aColor;	// ��ɫ����������λ��ֵΪ1
layout(location = 2) in vec2 aTexCoord; // �������������λ��ֵΪ2

out vec3 outColor;						// ��fragment shader���һ����ɫ
out vec2 texCoord;

uniform float offsetX;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	outColor = aColor;						// ��outColor����Ϊ�Ӷ�����������õ���������ɫ
	gl_Position = projection * view * model * vec4(aPos,1.0);
	texCoord = vec2(aTexCoord.x,aTexCoord.y);
};