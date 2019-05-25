#include <engine/engine.h>
#include <SFML/Window/Event.hpp>
#ifdef __linux__
#include <X11/Xlib.h>
#endif
#include "SFML/Graphics/RenderTexture.hpp"

namespace neko
{


MainEngine * MainEngine::instance = nullptr;

Engine::Engine()
{

}

Engine::~Engine()
{
}

void Engine::Init()
{
	graphicsManager = new GraphicsManager();
	if (renderTarget == nullptr)
	{
		auto* renderTexture = new sf::RenderTexture();
		renderTexture->create(renderTargetSize.x, renderTargetSize.y);
		renderTarget = renderTexture;
	}
	renderThread = std::thread(&GraphicsManager::RenderLoop, graphicsManager, this, std::ref(condSyncRender), std::ref(renderMutex));
	renderThread.detach();
}


void Engine::EngineLoop()
{
	isRunning = true;
    while(!graphicsManager->isReady)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }
	while (isRunning)
	{
		Update();
		condSyncRender.notify_all();
	}

	Destroy();
	delete(renderTarget);
	renderTarget = nullptr;
}

void Engine::OnEvent(sf::Event& event)
{
}

MainEngine::MainEngine()
{
}

MainEngine::~MainEngine()
{
}

void MainEngine::Init()
{
	workingThreadPool.resize(std::thread::hardware_concurrency() - 2);//removing main and render thread
#ifdef __linux__
    XInitThreads();
#endif
	renderWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Neko Engine");
	renderTarget = renderWindow;
	renderTarget->setActive(false);
	MainEngine::instance = this;

	Engine::Init();
}

void MainEngine::Update()
{
	sf::Event event;
	while (renderWindow->pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed)
		{
			isRunning = false;
		}
		OnEvent(event);
	}
	frameIndex++;
}

void MainEngine::Destroy()
{
	renderWindow->close();
	renderWindow = nullptr;

}

void MainEngine::OnEvent(sf::Event& event)
{
}

MainEngine* MainEngine::GetInstance()
{
	return instance;
}

}
