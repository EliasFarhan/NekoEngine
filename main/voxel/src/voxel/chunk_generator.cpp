#include "voxel/chunk_generator.h"


#include "engine/assert.h"
#include "mathematics/vector.h"

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko::voxel
{
Region ChunkGenerator::GenerateRegion(RegionId regionId) const
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Generate Region");
    EASY_BLOCK("Generate Height Map");
#endif
    const double maxHeight = regionHeight * chunkSize;
    const std::int32_t octave = 5;
    const double scale = 4.0;
    const double elevetionPow = 5.0;
    const int regionWidth = regionSize * chunkSize;
    const auto regionWidthFloat = static_cast<double>(regionWidth);

    // TODO implement space filling curve centered around 0,0
    const Vec2df regionOriginPos = Vec2df();
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
}
