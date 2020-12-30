#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse;
uniform bool blinn;

void main()
{
    // 环境光
    float ambientStrength=.1;
    vec3 ambient=ambientStrength*lightColor*texture(texture_diffuse,TexCoord).rgb;

    // 漫反射
    vec3 normalDir=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diff=max(dot(normalDir,lightDir),0);
    vec3 diffuse=diff*lightColor*texture(texture_diffuse,TexCoord).rgb;

    // 镜面反射
    float specularStrength=.5;
    vec3 viewDir    = normalize(viewPos - FragPos);
    float spec = 0;
    if(blinn){
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec=pow(max(dot(normalDir,halfwayDir),0),8);
    }else{
        vec3 reflectDir=reflect(-lightDir,normalDir);// 反射函数第一个参数是入射光方向 第二个参数是法线方向
        spec=pow(max(dot(viewDir,reflectDir),0),8);
    }
    vec3 specular=specularStrength*spec*lightColor*texture(texture_diffuse,TexCoord).rgb;

    vec3 result=(ambient+diffuse+specular);
    FragColor=vec4(result,1.);
}