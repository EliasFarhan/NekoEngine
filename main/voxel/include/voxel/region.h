#pragma once

#include <vector>

#include "mathematics/vector.h"
#include "voxel/chunk.h"

namespace neko::voxel
{

const size_t regionHeight = 10;
const size_t regionSize = 64;
using RegionId = std::uint32_t;
using HeightMap = std::vector<std::array<size_t, regionSize * chunkSize>> ;
class Region
{
public:
    void Init();
    [[nodiscard]] const std::vector<Chunk>& GetChunks() const;
    void GenerateFromHeightMap(const HeightMap& map);
    void SetChunk(const ChunkId uint16, Chunk&& chunk);
    [[nodiscard]] const Chunk* GetChunk(ChunkId chunkId);
    [[nodiscard]] static Vec2df GetRegionPos(RegionId regionId);
    [[nodiscard]] static ChunkId GetChunkId(size_t chunkX, size_t chunkZ, size_t chunkY);
private:
    std::vector<Chunk> chunks_{};
    RegionId regionId_ = 0;
};
}
