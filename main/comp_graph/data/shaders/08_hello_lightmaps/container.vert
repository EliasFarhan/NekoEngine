#version 300 es
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseTransposeModel;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

void main()
{
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(inverseTransposeModel)*aNormal;
    TexCoords = aTexCoords;
}