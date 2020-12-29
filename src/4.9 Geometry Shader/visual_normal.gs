#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT{
    vec4 Normal;
} gin[];

const float MAGNITUDE = 0.03;

void GenerateLine(int index){
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = gl_in[index].gl_Position + gin[index].Normal * MAGNITUDE;
    EmitVertex();

    EndPrimitive();
}

void main(){
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}