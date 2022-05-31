#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in vec3 vert_Normal[];
in vec2 fragmentUV[];

out vec3 frag_Normal;
out vec3 frag_Pos;
out vec2 frag_UV;

uniform mat4 objMat;
uniform mat4 mv_Mat;
uniform mat4 mvpMat;

void main() {
    gl_Position = mvpMat * objMat * gl_in[0].gl_Position + vec4(0,0,0,1);
    frag_Normal = vert_Normal[0];
    frag_Pos =  vec3(objMat *  gl_in[0].gl_Position);
    frag_UV = gl_in[0].gl_Position + vec4(0,0,0,1);
    EmitVertex();
    gl_Position = mvpMat * objMat *  gl_in[1].gl_Position + vec4(0,0,0,1);
    frag_Normal = vert_Normal[1];
    frag_Pos =  vec3(objMat *  gl_in[1].gl_Position);
    frag_UV = gl_in[1].gl_Position + vec4(0,0,0,1)
    EmitVertex();
    gl_Position = mvpMat * objMat *  gl_in[2].gl_Position + vec4(0,0,0,1);
    frag_Normal = vert_Normal[2];
    frag_Pos =  vec3(objMat *  gl_in[2].gl_Position);
    frag_UV = gl_in[1].gl_Position + vec4(0,0,0,1)
    EmitVertex();
    EndPrimitive();

}