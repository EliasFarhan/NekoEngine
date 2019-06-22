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

BasicEngine::BasicEngine(Configuration* config)
{
    if (config != nullptr)
    {
        this->config = *config;
    }
    initLog();

    rmt_CreateGlobalInstance(&rmt_);
}

BasicEngine::~BasicEngine()
{
    logDebug("Destroy Basic Engine");
    destroyLog();
    rmt_DestroyGlobalInstance(rmt_);
}

void BasicEngine::Update()
{

}

void BasicEngine::Init()
{
    renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(config.screenSize.x, config.screenSize.y),
                                                      "Neko Engine");
    if (config.vSync)
    {
        renderWindow->setVerticalSyncEnabled(config.vSync);
    }
    else
    {
        renderWindow->setFramerateLimit(config.framerateLimit);
    }
    ImGui::SFML::Init(*renderWindow);
}

void BasicEngine::Destroy()
{
    renderWindow->close();
    ImGui::SFML::Shutdown();
    renderWindow = nullptr;
}

void BasicEngine::EngineLoop()
{
    isRunning = true;
    while (isRunning)
    {

        rmt_ScopedCPUSample(EngineLoop, 0);
        dt = engineClock_.restart();

        keyboardManager_.ClearKeys();
        mouseManager_.ClearFrameData();
        sf::Event event{};
        while (renderWindow->pollEvent(event))
        {
            OnEvent(event);
            ImGui::SFML::ProcessEvent(event);
        }

        {
            rmt_ScopedCPUSample(Draw, 0);
            renderWindow->clear(config.bgColor);
            ImGui::SFML::Update(*renderWindow, dt);
            Update();
            ImGui::SFML::Render(*renderWindow);
            renderWindow->display();
        }
    }

    Destroy();
}

void BasicEngine::OnEvent(sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
        {
            isRunning = false;
            break;
        }
        case sf::Event::KeyPressed:
        {

            keyboardManager_.AddPressKey(event.key.code);
            break;
        }
        case sf::Event::KeyReleased:
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                isRunning = false;
            }
            keyboardManager_.AddReleaseKey(event.key.code);
            break;
        }
        case sf::Event::MouseWheelScrolled:
        {
            mouseManager_.OnWheelScrolled(event);
            break;
        }
        default:
        {
            break;
        }
    }
}

MainEngine* MainEngine::instance_ = nullptr;


void MainEngine::EngineLoop()
{

    isRunning = true;
    renderThread_ = std::thread(&GraphicsManager::RenderLoop, graphicsManager_.get());
    renderThread_.detach();
    while (isRunning)
    {

        rmt_ScopedCPUSample(EngineLoop, 0);
        dt = engineClock_.restart();
        if (frameIndex > 0)
        {

            rmt_ScopedCPUSample(WaitForGraphics, 0);
            std::unique_lock<std::mutex> lock(renderStartMutex);
            condSyncRender.notify_all();
        }
        keyboardManager_.ClearKeys();
        mouseManager_.ClearFrameData();
        sf::Event event{};
        while (renderWindow->pollEvent(event))
        {
            OnEvent(event);
            ImGui::SFML::ProcessEvent(event);
        }
        Update();
        if (frameIndex > 0)
        {
            //wait for the end of the rendering
            std::unique_lock<std::mutex> lock(graphicsManager_->renderingMutex);
        }
        frameIndex++;
    }

    Destroy();
}

MainEngine::MainEngine(Configuration* config) : BasicEngine(config)
{

}

MainEngine::~MainEngine()
{

    logDebug("Destroy Main Engine");


}

void MainEngine::Init()
{
    //workingThreadPool.resize(std::max(1u,std::thread::hardware_concurrency() - 3));//removing main and render and audio thread
#ifdef __linux__
    XInitThreads();
#endif
    BasicEngine::Init();
    renderWindow->setActive(false);
    instance_ = this;

    graphicsManager_ = std::make_unique<GraphicsManager>();


}

void MainEngine::Update()
{

}

void MainEngine::Destroy()
{
    {
        std::unique_lock<std::mutex> lock(graphicsManager_->renderingMutex);
    }
    renderWindow->setActive(true);

    BasicEngine::Destroy();
    instance_ = nullptr;
    graphicsManager_ = nullptr;
}

MainEngine* MainEngine::GetInstance()
{
    return instance_;
}


}
