#version 300 es
precision mediump float;


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transposeInverseModel;

void main()
{
    TexCoords = aTexCoords;    
	Normal = mat3(transposeInverseModel) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}