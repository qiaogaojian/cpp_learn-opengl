#version 330 core

in vec3 posColor;
out vec4 FragColor;

uniform vec4 colorC;

void main()
{
    FragColor=vec4(posColor,1.f);
}