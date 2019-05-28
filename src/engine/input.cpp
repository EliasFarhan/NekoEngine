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
	return !keyPressedStatusArray[static_cast<int>(key)].previousKeyPressed && keyPressedStatusArray[static_cast<int>(key)].keyPressed;
}

bool KeyboardManager::IsKeyUp(sf::Keyboard::Key key) const
{
	return !keyPressedStatusArray[static_cast<int>(key)].keyPressed && keyPressedStatusArray[static_cast<int>(key)].previousKeyPressed;
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
