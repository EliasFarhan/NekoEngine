#include <engine/engine.h>
#include <SFML/Window/Event.hpp>
#ifdef __linux__
#include <X11/Xlib.h>
#endif
#include "SFML/Graphics/RenderTexture.hpp"
#include "engine/log.h"
#include <sstream>

namespace neko
{

MainEngine * MainEngine::instance = nullptr;



void MainEngine::EngineLoop()
{

	isRunning = true;
	renderThread = std::thread(&GraphicsManager::RenderLoop, graphicsManager);
	renderThread.detach();
	while (isRunning)
	{
		if(frameIndex > 0)
		{
			std::unique_lock<std::mutex> lock(renderMutex);
			condSyncRender.wait(lock);
		}
		Update();

		frameIndex++;
	}

	Destroy();
}

MainEngine::MainEngine()
{
	initLog();
}

MainEngine::~MainEngine()
{
	destroyLog();
}

void MainEngine::Init()
{
	workingThreadPool.resize(std::max(1u,std::thread::hardware_concurrency() - 3));//removing main and render and audio thread
#ifdef __linux__
    XInitThreads();
#endif
	renderWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Neko Engine");
	renderWindow->setVerticalSyncEnabled(true);
	renderWindow->setActive(false);
	instance = this;

	graphicsManager = new GraphicsManager();
	
	
}

void MainEngine::Update()
{
	sf::Event event{};
	while (renderWindow->pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed)
		{
			isRunning = false;
		}
		OnEvent(event);
	}

}

void MainEngine::Destroy()
{
	renderWindow->close();
	delete renderWindow;
	renderWindow = nullptr;
}

void MainEngine::OnEvent(sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased)
	{
		if(event.key.code == sf::Keyboard::Escape)
		{
			isRunning = false;
		}

	}
}

MainEngine* MainEngine::GetInstance()
{
	return instance;
}

}
