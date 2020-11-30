#version 330 core
layout(location = 0) in vec3 aPos;		// λ�ñ���������λ��ֵΪ0
layout(location = 1) in vec3 aColor;	// ��ɫ����������λ��ֵΪ1
out vec3 outColor;						// ��fragment shader���һ����ɫ
uniform float offsetX;
void main()
{
	gl_Position = vec4(aPos.x + offsetX,aPos.y,aPos.z,1.0);
	outColor = aColor;					// ��outColor����Ϊ�Ӷ�����������õ���������ɫ
};