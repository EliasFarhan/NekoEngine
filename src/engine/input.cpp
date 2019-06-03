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
#include <engine/input.h>
#include <engine/log.h>
#include <sstream>
#include "engine/engine.h"
#include "SFML/Window/Mouse.hpp"

namespace neko
{
void KeyboardManager::Update()
{
    for (int i = 0; i < sf::Keyboard::KeyCount; i++)
    {
        keyPressedStatusArray[i].previousKeyPressed = keyPressedStatusArray[i].keyPressed;
        keyPressedStatusArray[i].keyPressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
        if (IsKeyDown(static_cast<sf::Keyboard::Key>(i)))
        {
            std::ostringstream oss;
            oss << "[Input] Pressing key: " << i;
            logDebug(oss.str());
        }
    }
}

bool KeyboardManager::IsKeyHeld(sf::Keyboard::Key key) const
{
    return keyPressedStatusArray[static_cast<int>(key)].keyPressed;
}

bool KeyboardManager::IsKeyDown(sf::Keyboard::Key key) const
{
    return !keyPressedStatusArray[static_cast<int>(key)].previousKeyPressed &&
           keyPressedStatusArray[static_cast<int>(key)].keyPressed;
}

bool KeyboardManager::IsKeyUp(sf::Keyboard::Key key) const
{
    return !keyPressedStatusArray[static_cast<int>(key)].keyPressed &&
           keyPressedStatusArray[static_cast<int>(key)].previousKeyPressed;
}

namespace MouseManager
{
sf::Vector2i GetPosition()
{
    auto* engine = MainEngine::GetInstance();
    return GetLocalPosition(*engine->renderWindow);
}

sf::Vector2i GetLocalPosition(sf::Window& window)
{
    return sf::Mouse::getPosition(window);
}

float GetWheelDelta()
{
    return 0.0f;
}
}
}
