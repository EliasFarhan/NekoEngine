#version 300 es
precision highp float;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform vec2 offset;
uniform vec2 scale;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(offset.x+aPos.x*scale.x, offset.y+aPos.y*scale.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}