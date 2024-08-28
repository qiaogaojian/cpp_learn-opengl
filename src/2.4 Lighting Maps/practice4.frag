#version 330 core
struct Material {
    sampler2D diffuse;   // 移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，不需要将它们分开储存
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
uniform sampler2D emission;

void main()
{
    // 环境光
    vec3 ambient=light.ambient * vec3(texture(material.diffuse, TexCoord));

    // 漫反射
    vec3 normalDir=normalize(Normal);
    vec3 lightDir=normalize(light.position - FragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    // 镜面反射
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normalDir);// 反射函数第一个参数是入射光方向 第二个参数是法线方向
    float spec=pow(max(dot(viewDir,reflectDir),0),material.shininess);
    vec3 specular=light.specular * spec * vec3(texture(material.specular,TexCoord));

    // 发光
    vec3 emiss = vec3(texture(emission, TexCoord));

    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result + emiss,1.);
}