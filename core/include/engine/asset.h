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

using AssetId = sole::uuid;
const AssetId INVALID_ASSET_ID = sole::uuid();


/*
 * \brief The Asset Manager stores the BufferFile using the Asset Id.
 * It will load one after another asset on the Resource thread.
 * 
 */
class AssetManager : public SystemInterface
{
public:
    AssetManager();
    /**
     * \brief Return the non-owning pointer to the BufferFile
     * \return Non owning pointer to the Buffer file, nullptr if not yet loaded
     */
    const BufferFile* GetResource(AssetId resourceId);
    /**
     * \brief This function will get the asset id from the meta file
     * and will return it. If the asset was not yet loaded, it will put it
     * in the asset loading queue.
     */
    AssetId LoadAsset(const std::string_view assetPath);
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    /**
     * \brief Remove the asset content from our internal storage
     */
    void RemoveAsset(AssetId resourceId);

protected:
    struct Asset
    {
        AssetId assetId = INVALID_ASSET_ID;
        std::string assetPath;
    };
    const std::string_view resourceMetafile_ = ".meta";

    Asset currentLoadedAsset_ {};
    std::map<AssetId, BufferFile> assetMap_;
    std::map<AssetId, std::string> assetPathMap_;
    std::vector<Asset> assetLoadingQueue_;
    Job assetLoadingJob_;
    std::mutex assetMapMutex_;

};


}