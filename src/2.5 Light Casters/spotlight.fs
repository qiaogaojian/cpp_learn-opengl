#version 330 core
struct Material {
    sampler2D diffuse;   // �Ƴ��˻����������ɫ��������Ϊ��������ɫ�ڼ�����������¶�������������ɫ������Ҫ�����Ƿֿ�����
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3  position;  // �۹�λ��
    vec3  direction; // �۹ⷽ��
    float cutOff;    // �й��

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // ˥��ϵ��
    float constant;  // ������
    float linear;    // һ����
    float quadratic; // ������
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
    vec3 lightDir = normalize(vec3(light.position) - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    float len = length(vec3(light.position) - FragPos);
    float attenuation  = 1 / (light.constant + light.linear * len + light.quadratic * len * len ); // ˥��
    if (theta > light.cutOff) { // �۹��
         // ������
         vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
         // �۹��
         vec3 spot = vec3(texture(material.diffuse, TexCoord)) * attenuation;
         // ���淴��
         vec3 viewDir=normalize(viewPos-FragPos);
         vec3 reflectDir=reflect(-lightDir,normalize(Normal));// ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
         float spec=pow(max(dot(viewDir,reflectDir),0),shininess);
         vec3 specular=light.specular * spec * vec3(texture(material.specular,TexCoord)) * attenuation;
         FragColor = vec4(ambient + spot + specular, 1.);
    } else {                   // ֻ�л�����
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
        FragColor = vec4(ambient ,1.);
    }
}