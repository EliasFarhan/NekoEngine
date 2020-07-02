#version 300 es
precision highp float;

out vec4 FragColor;
in vec2 TexCoords;
in vec4 Position;//world space

uniform int passType; //0 normal, 1 reflection, 2 refraction
uniform float waterHeight;
uniform sampler2D texture_diffuse1;

void main()
{    
	if(passType == 1 && Position.y < waterHeight)//World space
		discard;
    if(passType == 2 && Position.y > waterHeight)//World space
		discard;
    FragColor = texture(texture_diffuse1, TexCoords);
}