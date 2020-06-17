#version 300 es
precision highp float;
in vec4 FragPos;

uniform vec3 lightPos;
uniform float lightFarPlane;
uniform vec3 lightDir;

void main()
{
    vec3 delta = FragPos.xyz - lightPos;

    gl_FragDepth = length(delta)/lightFarPlane;


}