#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 resColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat; // ���߾��� ����������ͬ�������ŶԷ��߷����Ӱ��

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 Normal = normalMat * aNormal;
    vec3 FragPos = vec3(model * vec4(aPos,1.0));

    // ������
    float ambientStrength=.1;
    vec3 ambient=ambientStrength*lightColor;

    // ������
    vec3 normalDir=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=diff*lightColor;

    // ���淴��
    float specularStrength=.5;
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normalDir);// ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
    float spec=pow(max(dot(viewDir,reflectDir),0),32);
    vec3 specular=specularStrength*spec*lightColor;

    resColor=(ambient+diffuse+specular)*objectColor;
}

/*
So what do we see?
You can see (for yourself or in the provided image) the clear distinction of the two triangles at the front of the
cube. This 'stripe' is visible because of fragment interpolation. From the example image we can see that the top-right
vertex of the cube's front face is lit with specular highlights. Since the top-right vertex of the bottom-right triangle is
lit and the other 2 vertices of the triangle are not, the bright values interpolates to the other 2 vertices. The same
happens for the upper-left triangle. Since the intermediate fragment colors are not directly from the light source
but are the result of interpolation, the lighting is incorrect at the intermediate fragments and the top-left and
bottom-right triangle collide in their brightness resulting in a visible stripe between both triangles.

This effect will become more apparent when using more complicated shapes.
*/