#pragma once

#include <map>
#include <future>
#include "sole.hpp"
#include "utilities/json_utility.h"
#include "utilities/file_utility.h"

namespace neko
{

using ResourceId = sole::uuid;
const ResourceId INVALID_RESOURCE_ID = sole::uuid();

class ResourceManager
{
public:
    ResourceManager();
    void Close();
    void LoadingLoop();
    bool IsResourceReady(ResourceId resourceId);
    std::string GetResource(ResourceId resourceId);
    ResourceId LoadResource(const std::string assetPath);
private:
    std::map<ResourceId, std::string> resourceLoaded_;
    std::map<ResourceId, std::string> resourcePathQueue_;
    std::vector<ResourceId> pathQueue_;
    std::thread thread_;
    std::atomic<bool> isRunning_;
    std::condition_variable cv_;
    std::mutex mutex_;
};
}
