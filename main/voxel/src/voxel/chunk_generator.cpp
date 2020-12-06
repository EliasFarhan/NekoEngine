#include "voxel/chunk_generator.h"


#include "engine/assert.h"
#include "mathematics/vector.h"

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko::voxel
{



Chunk ChunkGenerator::GenerateChunk(RegionId regionId, ChunkId chunkId) const
{
    Chunk newChunk;
    newChunk.chunkId = chunkId;
    newChunk.flag = Chunk::IS_VISIBLE;
    const auto chunkX = chunkId / regionHeight / regionSize;
    const auto chunkZ = (chunkId - chunkX * regionHeight * regionSize) / regionHeight;
    const auto chunkY = chunkId % regionHeight;

    const Vec2df regionOriginPos = Region::GetRegionPos(regionId);
    for(size_t x = chunkX * chunkSize; x < (chunkX + 1) * chunkSize; x++)
    {
        for (size_t z = chunkZ * chunkSize; z < (chunkZ + 1) * chunkSize; z++)
        {
            const double voxelX =
                (static_cast<double>(x) - regionWidthFloat / 2.0 + regionOriginPos.x)
                / regionWidthFloat * scale;
            const double voxelZ =
                (static_cast<double>(z) - regionWidthFloat / 2.0 + regionOriginPos.y)
                / regionWidthFloat * scale;
            const auto height = pow(
                perlinNoise_.normalizedOctaveNoise2D_0_1(voxelX, voxelZ, octave),
                elevetionPow);
            const auto y = static_cast<size_t>(maxHeight * height);
            if (y >= chunkY * chunkSize && y < (chunkY+1) * chunkSize)
            {
                if(newChunk.contents == nullptr)
                {
                    newChunk.contents = std::make_unique<ChunkContent>();
                }
                const auto cubeX = x - chunkX * chunkSize;
                const auto cubeY = y - chunkY * chunkSize;
                const auto cubeZ = z - chunkZ * chunkSize;
                auto& cube = (*newChunk.contents)[cubeX][cubeZ][cubeY];
                cube.flag = cube.flag | Cube::IS_VISIBLE;
                cube.cubeTextureId = GenerateTextureId(CubeType::GRASS);
                cube.cubeId = cubeX * chunkSize * chunkSize + cubeZ * chunkSize + cubeY;
                newChunk.visibleCubes.push_back(&cube);
            }
        }
    }
    return newChunk;
}

Region ChunkGenerator::GenerateRegion(RegionId regionId) const
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Generate Region");
    EASY_BLOCK("Generate Height Map");
#endif

    const Vec2df regionOriginPos = Region::GetRegionPos(regionId);
    HeightMap heightMap{regionWidth, {{}}};
    for(int x = 0; x < regionWidth; x++)
    {
        for(int z = 0; z < regionWidth; z++)
        {
            const double voxelX = 
                (static_cast<double>(x) - regionWidthFloat / 2.0 + regionOriginPos.x)
                    / regionWidthFloat * scale;
            const double voxelZ = 
                (static_cast<double>(z) - regionWidthFloat / 2.0 + regionOriginPos.y)
                    / regionWidthFloat * scale;


            const auto height = pow(
                    perlinNoise_.normalizedOctaveNoise2D_0_1(voxelX, voxelZ, octave),
                    elevetionPow);
            heightMap[x][z] = static_cast<size_t>(maxHeight * height);
            
        }
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    Region newRegion{};
    newRegion.Init();
    newRegion.GenerateFromHeightMap(heightMap);
    return newRegion;

}

size_t ChunkGenerator::GetHeight(RegionId regionId, Vec2u cubePos) const
{
    const Vec2df regionOriginPos = Region::GetRegionPos(regionId);
    const double voxelX =
        (static_cast<double>(cubePos.x) - regionWidthFloat / 2.0 + regionOriginPos.x)
        / regionWidthFloat * scale;
    const double voxelZ =
        (static_cast<double>(cubePos.y) - regionWidthFloat / 2.0 + regionOriginPos.y)
        / regionWidthFloat * scale;
    const auto height = pow(
        perlinNoise_.normalizedOctaveNoise2D_0_1(voxelX, voxelZ, octave),
        elevetionPow);
    return static_cast<size_t>(maxHeight * height);
}
}
