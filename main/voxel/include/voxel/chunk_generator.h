#pragma once
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
    static constexpr double scale = 4.0;
    static constexpr double elevetionPow = 5.0;
    static constexpr double maxHeight = regionHeight * chunkSize;
    static constexpr int regionWidth = regionSize * chunkSize;
    static constexpr auto regionWidthFloat = static_cast<double>(regionWidth);
};

}
