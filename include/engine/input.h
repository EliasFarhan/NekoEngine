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

#include "system.h"
#include "SFML/Window/Keyboard.hpp"
#include <array>
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Window.hpp"

namespace neko
{
struct KeyPressedStatus
{
    bool previousKeyPressed;
    bool keyPressed;
};

class KeyboardManager : public System
{
public:
    using System::System;
    void Init() override
    {
        isKeyHeld.fill(0);
    }

    void Update() override;

    void Destroy() override
    {}

    bool IsKeyHeld(sf::Keyboard::Key key) const;

    bool IsKeyDown(sf::Keyboard::Key key) const;

    bool IsKeyUp(sf::Keyboard::Key key) const;

    void AddPressKey(sf::Keyboard::Key key);
    void AddReleaseKey(sf::Keyboard::Key key);

private:
    std::vector<sf::Keyboard::Key> pressedKey;
    std::vector<sf::Keyboard::Key> releasedKey;
    std::array<bool, sf::Keyboard::KeyCount> isKeyHeld;
};

namespace MouseManager
{
sf::Vector2i GetPosition();

sf::Vector2i GetLocalPosition(sf::Window& window);

}
}
