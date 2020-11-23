#version 300 es
precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;


void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    
    Normal = mat3(normalMatrix) * aNormal;   

    vec3 T = normalize(mat3(normalMatrix) * aTangent);
    vec3 N = normalize(mat3(normalMatrix) * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(cross(N, T));

    TBN = mat3(T, B, N);
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}