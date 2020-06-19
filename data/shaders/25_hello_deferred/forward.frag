#version 300 es
precision highp float;
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

struct Light {
    vec3 position;
    vec3 color;
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{

    // and the diffuse per-fragment color
    vec3 Albedo = texture(material.texture_diffuse1, TexCoords).rgb;
    float Specular = texture(material.texture_specular1, TexCoords).r;

    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // diffuse
        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0/(distance*distance);
        vec3 lightDir = normalize(lights[i].position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * attenuation * Albedo * lights[i].color;
        lighting += diffuse;
    }
    vec3 result = lighting / (lighting + vec3(1.0));
    FragColor = vec4(result, 1.0);
}