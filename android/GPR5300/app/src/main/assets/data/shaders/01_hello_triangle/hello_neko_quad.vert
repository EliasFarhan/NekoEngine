#version 300 es
layout (location = 0) in vec2 aPos;

uniform vec4 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    ourColor = aColor.rgb;
}