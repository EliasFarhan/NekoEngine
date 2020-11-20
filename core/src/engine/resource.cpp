
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

#include "engine/log.h"
#include "engine/resource.h"
#include "engine/engine.h"
#include "utils/json_utility.h"
#include "engine/assert.h"

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko
{
ResourceManager::ResourceManager(): resourceJob_([this]()
{
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Loading Resource");
#endif
    neko_assert(!resourceQueue_.empty(), "Queue should not be empty");
    auto& resource = resourceQueue_.front();
    BufferFile newFile;
    newFile.Load(resource.assetPath);
    {
        std::lock_guard<std::mutex> lock(resourceMapMutex_);
        resourceMap_[resource.resourceId] = std::move(newFile);
    }
    resourceQueue_.erase(resourceQueue_.cbegin());
})
{
}

const BufferFile* ResourceManager::GetResource(ResourceId resourceId)
{
    std::lock_guard<std::mutex> lock(resourceMapMutex_);
    const auto resourcePair = resourceMap_.find(resourceId);
    if (resourcePair != resourceMap_.end())
    {
        return &resourcePair->second;
    }
    return nullptr;
}

ResourceId ResourceManager::LoadResource(const std::string_view assetPath)
{
    const std::string resourceMetaPath = fmt::format("{}{}", assetPath, resourceMetafile_);
    const json resourceMetaJson = LoadJson(resourceMetaPath);
    sole::uuid resourceId = INVALID_RESOURCE_ID;
    if(resourceMetaJson.contains("uuid"))
    {
        resourceId = sole::rebuild(resourceMetaJson["uuid"]);
    }
    else
    {
        logDebug("[Error] Resource does not have an uuid in its meta file");
    }

    const auto it = resourcePathMap_.find(resourceId);
    if (it == resourcePathMap_.end())
    {
        resourcePathMap_[resourceId] = assetPath;
        resourceQueue_.push_back({resourceId, assetPath.data()});
    }

    return resourceId;
}

void ResourceManager::Init()
{
}

void ResourceManager::Update([[maybe_unused]]seconds dt)
{
    if(!resourceQueue_.empty())
    {
        if(!resourceJob_.HasStarted() || resourceJob_.IsDone())
        {
            resourceJob_.Reset();
            BasicEngine::GetInstance()->ScheduleJob(
                &resourceJob_, 
                JobThreadType::RESOURCE_THREAD);
        }
    }
}

void ResourceManager::Destroy()
{
    if(resourceJob_.HasStarted() && !resourceJob_.IsDone())
    {
        resourceJob_.Join();
    }
    if (!resourceQueue_.empty())
    {
        resourceQueue_.clear();
    }
}

void ResourceManager::RemoveResource(ResourceId resourceId)
{
    const auto it = resourceMap_.find(resourceId);
    if (it != resourceMap_.end())
    {
        resourceMap_.erase(it);
    }
    const auto pathIt = resourcePathMap_.find(resourceId);
    if(pathIt != resourcePathMap_.end())
    {
        resourcePathMap_.erase(pathIt);
    }
}
}
