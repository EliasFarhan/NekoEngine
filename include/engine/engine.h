#pragma once

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

#include <thread>
#include <engine/system.h>
#include <graphics/graphics.h>
#include <ctpl_stl.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Box2D/Common/b2Math.h>
#include "input.h"
#include <SFML/Window/WindowStyle.hpp>


struct Remotery;

namespace neko
{
struct Collider;

/**
 * \brief store various Engine constant or global values
 */
struct Configuration
{
    sf::Vector2u screenSize = sf::Vector2u(1280, 720);
    b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
    float physicsTimeStep = 1.0f / 50.0f;
    int velocityIterations = 8;
    int positionIterations = 3;
    float pixelPerMeter = 100.0f;
    sf::Color bgColor = sf::Color::Black;
    bool vSync = true;
    unsigned int framerateLimit = 0u;
    int windowStyle = sf::Style::Default;
};

/**
 * \brief basic engine class with no graphics manager implementation
 */
class BasicEngine : public sf::NonCopyable, public System
{
public:
    explicit BasicEngine(Configuration* config = nullptr);

    virtual ~BasicEngine();
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

    virtual void EngineLoop();


    virtual void OnEvent(sf::Event& event);

    Configuration config;
    bool isRunning = false;
    std::unique_ptr<sf::RenderWindow> renderWindow = nullptr;
	MouseManager& GetMouseManager();
    sf::Time clockDeltatime;
    template <typename T = BasicEngine>
    static T* GetInstance(){ return dynamic_cast<T>(instance_);};
protected:
    static BasicEngine* instance_;
    sf::Clock engineClock_;
    Remotery* rmt_ = nullptr;
    KeyboardManager keyboardManager_;
    MouseManager mouseManager_;
};


}
