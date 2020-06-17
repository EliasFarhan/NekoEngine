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
uniform samplerCube shadowMap;

struct PointLight
{
    vec3 lightPos;
};
uniform PointLight light;
uniform vec3 viewPos;
uniform float lightFarPlane;
uniform bool enablePcf;
uniform float bias;

float ShadowCalculation()
{
    vec3 fragToLight = FragPos - light.lightPos;
    float currentDepth = length(fragToLight);
    float closestDepth = texture(shadowMap, fragToLight).r;
    closestDepth *= lightFarPlane;
    // get depth of current fragment from light's perspective

    float shadow  = 0.0;
    const float samples = 4.0;
    const float offset  = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(shadowMap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= lightFarPlane;   // undo mapping [0;1]
                if(currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);
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
    vec3 lightDir = normalize(light.lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = 0.2 * spec * lightColor;
    // calculate shadow
    float shadow = ShadowCalculation();
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}