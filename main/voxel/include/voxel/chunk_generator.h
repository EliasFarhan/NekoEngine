#pragma once
#include <PerlinNoise.hpp>

#include "chunk.h"
#include "region.h"

namespace neko::voxel
{

class ChunkGenerator
{
public:
    [[nodiscard]] Region GenerateRegion(RegionId regionId) const;
private:
    siv::PerlinNoise perlinNoise_{};

};

}
