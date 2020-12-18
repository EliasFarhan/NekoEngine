#version 300 es
precision highp float;

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = vec4(texture(ourTexture, TexCoords).rgb, 1.0);
}
