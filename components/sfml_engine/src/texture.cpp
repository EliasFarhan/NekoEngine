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
#include <set>
#include <sstream>
#include "sfml_engine/texture.h"

#include "engine/log.h"
#include "engine/globals.h"
#include "utilities/file_utility.h"
#include "utilities/json_utility.h"

namespace neko::sfml
{
static auto textureMetadataExtension = ".ntx_meta";
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

TextureId TextureManager::LoadTexture(const std::string& filename)
{
	if (!HasValidExtension(filename))
	{
		std::ostringstream oss;
		oss << "[ERROR] Texture path: " << filename << " has invalid extension";
		logDebug(oss.str());
		return INVALID_INDEX;
	}
	xxh::hash_state_t<64> hash_stream(0);
	hash_stream.update(filename);
	TextureId textureId = hash_stream.digest();
	auto textureMapKey = textures_.find(textureId);
	//Was or still is loaded
	if (textureMapKey != textures_.end())
	{
		sf::Texture& texture = textureMapKey->second.texture;
		//Check if the texture was destroyed
		if (texture.getNativeHandle() != 0U)
		{
			return textureId;
		}
		else
		{
			if (!texture.loadFromFile(filename))
			{
				std::ostringstream oss;
				oss << "[ERROR] Could not load texture file: " << filename;
				logDebug(oss.str());
				return INVALID_INDEX;
			}
			return textureId;
		}
	}
	//Texture was never loaded
	if (FileExists(filename))
	{
		textures_[textureId] = Texture{};
		auto& texture = textures_[textureId];
		auto& sfTexture = texture.texture;
		if (!sfTexture.loadFromFile(filename))
		{
			std::ostringstream oss;
			oss << "[ERROR] Could not load texture file: " << filename;
			logDebug(oss.str());
			return textureId;
		}
		//TODO Read meta file or create one
		const std::string metaPath = filename + textureMetadataExtension;
		LoadMetadata(metaPath, texture);
		textureNames_[textureId] = filename;
		return textureId;
	}
	else
	{
		std::ostringstream oss;
		oss << "[ERROR] Could not load texture file: " << filename;
		logDebug(oss.str());
	}
	return INVALID_TEXTURE_ID;
}

const Texture* TextureManager::GetTexture(const TextureId textureId) const
{
	const auto textureMapKey = textures_.find(textureId);
	if (textureMapKey != textures_.end())
	{
		return &textureMapKey->second; // Same as return nullptr.
	}
	return nullptr;
}

std::string TextureManager::GetTexturePath(TextureId textureId) const
{
	const auto keyTextureName = textureNames_.find(textureId);
	if(keyTextureName == textureNames_.end())
	{
		std::ostringstream oss;
		oss << "[Error] No texture path for id: " << textureId;
		logDebug(oss.str());
		return "";
	}
	return keyTextureName->second;
}

const std::unordered_map<TextureId, std::string>& TextureManager::GetTextureNameMap()
{
	return textureNames_;
}

void TextureManager::LoadMetadata(const std::string& metadataPath, Texture& texture)
{
	if (!FileExists(metadataPath))
	{
		CreateEmptyMetaFile(metadataPath);
	}
	else
	{
		auto metadataJson = LoadJson(metadataPath);
		texture.origin = GetVectorFromJson(metadataJson, "origin");
		texture.texture.setSmooth(metadataJson["smooth"]);

	}
}

void TextureManager::CreateEmptyMetaFile(const std::string& metaPath)
{
	//TODO Check if folder exists

	
	json metaDataJson;
	metaDataJson["origin"] = { 0,0 };
	metaDataJson["smooth"] = false;
	const std::string metaDataStr = metaDataJson.dump(4);
	WriteStringToFile(metaPath, metaDataStr);


}
} // namespace neko
