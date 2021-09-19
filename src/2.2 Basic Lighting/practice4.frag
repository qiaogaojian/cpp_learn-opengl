#version 330 core
in vec3 resColor;

out vec4 FragColor;

void main()
{
    FragColor=vec4(resColor,1.);
}