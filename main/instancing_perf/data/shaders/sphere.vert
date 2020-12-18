#version 300 es
precision highp float;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 6) in vec3 worldPos;


out vec2 TexCoords;

uniform mat4 view;
uniform mat4 proj;

mat4 translate(float x, float y, float z) {
    return mat4(
    vec4(1.0, 0.0, 0.0, 0.0),
    vec4(0.0, 1.0, 0.0, 0.0),
    vec4(0.0, 0.0, 1.0, 0.0),
    vec4(x, y, z, 1.0)
    );
}

void main()
{
    gl_Position = proj * view * translate(worldPos.x, worldPos.y, worldPos.z)
        * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}