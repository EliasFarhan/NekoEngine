#pragma once

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
