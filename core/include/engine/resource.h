#pragma once

#include <map>
#include <future>
#include "sole.hpp"

namespace neko
{

using ResourceId = sole::uuid;
using Path = std::string_view;
const ResourceId INVALID_RESOURCE_ID = sole::uuid();

class ResourceManager
{
public:
    ResourceManager();
    void Close();
    void LoadingLoop();
    bool IsResourceReady(ResourceId resourceId);
    std::string GetResource(ResourceId resourceId);
    ResourceId LoadResource(Path assetPath);
private:
    //std::unordered_map<ResourceId, std::promise<const char*>> resourceFutures_;
    std::unordered_map<ResourceId, std::string> resourceLoaded_;
    std::unordered_map<ResourceId, Path> resourcePath_;
    std::vector<ResourceId> idQueue_;
    std::thread loadingThread_;
    std::atomic<std::uint8_t> isRunning_;
    std::condition_variable cv_;
    std::mutex loadingMutex_;
};
}
