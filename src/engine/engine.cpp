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
BasicEngine* BasicEngine::instance_ = nullptr;
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

void BasicEngine::Update([[maybe_unused]]float dt)
{

}

void BasicEngine::Init()
{
    instance_ = this;
    renderWindow = std::make_unique<sf::RenderWindow>(
		sf::VideoMode(config.realWindowSize.x, config.realWindowSize.y), "Neko Engine", config.windowStyle);
    if (config.vSync)
    {
        renderWindow->setVerticalSyncEnabled(config.vSync);
    }
    else
    {
        renderWindow->setFramerateLimit(config.framerateLimit);
    }
    ImGui::SFML::Init(*renderWindow);
    mouseManager_.SetWindow(renderWindow.get());
}

void BasicEngine::Destroy()
{
    instance_ = nullptr;
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
        clockDeltatime = engineClock_.restart();

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
            ImGui::SFML::Update(*renderWindow, clockDeltatime);
            Update(clockDeltatime.asSeconds());
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
        case sf::Event::Resized:
        {
            config.realWindowSize = sf::Vector2u(event.size.width, event.size.height);
            break;
        }
        default:
        {
            break;
        }
    }
}

MouseManager& BasicEngine::GetMouseManager()
{
	return mouseManager_;
}


}
