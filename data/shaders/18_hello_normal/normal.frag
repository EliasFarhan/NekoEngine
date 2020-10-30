#version 300 es
precision highp float;


in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_normal1;
};
uniform Material material;


in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 Normal;


uniform vec3 viewPos;
uniform vec3 lightPos;
uniform bool enableNormalMap;

void main() 
{

    vec3 normal;
    if(enableNormalMap)
    {
        normal = texture(material.texture_normal1, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }
    else
    {
        normal = normalize(Normal);
    }
    // get diffuse color
    vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir;
    vec3 viewDir;
    if(enableNormalMap)
    {
        lightDir = normalize(TangentLightPos - TangentFragPos);
        viewDir = normalize(TangentViewPos - TangentFragPos);
    }
    else
    {
        lightDir = normalize(lightPos - FragPos);
        viewDir = normalize(viewPos - FragPos);
    }
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    
}
