#version 330

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_UV;
layout (location = 3) in vec2 in_mapUV;

out vec3 frag_Normal;
out vec2 frag_UV;
out vec2 fragmentMapUV;
out vec3 fragPos;

uniform mat4 objMat;
uniform mat4 mv_Mat;
uniform mat4 mvpMat;

void main() {

     
    gl_Position = mvpMat * objMat *  vec4(in_Position, 1.0);
    frag_Normal = -in_Normal;

    fragPos =  vec3(objMat * vec4(in_Position, 1.0));
    frag_UV = in_UV;
    fragmentMapUV = in_mapUV;
}