/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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

#include <sstream>
#include <engine/input.h>
#include <engine/log.h>
#include <engine/engine.h>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <Remotery.h>

namespace neko
{
void KeyboardManager::ClearKeys()
{
    rmt_ScopedCPUSample(KeyboardLoop, 0)
    pressedKey.clear();
    releasedKey.clear();
}

bool KeyboardManager::IsKeyHeld(sf::Keyboard::Key key) const
{
    return keyStatusArray[int(key)];
}

bool KeyboardManager::IsKeyDown(sf::Keyboard::Key key) const
{
    return std::find(pressedKey.begin(), pressedKey.end(), key) != pressedKey.end();
}

bool KeyboardManager::IsKeyUp(sf::Keyboard::Key key) const
{
    return std::find(releasedKey.begin(), releasedKey.end(), key) != releasedKey.end();
}

void KeyboardManager::AddPressKey(sf::Keyboard::Key key)
{
    keyStatusArray[int(key)] = true;
    pressedKey.push_back(key);
}

void KeyboardManager::AddReleaseKey(sf::Keyboard::Key key)
{
    keyStatusArray[int(key)] = false;
    releasedKey.push_back(key);
}

sf::Vector2i MouseManager::GetPosition() const
{
    auto* engine = MainEngine::GetInstance();
    return GetLocalPosition(*engine->renderWindow);
}

sf::Vector2i MouseManager::GetLocalPosition(sf::Window& window) const
{
    return sf::Mouse::getPosition(window);
}

bool MouseManager::IsButtonPressed(sf::Mouse::Button button) const
{
	return sf::Mouse::isButtonPressed(button);
}

void MouseManager::OnWheelScrolled(const sf::Event& e)
{
	wheelDelta = e.mouseWheelScroll.delta;
}

void MouseManager::ClearFrameData()
{
	wheelDelta = 0.0f;

	previousMousePos = currentMousePos;
	currentMousePos = GetPosition();
}

float MouseManager::GetWheelDelta() const
{
	return wheelDelta;
}

sf::Vector2i MouseManager::GetMouseDelta() const
{
	return currentMousePos - previousMousePos;
}
}
