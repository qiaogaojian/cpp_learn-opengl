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

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main()
{
    // ������
    vec3 ambient=light.ambient * vec3(texture(material.diffuse, fs_in.TexCoord));

    // ������ // �� [-1,1] ת���� [0,1] ���������ķ�Χ��-1��1֮�䣬����������Ҫ����ӳ�䵽0��1�ķ�Χ
    vec3 normalDir=normalize(texture(material.specular,fs_in.TexCoord).rgb * 2.0 - 1.0);

    vec3 lightDir=normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoord));

    // ���淴��
    vec3 viewDir=normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);    // �ڹ۲�ռ���㾵�淴�� �۲�������ԭ�㲻�ü���
    vec3 reflectDir=reflect(-lightDir,normalDir);                           // ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
    float spec=pow(max(dot(viewDir,reflectDir),0),64);
    vec3 specular=vec3(spec * texture(material.diffuse, fs_in.TexCoord).rgb);

    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result,1.);
}