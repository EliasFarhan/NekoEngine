#version 300 es
precision highp float;
layout(location = 0) out vec4 FragColor;

in vec2 TexCoords;
in vec4 FragPos;
in vec3 FragWorldPos;

uniform sampler2D reflectionMap;//clipspace
uniform sampler2D refractionMap;//clipspace
uniform sampler2D dudvMap;//local space
uniform sampler2D depthMap;//clipspace
uniform sampler2D normalMap;//local space

uniform float waveStrength;// = 0.02;
uniform vec3 viewPos;
uniform float timeSinceStart;
uniform float waveSpeed;// = 0.01;

float linearize_depth(float d,float zNear,float zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}
//Using Schlick approximation
float Fresnel(float cosAngle, float n1, float n2)
{
    float R0 = ((n1-n2)/(n1+n2));
    R0 = R0 * R0;
    return R0+(1.0-R0)*(1.0-pow(cosAngle, 5.0));
}

void main()
{ 
	
	vec2 dudv = texture(dudvMap, TexCoords+(timeSinceStart*waveSpeed)).rg * 2.0 - 1.0;
	vec3 normal = texture(normalMap, TexCoords+(timeSinceStart*waveSpeed)).rgb;
	normal = normalize(vec3(normal.r, normal.b, normal.g))*2.0-1.0;
	float refractiveFactor = Fresnel(max(dot(normalize(viewPos-FragWorldPos), normal),0.0), 1.333, 1.0);
	vec2 ndc = (FragPos.xy/FragPos.w)/2.0 + 0.5;//clipspace
	float depth = linearize_depth(texture(depthMap, ndc).r, 0.1, 100.0);
	float depthAttenuation = clamp(abs(depth-FragWorldPos.z), 0.0,1.0);
	ndc += depthAttenuation * dudv * waveStrength;
	ndc = clamp(ndc, 0.001, 0.999);
	vec4 reflectionColor = texture(reflectionMap, vec2(1.0-ndc.x, ndc.y));
	vec4 refractionColor = texture(refractionMap, ndc);
    FragColor = mix(reflectionColor, refractionColor, refractiveFactor);
}