#include <engine/resource.h>


#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
#include "engine/assert.h"
#include "utilities/file_utility.h"
#include "utilities/json_utility.h"

neko::ResourceManager::ResourceManager()
{
    status_ = IS_RUNNING;
}

void neko::ResourceManager::Init()
{
    loadingThread_ = std::thread([this]() { this->LoadingLoop(); });
}

void neko::ResourceManager::Destroy()
{
    status_ &= ~IS_RUNNING;
    cv_.notify_one();
    loadingThread_.join();
}

void neko::ResourceManager::LoadingLoop()
{
#ifdef EASY_PROFILE_USE
    EASY_THREAD("ResourceManager");
    EASY_BLOCK("Loop");
#endif

    while (status_ & IS_RUNNING) 
    {
        if (status_ & IS_NOT_EMPTY) 
        {
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("Load", profiler::colors::Blue);
#endif
            ResourceId resourceToLoad;
            {
                std::lock_guard<std::mutex> lockGuard(loadingMutex_);
                resourceToLoad = idQueue_[0];
            }
            Path path = resourcePromises_[resourceToLoad].path;
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("FileLoading", profiler::colors::Blue600);
#endif
            resourcePromises_[resourceToLoad].resource.Load(path);
#ifdef EASY_PROFILE_USE
            EASY_END_BLOCK;
#endif
            resourcePromises_[resourceToLoad].ready = true;

            {
                std::lock_guard<std::mutex> lockGuard(loadingMutex_);
                idQueue_.erase(idQueue_.begin());
                if (idQueue_.empty())
                {
                    status_ &= ~IS_NOT_EMPTY;
                }
            }
#ifdef EASY_PROFILE_USE
            EASY_END_BLOCK;
#endif
        } else 
        {
            std::unique_lock<std::mutex> lock(loadingMutex_);
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("Wait", profiler::colors::Blue);
#endif
            if (!(status_ & IS_NOT_EMPTY))
            {
                cv_.wait(lock);
            }
#ifdef EASY_PROFILE_USE
            EASY_END_BLOCK;
#endif
        }
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
}

bool neko::ResourceManager::IsResourceReady(ResourceId resourceId)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("IsReady");
#endif
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    return resourcePromises_[resourceId].ready;
}


std::string neko::ResourceManager::GetResource(ResourceId resourceId)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("GetResource");
#endif
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    if (resourcePromises_[resourceId].ready)
    {
#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif
        return resourcePromises_[resourceId].resource.dataBuffer;
    }
    neko_assert(resourcePromises_[resourceId].ready, "Resource not ready");
}

neko::ResourceId neko::ResourceManager::LoadResource(const Path assetPath)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Add to queue");
#endif
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Generate UUID", profiler::colors::Red200);
#endif
    const ResourceId resourceId = sole::uuid4();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    idQueue_.push_back(resourceId);
    resourcePromises_[resourceId] = LoadPromise(assetPath);
    status_ |= IS_NOT_EMPTY;
    cv_.notify_all();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    return resourceId;
}
