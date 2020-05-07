#version 300 es
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = gl_Position;
	Normal = (projection * view * model * vec4(aNormal, 1.0)).xyz;
}