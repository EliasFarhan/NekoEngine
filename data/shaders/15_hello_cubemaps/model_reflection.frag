#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform float reflectionValue;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
	vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

	vec3 reflColor = texture(skybox, R).rgb;
	vec3 diffuseColor = texture(texture_diffuse1, TexCoords).rgb; 
    FragColor = vec4(mix(diffuseColor, reflColor, reflectionValue),1.0);
}