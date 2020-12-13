/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

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
