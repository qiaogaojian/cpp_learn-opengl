#version 330 core
struct Material {
    sampler2D diffuse;   // 移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，不需要将它们分开储存
    sampler2D specular;
    float shininess;
};

struct Light {
    // vec3 position; // 平行光不需要光照位置
    vec4 vector;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // 衰减系数
    float constant;  // 常数项
    float linear;    // 一次项
    float quadratic; // 二次项
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
    // 环境光
    vec3 ambient=light.ambient * vec3(texture(material.diffuse, TexCoord));

    // 漫反射
    vec3 normalDir=normalize(Normal);
    vec3 lightDir;
    float attenuation  = 1.0;
    if (light.vector.w == 0){ // 平行光
        lightDir=normalize(-vec3(light.vector));
    } else {                     // 点光
        lightDir=normalize(vec3(light.vector) - FragPos);
        float len = length(vec3(light.vector) - FragPos);
        // attenuation  = 1 / (light.constant + light.linear * len + light.quadratic * len * len ); // 衰减
    }

    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    // 镜面反射
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normalDir);// 反射函数第一个参数是入射光方向 第二个参数是法线方向
    float spec=pow(max(dot(viewDir,reflectDir),0),material.shininess);
    vec3 specular=light.specular * spec * vec3(texture(material.specular,TexCoord));

    // 我们可以将环境光分量保持不变，让环境光照不会随着距离减少，但是如果我们使用多于一
    // 个的光源，所有的环境光分量将会开始叠加，所以在这种情况下我们也希望衰减环境光照。
    vec3 result=ambient+diffuse+specular;
    FragColor=vec4(result * attenuation,1.);
}