#version 300 es
precision mediump float;

layout(location = 0) out vec4 FragColor;
uniform vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor , 1.0);
} 