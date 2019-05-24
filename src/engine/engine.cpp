#include <engine/engine.h>
#include <SFML/Window/Event.hpp>

namespace neko
{


Engine *Engine::instance = nullptr;

Engine::Engine()
{
    instance = this;
}

void Engine::Init()
{
    workingThreadPool.resize(std::thread::hardware_concurrency() - 2);//removing main and render thread

}
Engine *Engine::GetInstance()
{
    return instance;
}

void Engine::EngineLoop()
{

    renderWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Neko Engine");
    renderWindow->setActive(false);
    renderThread = std::thread(&GraphicsManager::RenderLoop, &graphicsManager, std::ref(condSyncRender), std::ref(renderMutex));
    renderThread.detach();
    isRunning = true;



    while (isRunning)
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event{};
        while (renderWindow->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                renderWindow->close();
                isRunning = false;
                return;
            }
        }
        {
            //TODO Push to render loop
            std::unique_lock<std::mutex> lock(renderMutex);

            condSyncRender.notify_one();
        }
    }

    delete(renderWindow);
    renderWindow = nullptr;
}

Engine::~Engine()
{
}
}
