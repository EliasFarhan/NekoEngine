#include <graphics/texture.h>
#include <set>
#include <sstream>
#include "engine/log.h"
#include "engine/globals.h"
#include "utilities/file_utility.h"

namespace neko
{
static std::set<std::string> imgExtensionSet
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
	m_TexturePaths.reserve(InitEntityNmb);
	m_TextureIdsRefCounts.reserve(InitEntityNmb);
	m_Textures.reserve(InitEntityNmb);
}

bool TextureManager::HasValidExtension(std::string filename)
{
	const std::string::size_type filenameExtensionIndex = filename.find_last_of('.');
	if (filenameExtensionIndex >= filename.size())
	{
		return false;
	}

	//Check extension first
	const std::string extension = filename.substr(filenameExtensionIndex);
	return imgExtensionSet.find(extension) != imgExtensionSet.end();
}

sf::Texture* TextureManager::LoadTexture(std::string filename)
{


	if (!HasValidExtension(filename))
	{
		std::ostringstream oss;
		oss << "[ERROR] Texture path: " << filename << " has invalid extension";
		logDebug(oss.str());
		return nullptr;
	}
	int textureIndex = -1;
	for (auto i = 0u; i < m_TexturePaths.size(); i++)
	{
		if (filename == m_TexturePaths[i])
		{
			textureIndex = i;
			break;
		}
	}
	//Was or still is loaded
	if (textureIndex != -1)
	{
		//Check if the texture was destroyed
		if (m_Textures[textureIndex].getNativeHandle() != 0U)
		{
			m_TextureIdsRefCounts[textureIndex]++;
			return nullptr;
		}
		else
		{
			auto& texture = m_Textures[textureIndex];
			if (!texture.loadFromFile(filename))
			{
				std::ostringstream oss;
				oss << "[ERROR] Could not load texture file: " << filename;
				logDebug(oss.str());
				return nullptr;
			}
			m_TextureIdsRefCounts[textureIndex] = 1U;
			return &texture;
		}
	}
	//Texture was never loaded
	if (FileExists(filename))
	{
		auto newIndex = m_Textures.size();
		auto texture = sf::Texture();
		if (!texture.loadFromFile(filename))
		{
			std::ostringstream oss;
			oss << "[ERROR] Could not load texture file: " << filename;
			logDebug(oss.str());
			return nullptr;
		}
		m_Textures.push_back(texture);
		m_TexturePaths.push_back(filename);
		m_TextureIdsRefCounts.push_back(1U);

		return &m_Textures.back();
	}
	else
	{
		std::ostringstream oss;
		oss << "[ERROR] Could not load texture file: " << filename;
		logDebug(oss.str());
	}
	return nullptr;
}
}
