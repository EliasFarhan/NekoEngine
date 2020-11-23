#version 300 es
precision highp float;

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

struct Light
{
    vec3 position;
    vec3 color;
};

struct Material
{
    float ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

uniform Light light;
uniform Material material;

uniform int toonLayers;// = 4;
uniform int specularLayers;
uniform vec3 viewPos;

void main()
{
    vec3 normal = normalize(Normal);
   
    // get diffuse color
    vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;
    // ambient
	vec3 lightColor = vec3(0.0,0.0,0.0);
	

    vec3 currentLightColor;
    vec3 lightDir = light.position - FragPos;
    vec3 viewDir = normalize(viewPos - FragPos);
    float attenuation = 1.0 / length(lightDir);
    lightDir = normalize(lightDir);

    float NdotL = dot(normal, lightDir);

    vec3 fragmentColor = vec3(material.ambient); 
        
    if(NdotL > 0.0)
    {
        float toonColorFactor = ceil(NdotL*float(toonLayers));        
        fragmentColor = vec3(material.ambient + color * (1.0 - material.ambient) * toonColorFactor / float(toonLayers));
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(reflectDir, viewDir), 0.0),  material.shininess);
        if(spec > 0.5)
        {
            fragmentColor = vec3(1.0);
        }
    }
	
    FragColor = vec4(fragmentColor, 1.0);
}