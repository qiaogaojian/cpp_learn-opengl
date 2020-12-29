
#version 330 core

layout(location=0)in vec2 aPos;
layout(location=1)in vec3 aColor;
layout(location=2)in vec2 offset;

out vec3 posColor;

uniform vec2 offsets[100];

void main()
{
    gl_Position=vec4(aPos+offset,0.0,1.f);
    posColor = aColor;
}