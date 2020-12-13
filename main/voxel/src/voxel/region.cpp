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

#include "voxel/region.h"

#include <mutex>

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko::voxel
{
void Region::Init()
{
    chunks_.resize(regionHeight * regionSize * regionSize);
}

const std::vector<Chunk>& Region::GetChunks() const
{
    return chunks_;
}

void Region::GenerateFromHeightMap(const HeightMap& map)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Generate From Height Map");
#endif
    const size_t regionWidth = regionSize * chunkSize;
    for(size_t x = 0; x < regionWidth; x++)
    {
        for (size_t z = 0; z < regionWidth; z++)
        {
            const auto chunkX = x / chunkSize;
            const auto chunkZ = z / chunkSize;
            const auto perlinValue = map[x][z];
            const auto chunkY = perlinValue / chunkSize;

            auto& chunk = chunks_[chunkX * regionHeight * regionSize + chunkZ * regionHeight + chunkY];
            chunk.chunkId = chunkX * regionHeight * regionSize + chunkZ * regionHeight + chunkY;
            chunk.flag = Chunk::IS_VISIBLE;
            const auto cubeX = x - chunkX * chunkSize;
            const auto cubeZ = z - chunkZ * chunkSize;
            const auto cubeY = perlinValue - chunkY * chunkSize;
            if(chunk.contents == nullptr)
            {
                chunk.contents = std::make_unique<ChunkContent>();
            }
            auto& cube = (*chunk.contents)[cubeX][cubeZ][cubeY];
            cube.flag = cube.flag | Cube::IS_VISIBLE;
            cube.cubeTextureId = GenerateTextureId(CubeType::GRASS);
            cube.cubeId = Cube::CalculateCubeId(Vec3b(cubeX, cubeY, cubeZ));
            chunk.visibleCubes.push_back(cube.cubeId);
        }
    }
}

void Region::SetChunk(const ChunkId chunkId, Chunk&& chunk)
{
    static std::mutex localMutex;
    std::lock_guard<std::mutex> lock(localMutex);
    chunks_[chunkId] = std::move(chunk);
    chunks_[chunkId].flag = Chunk::IS_VISIBLE;
}

const Chunk* Region::GetChunk(ChunkId chunkId)
{
    return &chunks_[chunkId];
}

Vec2df Region::GetRegionPos(RegionId regionId)
{
    return Vec2df();
}

ChunkId Region::GetChunkId(size_t chunkX, size_t chunkZ, size_t chunkY)
{
    return chunkX * regionHeight * regionSize + chunkZ * regionHeight + chunkY;
}
}
