#version 300 es
precision highp float;
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
out vec4 FragPos;

void main()
{
    FragPos = model * vec4(aPos, 1.0);
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}