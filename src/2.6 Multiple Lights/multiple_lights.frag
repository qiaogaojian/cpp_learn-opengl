#version 330 core

struct Material {
    sampler2D diffuse;   // 移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，不需要将它们分开储存
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, normal, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i],normal,FragPos,viewDir);
    }

    result += CalcSpotLight(spotLight, normal,FragPos,viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    float diff = max(dot(normal,lightDir),0);
    vec3 diffuse  = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff *texture(material.diffuse, TexCoord).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

    float distance = length(light.position - fragPos);
    float attenuation = 1/(light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff *texture(material.diffuse, TexCoord).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir),0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1/(light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient + (diffuse + specular) * intensity) * attenuation;
}