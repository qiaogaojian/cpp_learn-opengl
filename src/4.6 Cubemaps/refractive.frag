#version 330 core
struct Material {
    sampler2D diffuse;   // �Ƴ��˻����������ɫ��������Ϊ��������ɫ�ڼ�����������¶�������������ɫ������Ҫ�����Ƿֿ�����
    sampler2D specular;
    float shininess;
};

struct Light {
    // vec3 position; // ƽ�йⲻ��Ҫ����λ��
    vec4 vector;

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

uniform samplerCube skybox;

void main()
{
    float ratio = 1.0 / 1.2;
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = refract(I, normalize(Normal), ratio);

    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}