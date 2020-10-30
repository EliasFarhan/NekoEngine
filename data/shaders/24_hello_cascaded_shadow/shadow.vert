#version 300 es
precision highp float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec4 LightSpacePos[3];
out float ClipSpacePosZ;
out vec3 Normal;
out vec2 TexCoords;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 transposeInverseModel;
uniform mat4 lightSpaceMatrices[3];

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transposeInverseModel) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
    ClipSpacePosZ = gl_Position.z;
    for(int i = 0; i < 3; i++)
    {
        LightSpacePos[i] = lightSpaceMatrices[i] * vec4(FragPos, 1.0);
    }
}