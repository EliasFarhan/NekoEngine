#pragma once

#include <queue>

#include "chunk_generator.h"
#include "region.h"
#include "engine/jobsystem.h"

namespace neko::voxel
{

struct ChunkLoadingJob
{
    Job loadingJob;
    ChunkId chunkId = 0;
    RegionId regionId = 0;
};

class VoxelManager : public SystemInterface
{
public:
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;

    [[nodiscard]] const std::vector<const Chunk*>& GetChunks() const;
    [[nodiscard]] float GetInitialHeight() const;

private:
    const int renderDistance_ = 10;
    float initialHeight_ = 0.0f;
    ChunkGenerator chunkGenerator_;
    std::vector<const Chunk*> currentChunks_;
    std::vector<Region> regions_;
    std::queue<ChunkLoadingJob> chunkLoadingQueue_;
    Job initJob{ []() {} };
};
}
