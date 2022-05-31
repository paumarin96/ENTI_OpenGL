#version 330

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_UV;

out vec3 vert_Normal;
out vec2 fragmentUV;

void main() {

     
    gl_Position = vec4(in_Position, 1.0);
    vert_Normal = in_Normal;

    fragmentUV = in_UV;
}