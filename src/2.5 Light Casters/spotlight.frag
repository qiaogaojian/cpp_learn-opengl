#version 330 core
struct Material {
    sampler2D diffuse;   // �Ƴ��˻����������ɫ��������Ϊ��������ɫ�ڼ�����������¶�������������ɫ������Ҫ�����Ƿֿ�����
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3  position;     // �۹�λ��
    vec3  direction;    // �۹ⷽ��
    float cutOff;       // �й��
    float outerCutOff;  // �й��

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
    // ������
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    // ������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

    // ���淴��
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normalize(Normal));// ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
    float spec=pow(max(dot(viewDir,reflectDir),0),material.shininess);
    vec3 specular=light.specular * spec * vec3(texture(material.specular,TexCoord));

    // �۹��
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // ˥��
    float len = length(vec3(light.position) - FragPos);
    float attenuation  = 1 / (light.constant + light.linear * len + light.quadratic * len * len ); // ˥��

    vec3 result = (ambient + (diffuse + specular) * intensity) * attenuation;

    FragColor = vec4(result, 1.);
}