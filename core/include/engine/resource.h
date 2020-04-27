#pragma once

#include <map>
#include <future>
#include "sole.hpp"
#include "utilities/file_utility.h"

namespace neko
{

using ResourceId = sole::uuid;
using Path = std::string_view;
const ResourceId INVALID_RESOURCE_ID = sole::uuid();


/**
 * \brief Custom promise waiting for a resource
 */
struct Resource {
    BufferFile resource;
    Path relativePath = "";
    Path archivedPath = "";
    bool ready = false;
    Resource(Path newAssetPath) : relativePath(newAssetPath) {}
    Resource(Path newArchivedPath, Path newAssetPath) : relativePath(newAssetPath), archivedPath(newArchivedPath) {}
    Resource() {}
};

class ResourceManager
{
public:
    ResourceManager();
    void Init();
    void Destroy();
    bool IsResourceReady(const ResourceId resourceId);
    neko::BufferFile GetResource(const ResourceId resourceId);
    ResourceId LoadResource(const Path assetPath);
    ResourceId LoadArchivedResource(const Path archivedPath, const Path relativePath);
    void DeleteResource(const ResourceId resourceId);

private:
    void LoadingLoop();

    enum ResourceManagerStatus : std::uint8_t
    {
        IS_RUNNING = 1 << 1, //To check if the ResourceManager is running
        IS_NOT_EMPTY = 1 << 2, //To check if the ResourceManager has no tasks
    };
    std::unordered_map<ResourceId, Resource> resourcePromises_;
    std::vector<ResourceId> idQueue_;
    std::atomic<std::uint8_t> status_;
    std::thread loadingThread_;
    std::condition_variable cv_;
    std::mutex loadingMutex_;
    
};
}
