#version 300 es
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

mat4 translate(vec3 v)
{
    return mat4(
        vec4(1.0,0.0,0.0,0.0),
        vec4(0.0,1.0,0.0,0.0),
        vec4(0.0,0.0,1.0,0.0),
        vec4(v, 1.0)
    );
}

uniform vec3 position;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
    vec4 pos = projection * view * translate(position) * vec4(aPos, 1.0);
    gl_Position = pos;
    TexCoords = aTexCoords;
}