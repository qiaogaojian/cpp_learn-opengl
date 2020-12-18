#version 330 core
struct Material {
    sampler2D diffuse;   // �Ƴ��˻����������ɫ��������Ϊ��������ɫ�ڼ�����������¶�������������ɫ������Ҫ�����Ƿֿ�����
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ������
    vec3 ambient=light.ambient * vec3(texture(material.diffuse, TexCoord));

    // ������
    vec3 normalDir=normalize(Normal);
    vec3 lightDir=normalize(light.position - FragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    // ���淴��
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normalDir);// ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
    float spec=pow(max(dot(viewDir,reflectDir),0),32);
    vec3 specular=light.specular * spec *(vec3(1.0) - vec3(texture(material.specular,TexCoord)));

    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result,1.);
}