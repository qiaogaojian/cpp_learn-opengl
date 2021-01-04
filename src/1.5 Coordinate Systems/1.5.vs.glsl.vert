#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float offsetX;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vec3(aPos.x+offsetX,aPos.y,aPos.z), 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}