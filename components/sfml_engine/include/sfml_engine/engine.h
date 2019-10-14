#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "engine/engine.h"
#include "sfml_engine/input.h"
namespace sf
{
class Event;
}
namespace neko::sfml
{
class SfmlBasicEngine : public BasicEngine
{
public:
    using BasicEngine::BasicEngine;

    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

    void OnEvent(sf::Event& event);
protected:
    std::unique_ptr<sf::RenderWindow> window_ = nullptr;
    MouseManager mouseManager_;
    KeyboardManager keyboardManager_;
};
}