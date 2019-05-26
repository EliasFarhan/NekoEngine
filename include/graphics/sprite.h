#pragma once
#include <vector>
#include <SFML/Graphics/Sprite.hpp>


namespace neko
{
class Transform2dManager;
class GraphicsManager;

class SpriteManager
{
public:
	SpriteManager();
	void CopyTransformPosition(Transform2dManager& transformManager);
	void CopyTransformScales(Transform2dManager& transformManager);
	void CopyTransformAngles(Transform2dManager& transformManager);
	void PushCommands(GraphicsManager* graphicsManager);
	std::vector<sf::Sprite> sprites[2];
};
}
