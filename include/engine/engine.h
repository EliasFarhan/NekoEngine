#pragma once

#include <thread>
#include <engine/system.h>
#include <graphics/graphics.h>
#include <ctpl_stl.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace neko
{


class MainEngine : public sf::NonCopyable, public System
{
public:
	MainEngine();
	virtual ~MainEngine();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void EngineLoop();

	virtual void OnEvent(sf::Event& event);

	sf::RenderWindow* renderWindow = nullptr;
	static MainEngine *GetInstance();
	std::atomic<bool> isRunning = false;
	sf::Vector2u renderTargetSize;

	//used to sync with the render thread
	std::condition_variable condSyncRender;
	std::mutex renderMutex;
protected:
	ctpl::thread_pool workingThreadPool;
	unsigned frameIndex = 0;
	std::thread renderThread;
	GraphicsManager* graphicsManager;
	static MainEngine *instance;
};

}
