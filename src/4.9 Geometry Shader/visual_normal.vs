#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoord;

out VS_OUT{
    vec3 Normal;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;// 法线矩阵 用来消除不同比例缩放对法线方向的影响

void main()
{
    gl_Position=projection*view*model*vec4(aPos,1.);
    vs_out.Normal= normalize(vec3(projection * vec4((normalMat*aNormal), 0.0)));
}