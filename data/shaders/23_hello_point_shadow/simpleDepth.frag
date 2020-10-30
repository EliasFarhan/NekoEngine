#version 300 es
precision highp float;
in vec3 FragPos;

uniform vec3 lightPos;
uniform float lightFarPlane;

void main()
{
    vec3 delta = FragPos - lightPos;
    gl_FragDepth = length(delta)/lightFarPlane;
}