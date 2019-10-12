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

#include "engine/system.h"
#include "SFML/Window/Keyboard.hpp"
#include <array>
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Window.hpp"
#include "SFML/Window/Mouse.hpp"

namespace neko
{

/**
 * \brief manages the inputs from the keyboards through the window events.
 * It is part of the MainEngine class by default
 */

class KeyboardManager 
{
public:
	KeyboardManager();
    /**
     * \brief Should be called each frame before retrieving the new window event
     * it is called by MainEngine::Update
     */
	void ClearKeys();

    bool IsKeyHeld(sf::Keyboard::Key key) const;

    bool IsKeyDown(sf::Keyboard::Key key) const;

    bool IsKeyUp(sf::Keyboard::Key key) const;

    /**
     * \brief Should be called for each PressKeyEvent
     * it is called by MainEngine::Update
     * @param key
     */
    void AddPressKey(sf::Keyboard::Key key);
    /**
     * \brief Should be called for each ReleaseKeyEvent
     * it is called by MainEngine::Update
     * @param key
     */
    void AddReleaseKey(sf::Keyboard::Key key);

private:
    /**
     * \brief list of current frame pressed key
     */
    std::vector<sf::Keyboard::Key> pressedKeys_;
    /**
     * \brief list of current frame released key
     */
    std::vector<sf::Keyboard::Key> releasedKeys_;
    /**
     * \brief store the status of each key to check if
     */
    std::vector<char> keyStatusArray_;
};

/**
 * \brief manages the position of the mouse as well as the wheel state and button press
 * It is part of the MainEngine class by default
 */
class MouseManager
{
public:
    void SetWindow(sf::Window* window);
	sf::Vector2i GetPosition() const;
	sf::Vector2i GetLocalPosition(sf::Window& window) const;
	bool IsButtonPressed(sf::Mouse::Button button) const;
	/**
	 *  \brief should be called for each WheelScrollEvent
	 *  it is called by MainEngine::Update
	 * @param e
	 */
	void OnWheelScrolled(const sf::Event& e);
	/**
	 * \brief Should be called before retrieving MainEngine::Update
	 * it is called by MainEngine::Update
	 */
	void ClearFrameData();
	float GetWheelDelta() const;
	sf::Vector2i GetMouseDelta() const;
private:
    sf::Window* window_ = nullptr;
	float wheelDelta_ = 0.0f;
	sf::Vector2i previousMousePos_ = sf::Vector2i(0, 0);
	sf::Vector2i currentMousePos_ = sf::Vector2i(0, 0);
};
}
