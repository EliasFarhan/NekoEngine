#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <condition_variable>
#include <array>
#include <atomic>

namespace neko
{

const size_t MAX_COMMAND_NMB = 1024;
class MainEngine;

struct Command
{
    virtual void Draw(sf::RenderTarget * renderTarget) = 0;
};

struct SfmlCommand : public Command
{
	sf::Drawable* drawable = nullptr;
	void Draw(sf::RenderTarget* renderTarget) override;
};

class GraphicsManager
{
public:
	GraphicsManager();
	std::atomic<bool> isRendering = false;
	std::atomic<bool> isReady = false;

	std::atomic<unsigned int> frameIndex = 0;
	virtual void Draw(sf::Drawable& drawable);
    virtual void RenderLoop();
protected:
	sf::RenderWindow* renderWindow = nullptr;
	std::array<SfmlCommand, MAX_COMMAND_NMB> commands[2];
    std::array<Command*, MAX_COMMAND_NMB> commandBuffers[2];
    size_t renderLength = 0;
    size_t nextRenderLength = 0;
};
}
