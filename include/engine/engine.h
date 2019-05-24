#pragma once

#include <thread>
#include <engine/system.h>
#include <graphics/graphics.h>
#include <ctpl_stl.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace neko
{

class Engine : sf::NonCopyable
{
public:
    Engine();
    ~Engine();
    void Init();
    void EngineLoop();

    std::atomic<bool> isRunning = false;
    sf::RenderWindow* renderWindow;
    static Engine *GetInstance();
private:

    ctpl::thread_pool workingThreadPool;
    std::thread renderThread;
    GraphicsManager graphicsManager;
    std::condition_variable condSyncRender;
    std::mutex renderMutex;
    static Engine *instance;
};
}
