#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT
{
    vec2 TexCoord;
} vs_out;

uniform float offsetX;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(vec3(aPos.x+offsetX,aPos.y,aPos.z), 1.0);
    // gl_PointSize = gl_Position.z;
	vs_out.TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}