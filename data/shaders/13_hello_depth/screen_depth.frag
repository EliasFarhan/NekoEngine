#version 300 es
precision highp float;

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthTexture;
uniform float near; 
uniform float far; 
uniform bool linearDepth;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
void main()
{
    float depth = texture(depthTexture, TexCoords).r;
    if(linearDepth)
    {
        depth = LinearizeDepth(depth)/far;
    }
    FragColor = vec4(depth, depth, depth,1.0);
}