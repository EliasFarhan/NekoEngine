#pragma once

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

#include "mathematics/vector.h"

#include <array>
#include <memory>
#include <vector>
#include <memory>

namespace neko::voxel
{
const size_t chunkSize = 32;


using CubeId = std::uint16_t;
enum class CubeType : std::uint8_t
{
    NONE = 0u,
    GRASS,
    DESERT,
    SNOW,
    ROCK,
    BRICK,
    WOOD,
    DIRT,
    LENGTH,
};
using CubeTextureId = std::uint32_t;

CubeTextureId GenerateTextureId(CubeType cubeType);
struct Cube
{
    enum CubeFlag : std::uint8_t
    {
        NONE = 0u,
        IS_VISIBLE = 1u,

    };

    /**
     * \brief Used to identify a cube position inside a chunk
     */
    CubeId cubeId = 0;
    std::uint8_t flag = NONE;
    CubeTextureId cubeTextureId = 0u;
    [[nodiscard]] static Vec3b CalculateCubePosition(CubeId cubeId);
    [[nodiscard]] static CubeId CalculateCubeId(Vec3b cubePosition);
};


using ChunkContent = std::array<std::array<std::array<Cube, chunkSize>, chunkSize>, chunkSize>;
using ChunkVisibleArray = std::vector<CubeId>;
using ChunkId = std::uint16_t;
struct Chunk
{
    enum ChunkFlag : std::uint8_t
    {
        NONE = 0u,
        IS_VISIBLE = 1u,
    };
    std::unique_ptr<ChunkContent> contents = nullptr;
    ChunkVisibleArray visibleCubes;
    ChunkId chunkId = 0;
    std::uint8_t flag = NONE;
};


}
