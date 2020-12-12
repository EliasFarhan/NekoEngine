#version 300 es
precision highp float;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in uint faceType;
layout(location = 3) in uint regionId;
layout(location = 4) in uint chunkId;
layout(location = 5) in uint cubeId;
layout(location = 6) in uint textureId;

const uint regionSize = 64u;
const uint regionHeight = 16u;
const uint chunkSize = 32u;

const uint TopFace = 1u;
const uint BottomFace = 2u;
const uint SideFace = 3u;
const uint tileNmbX = 14u;
const uint tileNmbY = 7u;
const float tileSizeX = 1.0 / float(tileNmbX);
const float tileSizeY = 1.0 / float(tileNmbY);

out vec2 TexCoord;

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
    // TODO add region position calculation
    // chunk position calculation
    uint chunkY = chunkId % regionHeight;
    uint chunkX = chunkId / regionHeight / regionSize;
    uint chunkZ = (chunkId - chunkX * regionHeight * regionSize) / regionHeight;
    float chunkLength = float(chunkSize);
    // cube position calculation
    uint cubeY = cubeId & 31u;
    uint cubeX = (cubeId >> 10u) & 31u;
    uint cubeZ = (cubeId >> 5u) & 31u;
    vec3 worldPos = vec3( 
        float(cubeX) + float(chunkX) * chunkLength - float(regionSize) / 2.0 * chunkLength,
        float(cubeY) + float(chunkY) * chunkLength - float(regionHeight) / 2.0 * chunkLength,
        float(cubeZ) + float(chunkZ) * chunkLength - float(regionSize) / 2.0 * chunkLength);

    gl_Position = proj * view * translate(worldPos.x, worldPos.y, worldPos.z)
        * vec4(aPos, 1.0);
    uint textureIndex =
        faceType == TopFace ? textureId >> 16u :
        (faceType == BottomFace ? (textureId >> 8u) & 255u :
        textureId & 255u);

    uint tileY = textureIndex / tileNmbX;
    uint tileX = textureIndex - tileY * tileNmbX;
    vec2 texCoord = vec2(
            float(tileX) * tileSizeX,
            1.0 - float(tileY) * tileSizeY - tileSizeY)
        + aTexCoords * vec2(tileSizeX, tileSizeY);
    TexCoord = texCoord;
}