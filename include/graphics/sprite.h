#pragma once
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include "engine/globals.h"


namespace neko
{
class Transform2dManager;
class GraphicsManager;

class SpriteManager
{
public:
	SpriteManager();
	void AddSprite(const sf::Texture& texture);
	void CopyTransformPosition(Transform2dManager& transformManager, size_t start=0, size_t length=InitEntityNmb);
	void CopyTransformScales(Transform2dManager& transformManager, size_t start = 0, size_t length = InitEntityNmb);
	void CopyTransformAngles(Transform2dManager& transformManager, size_t start = 0, size_t length = InitEntityNmb);
	void PushCommands(GraphicsManager* graphicsManager, size_t start = 0, size_t length = InitEntityNmb);
private:
	std::vector<sf::Sprite> sprites[2];
};
}
