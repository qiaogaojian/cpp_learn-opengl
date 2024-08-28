#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec3 lightPosition;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    // 环境光
    float ambientStrength=.1;
    vec3 ambient=ambientStrength*lightColor;

    // 漫反射
    vec3 normalDir=normalize(Normal);
    vec3 lightDir=normalize(lightPosition-FragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=diff*lightColor;

    // 镜面反射
    float specularStrength=.5;
    vec3 viewDir=normalize(vec3(0)-FragPos);         // 在观察空间计算镜面反射 观察坐标是原点不用计算
    vec3 reflectDir=reflect(-lightDir,normalDir);      // 反射函数第一个参数是入射光方向 第二个参数是法线方向
    float spec=pow(max(dot(viewDir,reflectDir),0),32);
    vec3 specular=specularStrength*spec*lightColor;

    vec3 result=(ambient+diffuse+specular)*objectColor;
    FragColor=vec4(result,1.);
}