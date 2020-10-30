#version 300 es
precision mediump float;


layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

out vec3 TexCoords;
out vec3 Position;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; //z == 1
}