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

void main()
{    
	vec3 diffuseColor = texture(texture_diffuse1, TexCoords).rgb; 
    FragColor = vec4(diffuseColor,1.0);
}