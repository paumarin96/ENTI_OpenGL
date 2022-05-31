#version 330

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_UV;
out vec3 vert_Normal;
out vec3 fragPos;
out vec2 fragmentUV;
uniform mat4 objMat;
uniform mat4 mv_Mat;
uniform mat4 mvpMat;

void main() {
    gl_Position = mvpMat * objMat * vec4(in_Position, 1.0);
    vert_Normal = in_Normal;
    fragPos =  vec3(objMat * vec4(in_Position, 1.0));
    fragmentUV = in_UV;
}