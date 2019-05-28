#pragma once
#include "system.h"
#include "SFML/Window/Keyboard.hpp"
#include <array>
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Window.hpp"

namespace neko
{
struct KeyPressedStatus { bool previousKeyPressed; bool keyPressed; };

class KeyboardManager : public System
{
public:
	void Init() override{}
	void Update() override;
	void Destroy() override{}
	bool IsKeyHeld(sf::Keyboard::Key key) const;
	bool IsKeyDown(sf::Keyboard::Key key) const;
	bool IsKeyUp(sf::Keyboard::Key key) const;

private:
	std::array<KeyPressedStatus, sf::Keyboard::Key::KeyCount> keyPressedStatusArray = {};
};

namespace MouseManager
{
	sf::Vector2i GetPosition();
	sf::Vector2i GetLocalPosition(sf::Window& window);
	float GetWheelDelta();
}
}
