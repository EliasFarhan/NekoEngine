#include <Splash/graphics.h>

namespace sbp
{
GraphicsManager::GraphicsManager()
{
	renderTexture_.create(screenSize.x, screenSize.y);
}

void GraphicsManager::Update()
{
	renderTexture_.clear(sf::Color::Black);
	Draw(commandBuffer_);
	renderTexture_.display();
}
}
