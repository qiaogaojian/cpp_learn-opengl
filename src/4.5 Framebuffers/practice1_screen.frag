#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0/300.0;

void main()
{
      vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        // 锐化卷积矩阵 周围的像素和中心越相似 中心值越小 反差越大 中心值越大
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1

        // // 模糊卷积矩阵1 平均中心和周围像素
        // 1/9.0, 1/9.0, 1/9.0,
        // 1/9.0, 1/9.0, 1/9.0,
        // 1/9.0, 1/9.0, 1/9.0

        // // 模糊卷积矩阵2 平均中心和周围像素
        // 1.0 / 16, 2.0 / 16, 1.0 / 16,
        // 2.0 / 16, 4.0 / 16, 2.0 / 16,
        // 1.0 / 16, 2.0 / 16, 1.0 / 16

        // // 边缘检测卷积矩阵 周围的像素和中心越相似 中心值越小 反差越大 中心值越大
        // 1,  1,  1,
        // 1, -8,  1,
        // 1,  1,  1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++){
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++){
        col += sampleTex[i] * kernel[i];
    }
    FragColor = vec4(col, 1.0);

    // FragColor = texture(screenTexture, TexCoords);
    // float average = (FragColor.r + FragColor.g + FragColor.b)/3; // 黑白
    // 人眼会对绿色更加敏感一些，而对蓝色不那么敏感，所以为了获取物理上更精确的效果，我们需要使用加权的(Weighted)通道
    // float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    // FragColor = vec4(vec3(average), 1.0);
    // FragColor = vec4(1 - texture(screenTexture, TexCoords).rgb, 1.0); // 反相
}