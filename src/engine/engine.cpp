#include <engine/engine.h>
#include <SFML/Window/Event.hpp>

#ifdef __linux__
#include <X11/Xlib.h>
#endif //__linux__

#include <engine/log.h>
#include <sstream>
#include <Remotery.h>

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
		dt = engineClock.restart();
		rmt_ScopedCPUSample(EngineLoop, 0);
		isReady = true;
		if(frameIndex > 0)
		{
			std::unique_lock<std::mutex> lock(renderMutex);
			condSyncRender.wait(lock);
		}
		isReady = false;
		Update();

		frameIndex++;
	}

	Destroy();
}

MainEngine::MainEngine(Configuration* config)
{
	if(config != nullptr)
	{
		this->config = *config;
	}
	initLog();

	rmt_CreateGlobalInstance(&rmt);
}

MainEngine::~MainEngine()
{

	logDebug("Destroy Main Engine");
	destroyLog();
	rmt_DestroyGlobalInstance(rmt);

}

void MainEngine::Init()
{
	//workingThreadPool.resize(std::max(1u,std::thread::hardware_concurrency() - 3));//removing main and render and audio thread
#ifdef __linux__
    XInitThreads();
#endif
	renderWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Neko Engine");
	if(config.vSync)
    {
	    renderWindow->setVerticalSyncEnabled(config.vSync);
    }
	else
    {
	    renderWindow->setFramerateLimit(config.framerateLimit);
    }
	renderWindow->setActive(false);
	instance = this;

	graphicsManager = new GraphicsManager();
	
	
}

void MainEngine::Update()
{
	rmt_ScopedCPUSample(EngineManageEvent, 0);
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
	std::unique_lock<std::mutex> lock(renderMutex);
	condSyncRender.wait(lock); //waiting for render thread to finish
	renderWindow->setActive(true);
	renderWindow->close();
	delete renderWindow;
	renderWindow = nullptr;
	instance = nullptr;
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
