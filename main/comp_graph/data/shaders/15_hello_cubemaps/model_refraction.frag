#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform float refractionValue;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform float refractiveIndex;//= 1.52

void main()
{    
	float ratio = 1.00 / refractiveIndex;
    vec3 viewDir = normalize(Position - cameraPos);
    vec3 refractDir = refract(viewDir, normalize(Normal), ratio);

	vec3 refrColor = texture(skybox, refractDir).rgb;
	vec3 diffuseColor = texture(texture_diffuse1, TexCoords).rgb; 
    FragColor = vec4(mix(diffuseColor, refrColor, refractionValue),1.0);
}