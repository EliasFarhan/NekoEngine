#version 300 es
precision highp float;

out vec4 FragColor;


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material
{
    sampler2D texture_diffuse1;
};
uniform Material material;

struct DirectionalLight
{
    vec3 position;
    vec3 direction;
    sampler2D shadowMap;
};
uniform DirectionalLight lights[3];
uniform vec3 viewPos;

uniform bool enableCascadeColor;
uniform float bias;
uniform float maxNearCascade;
uniform float maxMiddleCascade;
uniform float lightFarPlane;

float ShadowCalculation()
{
    float depth = length(FragPos-viewPos);
    int cascadeIndex = depth < maxNearCascade? 0 : 
        depth < maxMiddleCascade ? 1 : 2;

    //outside of light frustum view
    vec3 fragToLight = FragPos - lights[cascadeIndex].position;
    float currentDepth = length(fragToLight);
    float closestDepth = texture(lights[cascadeIndex].shadowMap, fragToLight.xz).r;
    closestDepth *= lightFarPlane;

    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadow;
    
}

void main()
{           
    vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * color;
    // diffuse
    vec3 lightDir = normalize(-lights[0].direction);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation();       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}