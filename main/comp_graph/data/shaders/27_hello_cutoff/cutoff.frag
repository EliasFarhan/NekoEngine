#version 300 es
precision highp float;
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform bool enableCutoff;

void main()
{             
    vec4 texColor = texture(texture1, TexCoords);
    if(enableCutoff && texColor.a < 0.1)
        discard;
    FragColor = texColor;
}