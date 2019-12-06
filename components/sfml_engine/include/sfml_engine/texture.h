#pragma once

/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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
#include <set>
#include <SFML/Graphics/Texture.hpp>
#include "mathematics/vector.h"

#include <xxhash.hpp>
#include <unordered_map>
#include <graphics/texture.h>
#include "utilities/json_utility.h"

namespace neko::sfml
{

struct Texture : Resource
{
	Vec2f origin{};
	sf::Texture texture;
};

using TextureId = xxh::hash_t<64>;
const TextureId INVALID_TEXTURE_ID = TextureId(0);

class TextureManager : public neko::TextureManager<Texture>
{
public:

    TextureManager();
    static std::string_view GetMetaExtension();
	//Return an index to avoid pointer invalidation
	TextureId LoadTexture(const std::string& filename);
	const Texture* GetTexture(const TextureId textureId) const;
	std::string GetTexturePath(TextureId textureId)const;
	const std::unordered_map<TextureId, std::string>& GetTextureNameMap();
	void SetTextureOrigin(TextureId textureId, Vec2f origin);
    void SetTextureSmooth(TextureId textureId, bool isSmooth);
protected:
    static void LoadMetadata(const std::string& metadataPath, Texture& texture);
    static void CreateEmptyMetaFile(const std::string& filename);
    ResourceId LoadResource(const json& resoureceMetaJson) override;

    std::unordered_map<TextureId, Texture> textures_;
	std::unordered_map<TextureId, std::string> textureNames_;
};



}
namespace neko
{
template<>
bool HasValidExtension<sfml::Texture>(const std::string_view assetPath);
}