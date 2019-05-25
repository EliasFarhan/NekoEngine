#include <engine/engine.h>
#include <SFML/Window/Event.hpp>
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

	renderWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Neko Engine");
	renderTarget = renderWindow;
	renderTarget->setActive(false);
	Engine::Init();
}

void MainEngine::Update()
{
	sf::Event event = {};
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
}

void MainEngine::OnEvent(sf::Event& event)
{
}

MainEngine* MainEngine::GetInstance()
{
	return instance;
}

}
