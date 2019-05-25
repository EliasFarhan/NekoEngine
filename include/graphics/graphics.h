#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <condition_variable>
#include <queue>
#include <atomic>

namespace neko
{

class Engine;

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
	virtual void Draw(sf::Drawable& drawable);
    virtual void RenderLoop(Engine* engine, std::condition_variable& condSyncRender, std::mutex& renderMutex);
protected:
	Engine* engine = nullptr;
	std::vector<SfmlCommand> commands[2];
    std::queue<Command*> commandBuffers[2];
	std::atomic<unsigned int> frameIndex = 0;
};
}
