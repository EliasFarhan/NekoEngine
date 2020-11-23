#version 300 es
precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 transposeInverseModel;

uniform bool inverseNormals;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    vec3 n = inverseNormals ? -aNormal : aNormal;

    mat3 normalMatrix = mat3(transposeInverseModel);
    Normal = normalize(normalMatrix * n);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}