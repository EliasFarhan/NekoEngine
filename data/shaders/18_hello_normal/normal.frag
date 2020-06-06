#version 300 es
precision highp float;


in vec3 FragPos;
in mat3 TBN;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    // get diffuse color
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    FragColor = vec4((diff+ambient) * color, 1.0);
}
