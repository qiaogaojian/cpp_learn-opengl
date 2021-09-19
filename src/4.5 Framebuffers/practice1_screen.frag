#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0/300.0;

void main()
{
      vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ����
        vec2( offset,  offset), // ����
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ��
        vec2( offset,  0.0f),   // ��
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ����
        vec2( offset, -offset)  // ����
    );

    float kernel[9] = float[](
        // �񻯾������ ��Χ�����غ�����Խ���� ����ֵԽС ����Խ�� ����ֵԽ��
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1

        // // ģ���������1 ƽ�����ĺ���Χ����
        // 1/9.0, 1/9.0, 1/9.0,
        // 1/9.0, 1/9.0, 1/9.0,
        // 1/9.0, 1/9.0, 1/9.0

        // // ģ���������2 ƽ�����ĺ���Χ����
        // 1.0 / 16, 2.0 / 16, 1.0 / 16,
        // 2.0 / 16, 4.0 / 16, 2.0 / 16,
        // 1.0 / 16, 2.0 / 16, 1.0 / 16

        // // ��Ե��������� ��Χ�����غ�����Խ���� ����ֵԽС ����Խ�� ����ֵԽ��
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
    // float average = (FragColor.r + FragColor.g + FragColor.b)/3; // �ڰ�
    // ���ۻ����ɫ��������һЩ��������ɫ����ô���У�����Ϊ�˻�ȡ�����ϸ���ȷ��Ч����������Ҫʹ�ü�Ȩ��(Weighted)ͨ��
    // float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    // FragColor = vec4(vec3(average), 1.0);
    // FragColor = vec4(1 - texture(screenTexture, TexCoords).rgb, 1.0); // ����
}