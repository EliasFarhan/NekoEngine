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
#include "engine/system.h"
#include "utils/file_utility.h"

namespace neko
{

using ResourceId = sole::uuid;
const ResourceId INVALID_RESOURCE_ID = sole::uuid();

struct Resource
{
    ResourceId resourceId = INVALID_RESOURCE_ID;
    std::string assetPath;
};

class ResourceManager : public SystemInterface
{
public:
    ResourceManager();

    const BufferFile* GetResource(ResourceId resourceId);

    ResourceId LoadResource(const std::string_view assetPath);
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    void RemoveResource(ResourceId resourceId);

protected:
    const std::string_view resourceMetafile_ = ".meta";

    Resource currentLoadedResource_ {};
    std::map<ResourceId, BufferFile> resourceMap_;
    std::map<ResourceId, std::string> resourcePathMap_;
    std::vector<Resource> resourceQueue_;
    Job resourceJob_;
    std::mutex resourceMapMutex_;

};


}