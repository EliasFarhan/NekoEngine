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

bool TextureManager::HasValidExtension(const std::string_view filename)
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

Index TextureManager::LoadTexture(const std::string_view filename)
{
    if (!HasValidExtension(filename))
    {
        std::ostringstream oss;
        oss << "[ERROR] Texture path: " << filename << " has invalid extension";
        logDebug(oss.str());
        return INVALID_INDEX;
    }
    Index textureIndex = INVALID_INDEX;
    for (auto i = 0u; i < texturePaths_.size(); i++)
    {
        if (filename == texturePaths_[i])
        {
            textureIndex = i;
            break;
        }
    }
    //Was or still is loaded
    if (textureIndex != INVALID_INDEX)
    {
        //Check if the texture was destroyed
        if (textures_[textureIndex].getNativeHandle() != 0U)
        {
            return textureIndex;
        }
        else
        {
            if (!textures_[textureIndex].loadFromFile(filename.data()))
            {
                std::ostringstream oss;
                oss << "[ERROR] Could not load texture file: " << filename;
                logDebug(oss.str());
                return INVALID_INDEX;
            }
            return textureIndex;
        }
    }
    //Texture was never loaded
    if (FileExists(filename))
    {
        auto texture = sf::Texture();
        if (!texture.loadFromFile(filename.data()))
        {
            std::ostringstream oss;
            oss << "[ERROR] Could not load texture file: " << filename;
            logDebug(oss.str());
            return INVALID_INDEX;
        }
        textures_.emplace_back(sf::Texture(texture));
        texturePaths_.emplace_back(filename.data());

        return static_cast<Index>(textures_.size() - 1);
    }
    else
    {
        std::ostringstream oss;
        oss << "[ERROR] Could not load texture file: " << filename;
        logDebug(oss.str());
    }
    return INVALID_INDEX;
}

const sf::Texture* TextureManager::GetTexture(const Index index) const
{
    if (index == INVALID_INDEX || index >= textures_.size())
    {
        return {}; // Same as return nullptr.
    }
    return &textures_[index];
}

std::string TextureManager::GetTexturePath(const Index index) const
{
    if(index == INVALID_INDEX)
        return "None";
    return texturePaths_[index];
}

size_t TextureManager::GetTextureCount() const
{
    return textures_.size();
}

} // namespace neko
