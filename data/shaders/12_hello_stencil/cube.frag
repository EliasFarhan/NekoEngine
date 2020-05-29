#version 300 es
precision mediump float;
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 overrideColor;


void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(overrideColor,1.0);
}