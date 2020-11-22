
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
#include "engine/asset.h"
#include "engine/engine.h"
#include "utils/json_utility.h"
#include "engine/assert.h"

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko
{
AssetManager::AssetManager(FilesystemInterface& filesystem): assetLoadingJob_([this]()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Loading Asset From Asset Manager");
#endif
    BufferFile newFile = filesystem_.LoadFile(currentLoadedAsset_.assetPath);
    {
        std::lock_guard<std::mutex> lock(assetMapMutex_);
        assetMap_[currentLoadedAsset_.assetId] = std::move(newFile);
    }
    currentLoadedAsset_.assetId = INVALID_ASSET_ID;
}),
filesystem_(filesystem)
{
}

const BufferFile* AssetManager::GetResource(AssetId resourceId)
{
    std::lock_guard<std::mutex> lock(assetMapMutex_);
    const auto resourcePair = assetMap_.find(resourceId);
    if (resourcePair != assetMap_.end())
    {
        return &resourcePair->second;
    }
    return nullptr;
}

AssetId AssetManager::LoadAsset(const std::string_view assetPath)
{
    const std::string resourceMetaPath = fmt::format("{}{}", assetPath, resourceMetafile_);
    const json resourceMetaJson = LoadJson(resourceMetaPath);
    sole::uuid resourceId = INVALID_ASSET_ID;
    if(resourceMetaJson.contains("uuid"))
    {
        resourceId = sole::rebuild(resourceMetaJson["uuid"]);
    }
    else
    {
        logDebug("[Error] Resource does not have an uuid in its meta file");
        return resourceId;
    }

    const auto it = assetPathMap_.find(resourceId);
    if (it == assetPathMap_.end())
    {
        assetPathMap_[resourceId] = assetPath;
        assetLoadingQueue_.push_back({resourceId, assetPath.data()});
    }

    return resourceId;
}

void AssetManager::Init()
{
}

void AssetManager::Update([[maybe_unused]]seconds dt)
{
    if(!assetLoadingQueue_.empty())
    {
        if(currentLoadedAsset_.assetId == INVALID_ASSET_ID && 
            (!assetLoadingJob_.HasStarted() || 
            assetLoadingJob_.IsDone()))
        {
            currentLoadedAsset_ = assetLoadingQueue_.front();
            assetLoadingQueue_.erase(assetLoadingQueue_.cbegin());
            assetLoadingJob_.Reset();
            BasicEngine::GetInstance()->ScheduleJob(
                &assetLoadingJob_, 
                JobThreadType::RESOURCE_THREAD);
        }
    }
}

void AssetManager::Destroy()
{
    if(assetLoadingJob_.HasStarted() && !assetLoadingJob_.IsDone())
    {
        assetLoadingJob_.Join();
    }
}

void AssetManager::RemoveAsset(AssetId resourceId)
{
    const auto it = assetMap_.find(resourceId);
    if (it != assetMap_.end())
    {
        assetMap_.erase(it);
    }
    const auto pathIt = assetPathMap_.find(resourceId);
    if(pathIt != assetPathMap_.end())
    {
        assetPathMap_.erase(pathIt);
    }
}
}
