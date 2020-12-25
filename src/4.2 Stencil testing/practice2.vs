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
uniform mat3 normalMat; // ���߾��� ����������ͬ�������ŶԷ��߷����Ӱ��
uniform float scale;

void main()
{
    // ģ�͵�ԭ�㲻�����ĵ�ʱ, �Ŵ�����ߺ�ԭ��ģ�ͻ������,��Ҫ�ط��߷���ƽ��ÿ������
	gl_Position = projection * view * model * vec4(aPos + aNormal * scale, 1.0);
    Normal = normalMat * aNormal;
    FragPos = vec3(model * vec4(aPos,1.0));
    TexCoord = aTexCoord;
}