#include <engine/resource.h>


#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#include <easy/arbitrary_value.h> // EASY_VALUE, EASY_ARRAY are defined here
#endif
#include "engine/assert.h"
#include "utilities/file_utility.h"
#include "utilities/json_utility.h"

neko::ResourceManager::ResourceManager()
{
    status_ = ~IS_RUNNING;
}

void neko::ResourceManager::Init()
{
    status_ = IS_RUNNING;
    loadingThread_ = std::thread([this]()
    {
        this->LoadingLoop();
    });
}

void neko::ResourceManager::Destroy()
{
    status_ &= ~IS_RUNNING;
    cv_.notify_one();
    loadingThread_.join();
    for (auto resourcePromise : resourcePromises_)
    {
        resourcePromise.second.resource.Destroy();
    }
    resourcePromises_.clear();
    idQueue_.clear();
}

void neko::ResourceManager::LoadingLoop()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Loop", profiler::colors::Blue200);
#endif

    while (status_ & IS_RUNNING) 
    {
        if (status_ & IS_NOT_EMPTY) 
        {
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("Load", profiler::colors::Blue);
#endif
            ResourceId resourceToLoad;
            LoadPromise promise;
            {
#ifdef EASY_PROFILE_USE
                EASY_BLOCK("Get Queue and Promise", profiler::colors::Red);
#endif
                std::lock_guard<std::mutex> lockGuard(loadingMutex_);
                resourceToLoad = idQueue_[0];
                promise = resourcePromises_[resourceToLoad];
#ifdef EASY_PROFILE_USE
                EASY_END_BLOCK;
#endif
            }
#ifdef EASY_PROFILE_USE
            EASY_BLOCK("File Loading", profiler::colors::Blue600);
#endif
            promise.resource.Load(promise.path);
#ifdef EASY_PROFILE_USE
            EASY_END_BLOCK;
#endif
            promise.ready = true;

            {
#ifdef EASY_PROFILE_USE
                EASY_BLOCK("Erase Queue and Set Promise", profiler::colors::Red);
#endif
                std::lock_guard<std::mutex> lockGuard(loadingMutex_);
#ifdef EASY_PROFILE_USE
                EASY_BLOCK("Set Promise", profiler::colors::Red200);
#endif
                resourcePromises_[resourceToLoad] = promise;
#ifdef EASY_PROFILE_USE
                EASY_END_BLOCK;
#endif
#ifdef EASY_PROFILE_USE
                EASY_BLOCK("Erase Queue", profiler::colors::Red200);
#endif
                idQueue_.erase(idQueue_.begin());
                if (idQueue_.empty())
                {
                    status_ &= ~IS_NOT_EMPTY;
                }
#ifdef EASY_PROFILE_USE
                EASY_END_BLOCK;
                EASY_END_BLOCK;
#endif
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

bool neko::ResourceManager::IsResourceReady(const ResourceId resourceId)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("IsResourceReady", profiler::colors::Red);
#endif
    bool ready;
    {
        const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
        ready = resourcePromises_[resourceId].ready;
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Read", profiler::colors::Red200);
#endif
    return ready;
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
}


neko::BufferFile neko::ResourceManager::GetResource(const ResourceId resourceId)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("GetResource", profiler::colors::Red);
#endif
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
    neko_assert(resourcePromises_[resourceId].ready, "Resource not ready");
    return resourcePromises_[resourceId].resource;
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
}

neko::ResourceId neko::ResourceManager::LoadResource(const Path assetPath)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("LoadResource", profiler::colors::Blue);
#endif
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Generate UUID", profiler::colors::Purple);
#endif
    const ResourceId resourceId = sole::uuid4();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Add to queue", profiler::colors::Red);
#endif
    {
        const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
        idQueue_.push_back(resourceId);
        resourcePromises_[resourceId] = LoadPromise(assetPath);
    }
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
    status_ |= IS_NOT_EMPTY;
    cv_.notify_all();
    return resourceId;
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
}


void neko::ResourceManager::DeleteResource(const ResourceId resourceId)
{
    const std::lock_guard<std::mutex> lockGuard(loadingMutex_);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Deleting Resource");
#endif
    resourcePromises_[resourceId].resource.Destroy();
    resourcePromises_.erase(resourceId);
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK;
#endif
}
