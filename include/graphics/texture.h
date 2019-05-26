#pragma once
#include <vector>
#include <SFML/Graphics/Texture.hpp>
namespace neko
{
class TextureManager
{
public:

	TextureManager();

	bool HasValidExtension(std::string filename);
	sf::Texture* LoadTexture(std::string filename);

	std::vector<std::string> m_TexturePaths;
	std::vector<sf::Texture> m_Textures;
	std::vector<size_t> m_TextureIdsRefCounts;


};
}