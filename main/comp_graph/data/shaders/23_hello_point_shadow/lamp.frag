#version 300 es
precision mediump float;
layout (location = 0) out vec4 FragColor;

uniform vec3 lightColor;
void main()
{
	FragColor = vec4(lightColor, 0.0);
}