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

#include "voxel/chunk_generator.h"


#include "engine/assert.h"
#include "mathematics/basic.h"
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
    const auto chunkX = chunkId / regionHeight / regionSize;
    const auto chunkZ = (chunkId - chunkX * regionHeight * regionSize) / regionHeight;
    const auto chunkY = chunkId % regionHeight;

    const Vec2df regionOriginPos = Region::GetRegionPos(regionId);
    for (size_t x = chunkX * chunkSize; x < (chunkX + 1) * chunkSize; x++)
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
            if (y < chunkY * chunkSize || y >= (chunkY + 1) * chunkSize)
                continue;
            if (newChunk.contents == nullptr)
            {
                newChunk.contents = std::make_unique<ChunkContent>();
            }
            const auto cubeX = x - chunkX * chunkSize;
            const auto cubeY = y - chunkY * chunkSize;
            const auto cubeZ = z - chunkZ * chunkSize;

            auto& cube = (*newChunk.contents)[cubeX][cubeZ][cubeY];
            cube.flag = cube.flag | Cube::IS_VISIBLE;
            cube.cubeTextureId = GenerateTextureId(CubeType::GRASS);
            cube.cubeId = Cube::CalculateCubeId(Vec3b(cubeX, cubeY, cubeZ));
            newChunk.visibleCubes.push_back(cube.cubeId);

            int maxDiff = 0;
            for (int dx = -1; dx <= 1; dx++)
            {
                if (x == 0 || x >= (chunkX + 1) * chunkSize)
                    continue;
                if (!(x - dx >= chunkX * chunkSize && x - dx < (chunkX + 1) * chunkSize))
                    continue;
                for (int dz = -1; dz <= 1; dz++)
                {
                    if (z == 0 || z >= (chunkZ + 1) * chunkSize)
                        continue;
                    if (!(z - dz >= chunkZ * chunkSize && z - dz < (chunkZ + 1) * chunkSize))
                        continue;
                    if (dx == 0 && dz == 0)
                        continue;
                    const double otherVoxelX =
                        (static_cast<double>(x - dx) - regionWidthFloat / 2.0 + regionOriginPos.x)
                        / regionWidthFloat * scale;
                    const double otherVoxelZ =
                        (static_cast<double>(z - dz) - regionWidthFloat / 2.0 + regionOriginPos.y)
                        / regionWidthFloat * scale;
                    const auto otherHeight = pow(
                        perlinNoise_.normalizedOctaveNoise2D_0_1(otherVoxelX, otherVoxelZ, octave),
                        elevetionPow);
                    const auto otherY = static_cast<size_t>(maxHeight * otherHeight);
                    const int diff = y - otherY;
                    if (maxDiff < diff)
                    {
                        for (int dy = maxDiff; dy < diff; dy++)
                        {
                            if(dy == 0)
                                continue;
                            if (y - dy < chunkY * chunkSize || y - dy >= (chunkY + 1) * chunkSize)
                                continue;
                            //generate below cubes
                            const auto otherCubeX = x - chunkX * chunkSize;
                            const auto otherCubeY = y - dy - chunkY * chunkSize;
                            const auto otherCubeZ = z - chunkZ * chunkSize;
                            neko_assert(otherCubeX < chunkSize, "Valid cube X");
                            neko_assert(otherCubeZ < chunkSize, "Valid cube Z");
                            neko_assert(otherCubeY < chunkSize, "Valid cube Y");
                            
                            auto& otherCube = (*newChunk.contents)[otherCubeX][otherCubeZ][otherCubeY];
                            otherCube.flag = otherCube.flag | Cube::IS_VISIBLE;
                            otherCube.cubeTextureId = GenerateTextureId(CubeType::ROCK);
                            otherCube.cubeId = Cube::CalculateCubeId(Vec3b(otherCubeX, otherCubeY, otherCubeZ));
                            newChunk.visibleCubes.push_back(otherCube.cubeId);
                            
                        }
                        maxDiff = diff;
                    }
                }
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
    HeightMap heightMap{ regionWidth, {{}} };
    for (int x = 0; x < regionWidth; x++)
    {
        for (int z = 0; z < regionWidth; z++)
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
