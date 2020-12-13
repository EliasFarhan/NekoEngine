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

#include <PerlinNoise.hpp>

#include "chunk.h"
#include "region.h"
#include "mathematics/vector.h"

namespace neko::voxel
{

class ChunkGenerator
{
public:
    [[nodiscard]] Chunk GenerateChunk(RegionId regionId, ChunkId chunkId) const;
    [[nodiscard]] Region GenerateRegion(RegionId regionId) const;
    [[nodiscard]] size_t GetHeight(RegionId regionId, Vec2u cubePos) const;
private:
    siv::PerlinNoise perlinNoise_{};
    static constexpr std::int32_t octave = 5;
    static constexpr double scale = 10.0;
    static constexpr double elevetionPow = 5.0;
    static constexpr double maxHeight = regionHeight * chunkSize;
    static constexpr int regionWidth = regionSize * chunkSize;
    static constexpr auto regionWidthFloat = static_cast<double>(regionWidth);
};

}
