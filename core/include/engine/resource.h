#pragma once

#include <map>
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
        if (resourcePair != resourceMap_.end())
        {
            return resourcePair->second;
        }
        return nullptr;
    }

    ResourceId LoadResource(const std::string_view assetPath)
    {
        const std::string resourceMetaPath = std::string(assetPath.data()) + resourceMetafile_.data();
        const json resourceMetaJson = LoadJson(assetPath);
        const auto resourceId = LoadResource(resourceMetaJson);
        return resourceId;
    }

protected:
    const std::string_view resourceMetafile_ = ".n_meta";

    virtual ResourceId LoadResource(const json& resoureceMetaJson) = 0;

    std::map<ResourceId, T> resourceMap_;
    std::map<ResourceId, std::string> resourcePathMap_;
};

template<class T>
bool HasValidExtension(const std::string_view assetPath);


}