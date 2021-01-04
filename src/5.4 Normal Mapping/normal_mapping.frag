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

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // 环境光
    vec3 ambient=light.ambient * vec3(texture(material.diffuse, fs_in.TexCoord));

    // 漫反射
    vec3 normalDir=normalize(texture(material.specular,fs_in.TexCoord).rgb);
    vec3 lightDir=normalize(light.position - fs_in.FragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoord));

    // 镜面反射
    vec3 viewDir=normalize(viewPos-fs_in.FragPos);         // 在观察空间计算镜面反射 观察坐标是原点不用计算
    vec3 reflectDir=reflect(-lightDir,normalDir);      // 反射函数第一个参数是入射光方向 第二个参数是法线方向
    float spec=pow(max(dot(viewDir,reflectDir),0),64);
    vec3 specular=vec3(spec * texture(material.diffuse, fs_in.TexCoord).rgb);

    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result,1.);
}