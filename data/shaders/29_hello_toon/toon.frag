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
};

uniform Light light;
uniform Material material;

uniform int toonLayers;// = 4;
uniform vec3 viewPos;


void main()
{
    vec3 normal = Normal;
   
    // get diffuse color
    vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;
    // ambient
    vec3 ambient = material.ambient * color;
	vec3 lightColor = vec3(0.0,0.0,0.0);
	

    vec3 currentLightColor;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = light.position - FragPos;
    float attenuation = 1.0 / length(lightDir);
    lightDir = normalize(lightDir);

    vec3 fragmentColor = vec3(material.ambient); 
        
    for(int i = toonLayers-1; i >= 0; i--)
    {
        if (attenuation * max(0.0, dot(normal, lightDir)) >= 
            0.5 + float(i) / float(toonLayers)/2.0)
        {
            fragmentColor = vec3(material.ambient + (1.0 - material.ambient) * float(i) / float(toonLayers));
            break;
        }
    }
	
    FragColor = vec4(ambient + fragmentColor, 1.0);
}