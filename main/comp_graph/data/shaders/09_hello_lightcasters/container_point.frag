#version 300 es
precision highp float;
out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    int     shininess;
};

uniform Material objectMaterial;

struct Light
{
    vec3 color;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

};
uniform Light light;
uniform vec3 viewPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;


in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

void main()
{
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

    vec3 ambient = texture(objectMaterial.diffuse, TexCoords).rgb * light.color * ambientStrength;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * texture(objectMaterial.diffuse, TexCoords).rgb * diffuseStrength * attenuation;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(diff*dot(viewDir, reflectDir), 0.0), float(objectMaterial.shininess));
    vec3 specular = light.color * spec * texture(objectMaterial.specular, TexCoords).rgb * specularStrength * attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}