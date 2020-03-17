#pragma once

#include <map>
#include <future>
#include "sole.hpp"

namespace neko
{

using ResourceId = sole::uuid;
using Path = std::string_view;
const ResourceId INVALID_RESOURCE_ID = sole::uuid();


/**
 * \brief Custom promise waiting for a resource
 */
struct LoadPromise {
    std::string resource = "";
    Path path = "";
    bool ready = false;
    LoadPromise(Path newPath) : path(newPath) {}
    LoadPromise() {}
};

class ResourceManager
{
public:
    ResourceManager();
    void Destroy();
    bool IsResourceReady(ResourceId resourceId);
    std::string GetResource(ResourceId resourceId);
    ResourceId LoadResource(Path assetPath);
private:
    void LoadingLoop();
    enum ResourceManagerStatus : std::uint8_t
    {
        IS_RUNNING = 1, //To check if the ResourceManager is running
        IS_NOT_EMPTY = 10, //To check if the ResourceManager has tasks
        IS_WAITING = 100, //To check if the ResourceManager is waiting for a task
    };
    std::unordered_map<ResourceId, LoadPromise> resourcePromises_;
    std::vector<ResourceId> idQueue_;
    std::thread loadingThread_;
    std::atomic<std::uint8_t> status_;
    std::condition_variable cv_;
    std::mutex loadingMutex_;
    
};
}
