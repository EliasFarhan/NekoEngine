#version 300 es
precision highp float;


in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float resolution;
    float shininess;
};
uniform Material material;

uniform vec3 viewPos;
struct Light
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

};
uniform Light light;

uniform bool onlySpecular;

void main() 
{
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));
    vec3 normal = normalize(Normal);
    
    // get diffuse color
    vec3 color = texture(material.texture_diffuse1, TexCoords*material.resolution).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir;
    vec3 viewDir;

    lightDir = normalize(light.position - FragPos);
    viewDir = normalize(viewPos - FragPos);
    
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color * attenuation * (onlySpecular?0.0:1.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0),  material.shininess);

    vec3 specular = vec3(onlySpecular?1.0:0.2) * spec * attenuation;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    
}
