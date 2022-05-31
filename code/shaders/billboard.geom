#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in vec3 vert_Normal[];
in vec2 fragmentUV[];

out vec3 frag_Normal;
out vec3 frag_Pos;
out vec2 frag_UV;
out vec3 frag_viewPos;

uniform mat4 objMat;
uniform mat4 mv_Mat;
uniform mat4 mvpMat;
uniform vec3 viewPos;

void main() {

    vec3 billboardNormal = vec3(gl_in[0].gl_Position) - viewPos;
    billboardNormal = normalize(billboardNormal);

    vec3 rightCrossed = cross(billboardNormal, vec3(0,1,0));
    rightCrossed = normalize(rightCrossed);
    vec3 upCrossed = cross(rightCrossed, billboardNormal);
    upCrossed = normalize(upCrossed);


    

    gl_Position = mvpMat * objMat * (gl_in[0].gl_Position - vec4(rightCrossed,0) * 0.5);
    frag_Normal = vert_Normal[0];
    frag_Pos =  vec3(objMat *  gl_in[0].gl_Position);
    frag_UV = vec2(0,0);
    EmitVertex();
    gl_Position = mvpMat * objMat *  (gl_in[0].gl_Position + vec4(rightCrossed,0) * 0.5);
    frag_Normal = vert_Normal[0];
    frag_Pos =  vec3(objMat *  gl_in[0].gl_Position);
    frag_UV = vec2(1,0);
    EmitVertex();
    gl_Position = mvpMat * objMat *  (gl_in[0].gl_Position - vec4(rightCrossed,0) * 0.5 + vec4(upCrossed,0));
    frag_Normal = vert_Normal[0];
    frag_Pos =  vec3(objMat *  gl_in[0].gl_Position);
    frag_UV = vec2(0,1);
    EmitVertex();
    gl_Position = mvpMat * objMat *  (gl_in[0].gl_Position + vec4(upCrossed +rightCrossed*0.5,0));
    frag_Normal = vert_Normal[0];
    frag_Pos =  vec3(objMat *  gl_in[0].gl_Position);
    frag_UV = vec2(1,1);
    EmitVertex();

    EndPrimitive();
    frag_viewPos = viewPos;

}