#version 300 es
precision highp float;

layout(location = 0) out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    float v = texture(ourTexture, TexCoord).r;
    FragColor = vec4(v,v,v,1.0);
}