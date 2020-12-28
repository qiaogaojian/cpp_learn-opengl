#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT{
    vec2 TexCoord;
    vec3 color;
} colors[];

out vec3 fColor;

void build_house(vec4 position)
{
    fColor = colors[0].color; // gs_in[0] ��Ϊֻ��һ�����붥��
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:����
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:����
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:����
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:����
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:����
    EmitVertex();
    EndPrimitive();
}

void main() {
    build_house(gl_in[0].gl_Position);
}