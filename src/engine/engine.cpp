/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <engine/engine.h>
#include <SFML/Window/Event.hpp>

#ifdef __linux__

#include <X11/Xlib.h>

#endif //__linux__

#include <engine/log.h>
#include <sstream>
#include <Remotery.h>
#include "imgui.h"
#include "imgui-SFML.h"

namespace neko
{

MainEngine* MainEngine::instance = nullptr;


void MainEngine::EngineLoop()
{

    isRunning = true;
    renderThread = std::thread(&GraphicsManager::RenderLoop, graphicsManager);
    renderThread.detach();
    while (isRunning)
    {

        rmt_ScopedCPUSample(EngineLoop, 0);
        dt = engineClock.restart();
        isReady = true;
        if (frameIndex > 0)
        {

            rmt_ScopedCPUSample(WaitForGraphics, 0);
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
    if (config != nullptr)
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
    if (config.vSync)
    {
        renderWindow->setVerticalSyncEnabled(config.vSync);
    }
    else
    {
        renderWindow->setFramerateLimit(config.framerateLimit);
    }
    ImGui::SFML::Init(*renderWindow);
    renderWindow->setActive(false);
    instance = this;

    graphicsManager = new GraphicsManager();


}

void MainEngine::Update()
{
    rmt_ScopedCPUSample(EngineUpdate, 0);
    sf::Event event{};
    while (renderWindow->pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
        {
            isRunning = false;
        }
        OnEvent(event);
        ImGui::SFML::ProcessEvent(event);
    }

}

void MainEngine::Destroy()
{
    std::unique_lock<std::mutex> lock(renderMutex);
    condSyncRender.wait(lock); //waiting for render thread to finish
    renderWindow->setActive(true);

    renderWindow->close();
    ImGui::SFML::Shutdown();
    delete renderWindow;
    renderWindow = nullptr;
    instance = nullptr;
    delete graphicsManager;
    graphicsManager = nullptr;
}

void MainEngine::OnEvent(sf::Event& event)
{
    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::Escape)
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
