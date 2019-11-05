#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "engine/engine.h"
#include "sfml_engine/input.h"
#include "sfml_engine/graphics.h"
#include "sfml_engine/transform.h"
#include "sfml_engine/sprite.h"
#include "sfml_engine/spine.h"

namespace neko::box2d
{
struct Collider;
}
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

	virtual void OnEvent(sf::Event& event);
protected:
	std::unique_ptr<sf::RenderWindow> window_ = nullptr;
	MouseManager mouseManager_;
	KeyboardManager keyboardManager_;

	GraphicsManager graphicsManager_;
	Delegate<const box2d::Collider*, const box2d::Collider*> contactBeginDelegate_;
	Delegate<const box2d::Collider*, const box2d::Collider*> contactEndDelegate_;
};

class SfmlFullEngine : public SfmlBasicEngine
{
public:
	explicit SfmlFullEngine(Configuration* config = nullptr);
	void OnBeginContact(const box2d::Collider* collider1, const box2d::Collider* collider2);
	void OnEndContact(const box2d::Collider* collider1, const box2d::Collider* collider2);
protected:
	Position2dManager position2dManager_;
	Scale2dManager scale2dManager_;
	Rotation2dManager rotation2dManager_;
	Transform2dManager transform2dManager_;

	TextureManager textureManager_;
	SpriteManager spriteManager_;
	SpineManager spineManager_;
	
	box2d::Physics2dManager physics2dManager_;
	box2d::Body2dManager body2dManager_;
	box2d::ContactListener contactListener_;
};

}