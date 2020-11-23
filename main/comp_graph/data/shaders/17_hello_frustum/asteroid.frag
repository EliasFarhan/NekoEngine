#version 300 es
precision mediump float;

layout(location = 0) out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}