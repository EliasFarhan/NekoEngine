#version 300 es
precision highp float;

out vec4 FragColor;
in vec3 FragPos;

uniform vec3 lightPos;
uniform float lightFarPlane;
void main()
{
    vec3 delta = FragPos - lightPos;
    gl_FragDepth = length(delta)/lightFarPlane; //dot(delta, lightDir) < 0.0 ? 1.0 :
}