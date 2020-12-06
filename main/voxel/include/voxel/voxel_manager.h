#pragma once

#include <queue>

#include "chunk_generator.h"
#include "region.h"
#include "engine/jobsystem.h"

namespace neko::voxel
{
class VoxelManager : public SystemInterface
{
public:
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;

    [nodiscard] std::vector<Chunk*> GetChunks() const;

private:
    ChunkGenerator chunkGenerator_;
    std::vector<Chunk*> currentChunks_;
    std::vector<Region> regions_;
    std::queue<Job> chunkLoadingQueue_;
};
}
