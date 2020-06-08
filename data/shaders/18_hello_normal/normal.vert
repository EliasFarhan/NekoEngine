#version 300 es
precision highp float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform mat4 transposeInverseModel;
uniform bool enableNormalMap;


void main() 
{
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    mat3 normalMatrix = mat3(transposeInverseModel);
    if(enableNormalMap)
    {
        vec3 T = normalize(normalMatrix * aTangent);
        vec3 N = normalize(normalMatrix * aNormal);
        T = normalize(T - dot(T, N) * N);
        vec3 B = normalize(cross(N, T));

        mat3 TBN = transpose(mat3(T, B, N));

        TangentLightPos = TBN * lightPos;
        TangentViewPos  = TBN * viewPos;
        TangentFragPos  = TBN * FragPos;
    }
    else
    {
        Normal = normalMatrix * aNormal;
    }
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
}
