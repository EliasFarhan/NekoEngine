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

#include <voxel/voxel_manager.h>

#include "engine/engine.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko::voxel
{
void VoxelManager::Init()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Init Voxel Manager");
#endif
    regions_.emplace_back();
    auto& region = regions_.front();
    const auto chunkX = regionSize / 2;
    const auto chunkZ = regionSize / 2;
    region.Init();
    {

        const Vec2u cubePos(chunkX * chunkSize, chunkZ * chunkSize);
        const auto height = chunkGenerator_.GetHeight(0, cubePos);
        initialHeight_ = float(height) - float(regionHeight / 2 * chunkSize);
    }
    const auto workerNumber = BasicEngine::GetInstance()->GetWorkersNumber();
    initJob = Job([this, chunkX, chunkZ, workerNumber]()
        {
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("Init Job Voxel Manager");
#endif
            for (int dx = -renderDistance_; dx <= renderDistance_; dx++)
            {
                for (int dz = -renderDistance_; dz <= renderDistance_; dz++)
                {
                    const auto newChunkX = chunkX + dx;
                    const auto newChunkZ = chunkZ + dz;
                    const Vec2u cubePos(newChunkX * chunkSize, newChunkZ * chunkSize);

                    const auto height = chunkGenerator_.GetHeight(0, cubePos);

                    auto minHeight = height;
                    auto maxHeight = height;
                    for (unsigned int cubeDx = 0; cubeDx < chunkSize; cubeDx++)
                    {
                        for (unsigned int cubeDz = 0; cubeDz < chunkSize; cubeDz++)
                        {
                            const auto tmpHeight = chunkGenerator_.GetHeight(0, cubePos + Vec2u(cubeDx, cubeDz));
                            if (tmpHeight > maxHeight)
                            {
                                maxHeight = tmpHeight;
                            }
                            if (tmpHeight < minHeight)
                            {
                                minHeight = tmpHeight;
                            }
                        }
                    }
                    const int minDiff = int(minHeight) - int(height);
                    const int maxDiff = int(maxHeight) - int(height);
                    for (int dy = minDiff; dy <= maxDiff; dy++)
                    {
                        const auto newChunkY = (height + dy) / chunkSize;
                        const auto chunkId = Region::GetChunkId(newChunkX, newChunkZ, newChunkY);
                        chunkLoadingQueue_.push(ChunkLoadingJob{ Job{ [this, chunkId]()
                        {
                            #ifdef EASY_PROFILE_USE
                                EASY_BLOCK("Generating Chunk");
                            #endif
                            regions_.front().SetChunk(chunkId, chunkGenerator_.GenerateChunk(0, chunkId));
                        } }, chunkId, 0 });

                        BasicEngine::GetInstance()->ScheduleJob(&chunkLoadingQueue_.back().loadingJob,
                                                            JobThreadType::OTHER_THREAD);
                    }
                }
            }
        });
    BasicEngine::GetInstance()->ScheduleJob(&initJob, JobThreadType::RESOURCE_THREAD);
    
}

void VoxelManager::Update(seconds dt)
{
    if (!initJob.IsDone())
        return;
    while (!chunkLoadingQueue_.empty())
    {
        if(chunkLoadingQueue_.front().loadingJob.IsDone())
        {
            const auto& chunkLoadingJob = chunkLoadingQueue_.front();
            currentChunks_.push_back(regions_[chunkLoadingJob.regionId]
                .GetChunk(chunkLoadingJob.chunkId));
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

const std::vector<const Chunk*>& VoxelManager::GetChunks() const
{
    return currentChunks_;
}

float VoxelManager::GetInitialHeight() const
{
    return initialHeight_;
}
}
