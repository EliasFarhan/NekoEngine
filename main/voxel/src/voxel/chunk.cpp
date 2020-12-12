#include "voxel/chunk.h"

namespace neko::voxel
{
CubeTextureId GenerateTextureId(CubeType cubeType)
{
    switch (cubeType)
    {
    case CubeType::GRASS:
        return (12u << 16u) | (3u << 8u) | 0u;
    case CubeType::DESERT:
        return (13u << 16u) | (3u << 8u) | 1u;
    case CubeType::SNOW:
        return (26u << 16u) | (3u << 8u) | 2u;
    case CubeType::ROCK:
        return (17u << 16u) | (17u << 8u) | 17u;
    case CubeType::BRICK:
        return (59u << 16u) | (59u << 8u) | 59u;
    case CubeType::WOOD:
        return (45u << 16u) | (45u << 8u) | 45u;
    case CubeType::DIRT:
        return (3u << 16u) | (3u << 8u) | 3u;
    default:
        break;
    }
    return 0;
}

Vec3b Cube::CalculateCubePosition(CubeId cubeId)
{
    return Vec3b((cubeId >> 10u) & 31u, cubeId & 31u, (cubeId >> 5u) & 31u);
}

CubeId Cube::CalculateCubeId(Vec3b cubePosition)
{
    return (cubePosition.x << 10u) + (cubePosition.z << 5u) + cubePosition.y;
}
}
