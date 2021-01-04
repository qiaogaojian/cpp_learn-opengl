#version 330 core
struct Material {
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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_reflection1;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(FragPos - viewPos);
    vec3 normal = texture(texture_normal1, TexCoord).rgb;
    vec3 R = reflect(I, normal);
    vec3 reflectMap = texture(texture_reflection1, TexCoord).rgb;
    vec3 reflection = vec3(texture(skybox, R).rgb) * reflectMap;

    // // ������
    vec3 ambient=light.ambient * vec3(texture(texture_diffuse1, TexCoord));

    // ������
    vec3 normalDir=normalize(Normal);
    vec3 lightDir;
    float attenuation  = 1.0;
    if (light.vector.w == 0){ // ƽ�й�
        lightDir=normalize(-vec3(light.vector));
    } else {                     // ���
        lightDir=normalize(vec3(light.vector) - FragPos);
        float len = length(vec3(light.vector) - FragPos);
        // attenuation  = 1 / (light.constant + light.linear * len + light.quadratic * len * len ); // ˥��
    }

    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));

    // ���淴��
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normalDir);// ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
    float spec=pow(max(dot(viewDir,reflectDir),0),material.shininess);
    vec3 specular=light.specular * spec * vec3(texture(texture_specular1,TexCoord));

    // ���ǿ��Խ�������������ֲ��䣬�û������ղ������ž�����٣������������ʹ�ö���һ
    // ���Ĺ�Դ�����еĻ�����������Ὺʼ���ӣ��������������������Ҳϣ��˥���������ա�
    // vec3 result=(ambient+diffuse+specular+reflection)* attenuation;
    FragColor=vec4(ambient+diffuse+specular+reflection, 1.);
}