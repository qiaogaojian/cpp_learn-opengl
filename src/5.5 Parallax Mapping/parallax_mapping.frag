#version 330 core
struct Material {
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

uniform float heightScale;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

vec2 calTexOffCoor(vec2 texCoord, vec3 viewDir){
    float height = texture(heightMap, texCoord).r;
    // vec2 offset = height * (viewDir.xy / viewDir.z) * heightScale;
    return texCoord - viewDir.xy / viewDir.z * (height * heightScale);
}

void main()
{
    // ������
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 offsetCoord = calTexOffCoor(fs_in.TexCoord, viewDir);
    if(offsetCoord.x > 1.0 || offsetCoord.x < -1.0 || offsetCoord.y > 1.0 || offsetCoord.y < -1.0){
        discard;
    }

    vec3 ambient=light.ambient * vec3(texture(diffuseMap, offsetCoord));

    // ������ // �� [-1,1] ת���� [0,1] ���������ķ�Χ��-1��1֮�䣬����������Ҫ����ӳ�䵽0��1�ķ�Χ
    vec3 normalDir=normalize(texture(normalMap,offsetCoord).rgb * 2.0 - 1.0);

    vec3 lightDir=normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=light.diffuse * diff * vec3(texture(diffuseMap, offsetCoord));

    // ���淴��
    vec3 reflectDir=reflect(-lightDir,normalDir);                           // ���亯����һ������������ⷽ�� �ڶ��������Ƿ��߷���
    float spec=pow(max(dot(viewDir,reflectDir),0),material.shininess);
    vec3 specular=vec3(spec * texture(diffuseMap, offsetCoord).rgb);

    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result,1.);
}