#version 300 es
precision highp float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 FragPos;
out vec3 FragWorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{ 
    TexCoords = aTexCoords;
	FragWorldPos = vec3(model * vec4(aPos, 1.0));
	FragPos = projection * view * model * vec4(aPos, 1.0);
	gl_Position = FragPos;
}  