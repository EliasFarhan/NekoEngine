#version 300 es
precision highp float;
out vec4 FragColor;
struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_normal1;
};
uniform Material material;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
	vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;

	vec3 ambient = 0.1 * color;
	
	vec3 normal = normalize(Normal);
	
	//lightDir is the direction to the light from the fragment
	vec3 lightDir = normalize(lightPos - FragPos); 
		//viewDir is the direction to the camera from the fragment
	vec3 viewDir = normalize(viewPos - FragPos);
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * color ;
	// specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.2) * spec;
	FragColor = vec4(ambient + diffuse + specular, 1.0);
}