#pragma once

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

#include <map>
#include "sole.hpp"
#include "utils/json_utility.h"
#include "utils/file_utility.h"

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