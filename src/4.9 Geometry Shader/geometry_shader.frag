#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec2 TexCoord;
} fs_in;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alpha;

in vec3 fColor;

void main()
{
    FragColor = vec4(fColor, 1.0);
}