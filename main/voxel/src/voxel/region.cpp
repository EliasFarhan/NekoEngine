#include "voxel/region.h"

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
            cube.cubeId = cubeX * chunkSize * chunkSize + cubeZ * chunkSize + cubeY;
            chunk.visibleCubes.push_back(&cube);
        }
    }
}
}
