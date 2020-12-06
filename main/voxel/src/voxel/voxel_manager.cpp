#include <voxel/voxel_manager.h>

#include "engine/engine.h"

namespace neko::voxel
{
void VoxelManager::Init()
{
    regions_.push_back({});
    auto& region = regions_.front();
    region.Init();
    const auto chunkX = regionSize / 2;
    const auto chunkZ = regionSize / 2;
    const Vec2u cubePos( chunkX * chunkSize,  chunkZ * chunkSize);
    const auto height = chunkGenerator_.GetHeight(0, cubePos);
    const auto chunkY = height / chunkSize;
    const auto chunkId = Region::GetChunkId(chunkX, chunkZ, chunkY);

    chunkLoadingQueue_.push(Job{ [this, chunkId]()
        {
            regions_.front().SetChunk(chunkId, chunkGenerator_.GenerateChunk(0, chunkId));
        } });
    BasicEngine::GetInstance()->ScheduleJob(&chunkLoadingQueue_.back(), JobThreadType::RESOURCE_THREAD);
}

void VoxelManager::Update(seconds dt)
{
    while (!chunkLoadingQueue_.empty())
    {
        if(chunkLoadingQueue_.front().IsDone())
        {
            chunkLoadingQueue_.pop();
        }
        else
        {
            break;
        }
    }
}

void VoxelManager::Destroy()
{
}

std::vector<Chunk*> VoxelManager::GetChunks() const
{
}
}
