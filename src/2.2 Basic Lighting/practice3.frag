#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec3 lightPosition;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    // ������
    float ambientStrength=.1;
    vec3 ambient=ambientStrength*lightColor;

    // ������
    vec3 normalDir=normalize(Normal);
    vec3 lightDir=normalize(lightPosition-FragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=diff*lightColor;

    // ���淴��
    float specularStrength=.5;
    vec3 viewDir=normalize(vec3(0)-FragPos);         // �ڹ۲�ռ���㾵�淴�� �۲�������ԭ�㲻�ü���
    vec3 reflectDir=reflect(-lightDir,normalDir);      // ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
    float spec=pow(max(dot(viewDir,reflectDir),0),32);
    vec3 specular=specularStrength*spec*lightColor;

    vec3 result=(ambient+diffuse+specular)*objectColor;
    FragColor=vec4(result,1.);
}