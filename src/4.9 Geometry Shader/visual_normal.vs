#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoord;

out VS_OUT{
    vec4 Normal;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position=projection*view*model*vec4(aPos,1.);
    mat4 normalMatrix = mat4(transpose(inverse(view * model)));
    vs_out.Normal= normalize(projection * normalMatrix * vec4(aNormal, 0.0));
}