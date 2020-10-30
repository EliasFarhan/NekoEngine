#version 300 es
precision mediump float;

out vec4 FragColor;
in vec2 TexCoords;
uniform vec4 spriteColor;
uniform sampler2D spriteTexture;

void main()
{
    FragColor = texture(spriteTexture, TexCoords) * spriteColor;
}