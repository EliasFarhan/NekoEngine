#version 300 es
precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

void main()
{
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    FragPos = viewPos.xyz; 
    TexCoords = aTexCoords;
    
    Normal = mat3(normalMatrix) * aNormal;
    
    gl_Position = projection * viewPos;
}