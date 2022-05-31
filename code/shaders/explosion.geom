#version 330

layout (triangles) in;
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

uniform float time;

float hash11(float p)
{
    p = fract(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}


void main() {

    vec3 faceNormal = cross(vec3(gl_in[0].gl_Position - gl_in[1].gl_Position), vec3(gl_in[1].gl_Position -  (mix(gl_in[1].gl_Position,gl_in[2].gl_Position,0.5))));
    faceNormal = normalize(faceNormal);

    float y;
    vec3 direction = faceNormal * (1 + sin(time));

    y = (1*(1+sin(time))) + (0.5*-9.8*pow((1+sin(time)),2));


    gl_Position = mvpMat * objMat * (gl_in[0].gl_Position + vec4(direction.x, direction.y + y , direction.z,0));
    frag_Normal = vert_Normal[0];
    frag_Pos =  vec3(objMat *  (gl_in[0].gl_Position + vec4(direction.x, direction.y  , direction.z,0)));
    frag_UV = fragmentUV[0];
    EmitVertex();

    gl_Position = mvpMat * objMat *  (gl_in[1].gl_Position + vec4(direction.x, direction.y + y, direction.z,0));
    frag_Normal = vert_Normal[1];
    frag_Pos =  vec3(objMat *  (gl_in[1].gl_Position + vec4(direction.x, direction.y , direction.z,0)));
    frag_UV = fragmentUV[1];
    EmitVertex();


    gl_Position = mvpMat * objMat *  (mix(gl_in[1].gl_Position,gl_in[2].gl_Position,0.5) + vec4(direction.x, direction.y + y, direction.z,0));
    frag_Normal = vert_Normal[2];
    frag_Pos =  vec3(objMat *  (gl_in[2].gl_Position + vec4(direction.x, direction.y, direction.z,0)));
    frag_UV = fragmentUV[2];
    EmitVertex();

    EndPrimitive();
       
    faceNormal = cross(vec3(gl_in[0].gl_Position -  (mix(gl_in[1].gl_Position,gl_in[2].gl_Position,0.5))), vec3(gl_in[0].gl_Position - gl_in[2].gl_Position));
    faceNormal = normalize(faceNormal);
    
    direction = faceNormal * (1 + sin(time));

    y = (-1*(1+sin(time))) + (0.5*-9.8*pow((1+sin(time)),2));

    gl_Position = mvpMat * objMat *  (gl_in[0].gl_Position + vec4(direction.x, direction.y + y, direction.z,0));
    frag_Normal = vert_Normal[0];
    frag_Pos =  vec3(objMat *  (gl_in[0].gl_Position + vec4(direction.x, direction.y, direction.z,0)));
    frag_UV = fragmentUV[2];
    EmitVertex();

    gl_Position = mvpMat * objMat *  (mix(gl_in[1].gl_Position,gl_in[2].gl_Position,0.5) + vec4(direction.x, direction.y + y, direction.z,0));
    frag_Normal = vert_Normal[1];
    frag_Pos =  vec3(objMat *  (gl_in[2].gl_Position + vec4(direction.x, direction.y, direction.z,0)));
    frag_UV = fragmentUV[2];
    EmitVertex();

    gl_Position = mvpMat * objMat *  (gl_in[2].gl_Position + vec4(direction.x, direction.y + y, direction.z,0));
    frag_Normal = vert_Normal[2];
    frag_Pos =  vec3(objMat *  (gl_in[2].gl_Position + vec4(direction.x, direction.y, direction.z,0)));
    frag_UV = fragmentUV[2];
    EmitVertex();

    EndPrimitive();
    frag_viewPos = viewPos;

}

