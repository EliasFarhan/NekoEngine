#version 300 es
precision highp float;

out vec4 FragColor;


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 LightSpacePos[3];
in float ClipSpacePosZ;

struct Material
{
    sampler2D texture_diffuse1;
};
uniform Material material;

struct DirectionalLight
{
    vec3 direction;
    sampler2D shadowMap;
};
uniform DirectionalLight lights[3];
uniform vec3 viewPos;

uniform bool enableCascadeColor;
uniform float bias;

uniform float maxNearCascade;
uniform float maxMiddleCascade;
uniform float farPlane;

float ShadowCalculation(int cascadeIndex)
{
    vec4 fragPosLightSpace = LightSpacePos[cascadeIndex];
     // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(lights[cascadeIndex].shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

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
    vec3 cascadeIndicator = vec3(0.0, 0.0, 0.0);
    float shadow = 0.0;
    float cascadeFarPlanes[3] = float[3](
        maxNearCascade,
        maxMiddleCascade,
        farPlane
    );
    for (int i = 0 ; i < 3 ; i++) 
    {
        if (ClipSpacePosZ <= cascadeFarPlanes[i]) 
        {
            shadow = ShadowCalculation(i);

            if (i == 0) 
                cascadeIndicator = vec3(0.1, 0.0, 0.0);
            else if (i == 1)
                cascadeIndicator = vec3(0.0, 0.1, 0.0);
            else if (i == 2)
                cascadeIndicator = vec3(0.0, 0.0, 0.1);

            break;
        }
   }      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color + (enableCascadeColor? cascadeIndicator : vec3(0.0));    
    
    FragColor = vec4(lighting, 1.0);
}