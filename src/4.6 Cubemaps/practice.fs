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
    // vec3 I = normalize(FragPos - viewPos);
    // vec3 R = reflect(I, normalize(Normal));

    // FragColor = vec4(texture(skybox, R).rgb, 1.0);

    // ������
    vec3 ambient=light.ambient * vec3(texture(material.diffuse, TexCoord));

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
    vec3 diffuse=light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    // ���淴��
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normalDir);// ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
    float spec=pow(max(dot(viewDir,reflectDir),0),material.shininess);
    vec3 specular=light.specular * spec * vec3(texture(material.specular,TexCoord));

    // ���ǿ��Խ�������������ֲ��䣬�û������ղ������ž�����٣������������ʹ�ö���һ
    // ���Ĺ�Դ�����еĻ�����������Ὺʼ���ӣ��������������������Ҳϣ��˥���������ա�
    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result * attenuation,1.);
}