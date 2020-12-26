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

uniform samplerCube skybox;

void main()
{
    float ratio = 1.0 / 1.2;
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = refract(I, normalize(Normal), ratio);

    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}