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

struct Resource
{
    Resource() = default;

    virtual ~Resource() = default;

    Resource(const Resource&) = delete;

    Resource(Resource&&) = default;

    ResourceId resourceId = INVALID_RESOURCE_ID;
    std::string assetPath;
};

template<class T=Resource>
class ResourceManager
{
    const T* GetResource(ResourceId resourceId)
    {
        const auto resourcePair = resourceMap_.find(resourceId);
        const auto resourceThread = resourceThread_.find(resourceId);
        if (resourcePair != resourceMap_.end())
        {
            return resourcePair->second;
        } else if (resourceThread != resourceThread_.end())
        {
            T value = resourceFuture_[resourceId].get(0);
            resourceMap_[resourceId] = value;
            resourceThread_[resourceId].join();
            return value;
        }
        return nullptr;
    }

    ResourceId LoadResource(const std::string_view assetPath)
    {
        const std::string resourceMetaPath = assetPath + resourceMetafile_;
        const json resourceMetaJson = LoadJson(assetPath);
        std::promise<T> p;
        std::future<T> f3 = p.get_future();
        std::thread workThread(LoadResource, resourceMetaJson, std::move(p));
        ResourceId id;
        resourceFuture_[id] = f3;
        resourceThread_[id] = workThread;
        workThread.join();
        return resourceId;
    }

protected:
    const std::string_view resourceMetafile_ = ".n_meta";

    virtual ResourceId LoadResource(const json& resoureceMetaJson, std::promise<T> promise) = 0;
    std::map<ResourceId, T> resourceMap_;
    std::map<ResourceId, std::thread> resourceThread_;
    std::map<ResourceId, std::future<T>> resourceFuture_;
    std::map<ResourceId, std::string> resourcePathMap_;
};

template<class T>
bool HasValidExtension(const std::string_view assetPath);


}