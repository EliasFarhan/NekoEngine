#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <condition_variable>
#include <queue>
#include <atomic>

namespace neko
{

class MainEngine;

struct Command
{
    virtual void Draw(sf::RenderTarget * renderTarget) = 0;
};

struct SfmlCommand : public Command
{
	sf::Drawable* drawable;
	void Draw(sf::RenderTarget* renderTarget) override;
};

class GraphicsManager
{
public:
	std::atomic<bool> isRendering = false;
	std::atomic<bool> isReady = false;

	std::atomic<unsigned int> frameIndex = 0;
	virtual void Draw(sf::Drawable& drawable);
    virtual void RenderLoop();
protected:
	sf::RenderWindow* renderWindow = nullptr;
	std::vector<SfmlCommand> commands[2];
    std::queue<Command*> commandBuffers[2];
};
}
