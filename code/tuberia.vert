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
uniform float time;


void main() {
    
   // vec3 finalPos = in_Position + vec3(sin(time + in_Position.y),0,0);
       vert_Normal = normalize(vec3(in_Position.x, 0,  in_Position.z));
    vec3 finalPos = in_Position + vec3(clamp(sin(time + in_Position.y * 2),0,1) * vert_Normal.x,0,clamp(sin(time + in_Position.y * 2) ,0,1) * vert_Normal.z);
    gl_Position = mvpMat * objMat * vec4(finalPos, 1.0) ;

    fragPos =  vec3(objMat * vec4(in_Position, 1.0));
    fragmentUV = in_UV;
}