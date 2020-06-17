#version 300 es
precision highp float;
in vec3 FragPos;

uniform vec3 lightPos;
uniform float lightFarPlane;
uniform vec3 lightDir;

void main()
{
    vec3 delta = FragPos - lightPos;

    gl_FragDepth = dot(delta, lightDir) < 0.0 ? 1.0 : length(delta)/lightFarPlane;
}