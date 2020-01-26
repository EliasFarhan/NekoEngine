#version 300 es
layout(location = 0) out vec4 FragColor;
in vec3 ourColor;

uniform float colorCoeff;

void main()
{
    FragColor = vec4(ourColor * colorCoeff, 1.0f) ;
} 