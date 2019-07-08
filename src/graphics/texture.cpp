/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <graphics/texture.h>
#include <set>
#include <sstream>
#include "engine/log.h"
#include "engine/globals.h"
#include "utilities/file_utility.h"

namespace neko
{

static std::set<std::string_view> imgExtensionSet
        {
                ".png",
                ".jpg",
                ".jpeg",
                ".bmp",
                ".tga",
                ".gif",
                ".psd",
                ".hdr",
                ".pic"
        };


TextureManager::TextureManager()
{
    texturePaths_.reserve(INIT_ENTITY_NMB);
    textures_.reserve(INIT_ENTITY_NMB);
}

bool TextureManager::HasValidExtension(const std::string& filename)
{
    const auto filenameExtensionIndex = filename.find_last_of('.');
    if (filenameExtensionIndex >= filename.size())
    {
        return false;
    }

    //Check extension first
    const std::string_view extension(&filename.at(filenameExtensionIndex));
    return imgExtensionSet.find(extension) != imgExtensionSet.end();
}

Index TextureManager::LoadTexture(std::string filename)
{


    if (!HasValidExtension(filename))
    {
        std::ostringstream oss;
        oss << "[ERROR] Texture path: " << filename << " has invalid extension";
        logDebug(oss.str());
        return INDEX_INVALID;
    }
    int textureIndex = INDEX_INVALID;
    for (auto i = 0u; i < texturePaths_.size(); i++)
    {
        if (filename == texturePaths_[i])
        {
            textureIndex = i;
            break;
        }
    }
    //Was or still is loaded
    if (textureIndex != INDEX_INVALID)
    {
        //Check if the texture was destroyed
        if (textures_[textureIndex]->getNativeHandle() != 0U)
        {
            return textureIndex;
        }
        else
        {
            if (!textures_[textureIndex]->loadFromFile(filename))
            {
                std::ostringstream oss;
                oss << "[ERROR] Could not load texture file: " << filename;
                logDebug(oss.str());
                return INDEX_INVALID;
            }
            return textureIndex;
        }
    }
    //Texture was never loaded
    if (FileExists(filename))
    {
        auto texture = sf::Texture();
        if (!texture.loadFromFile(filename))
        {
            std::ostringstream oss;
            oss << "[ERROR] Could not load texture file: " << filename;
            logDebug(oss.str());
            return INDEX_INVALID;
        }
        textures_.emplace_back(std::make_shared<sf::Texture>(texture));
        texturePaths_.push_back(filename);

        return static_cast<Index>(textures_.size() - 1);
    }
    else
    {
        std::ostringstream oss;
        oss << "[ERROR] Could not load texture file: " << filename;
        logDebug(oss.str());
    }
    return INDEX_INVALID;
}

const std::shared_ptr<sf::Texture> TextureManager::GetTexture(const Index index) const
{
    if (index == INDEX_INVALID || index >= textures_.size())
    {
        return {}; // Same as return nullptr.
    }
    return textures_[index];
}

} // namespace neko
