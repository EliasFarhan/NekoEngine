#version 300 es
precision highp float;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 transform;

out vec2 TexCoords;

void main()
{
    gl_Position = transform * vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}