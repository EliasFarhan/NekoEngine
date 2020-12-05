#version 300 es
precision highp float;

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D tilesheet;

void main()
{
    FragColor = vec4(texture(tilesheet, TexCoord).rgb, 1.0);
}
