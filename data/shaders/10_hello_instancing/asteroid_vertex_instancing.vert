#version 300 es
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 5) in mat4 aModel;

uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    vec4 pos = projection * view * aModel * vec4(aPos, 1.0);
    gl_Position = pos;
    TexCoords = aTexCoords;
}