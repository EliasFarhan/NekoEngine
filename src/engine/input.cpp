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
KeyboardManager::KeyboardManager()
{
	keyStatusArray_.resize(sf::Keyboard::KeyCount, false);
}

void KeyboardManager::ClearKeys()
{
    rmt_ScopedCPUSample(KeyboardLoop, 0)
    pressedKeys_.clear();
    releasedKeys_.clear();
}

bool KeyboardManager::IsKeyHeld(sf::Keyboard::Key key) const
{
    return keyStatusArray_[int(key)];
}

bool KeyboardManager::IsKeyDown(sf::Keyboard::Key key) const
{
    return std::find(pressedKeys_.begin(), pressedKeys_.end(), key) != pressedKeys_.end();
}

bool KeyboardManager::IsKeyUp(sf::Keyboard::Key key) const
{
    return std::find(releasedKeys_.begin(), releasedKeys_.end(), key) != releasedKeys_.end();
}

void KeyboardManager::AddPressKey(sf::Keyboard::Key key)
{
	if(key == -1)
		return;
    keyStatusArray_[int(key)] = true;
    pressedKeys_.push_back(key);
}

void KeyboardManager::AddReleaseKey(sf::Keyboard::Key key)
{
	if (key == -1)
		return;
    keyStatusArray_[int(key)] = false;
    releasedKeys_.push_back(key);
}

sf::Vector2i MouseManager::GetPosition() const
{
    return GetLocalPosition(*window_);
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
    wheelDelta_ = e.mouseWheelScroll.delta;
}

void MouseManager::ClearFrameData()
{
    wheelDelta_ = 0.0f;

    previousMousePos_ = currentMousePos_;
    currentMousePos_ = GetPosition();
}

float MouseManager::GetWheelDelta() const
{
    return wheelDelta_;
}

sf::Vector2i MouseManager::GetMouseDelta() const
{
    return currentMousePos_ - previousMousePos_;
}

void MouseManager::SetWindow(sf::Window* window)
{
    this->window_ = window;
}
}
