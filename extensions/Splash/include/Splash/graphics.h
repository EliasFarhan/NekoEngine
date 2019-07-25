#pragma once

#include <graphics/graphics.h>
#include <SFML/Graphics/RenderTexture.hpp>

namespace sbp
{

const sf::Vector2i screenSize = sf::Vector2i(1920, 1080);

class GraphicsManager : public neko::BasicGraphicsManager
{
public:
	GraphicsManager();
	void Draw(sf::Drawable& drawable, int layer) override;
	void Draw(sf::VertexArray* vertexArray, sf::Texture* texture, int layer) override;
	void Update();
protected:
	void Draw(std::array<neko::Command*, neko::MAX_COMMAND_NMB>& commandBuffers_) override;
	sf::RenderTexture renderTexture_;
};
}