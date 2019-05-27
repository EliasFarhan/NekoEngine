#include <gtest/gtest.h>
#include "engine/engine.h"
#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "engine/transform.h"
#include "physics/physics.h"

class PlatformerEngine : public neko::MainEngine
{
public:
	void Init() override
	{
		MainEngine::Init();
		physicsManager.Init();
	}

	void Update() override
	{
		MainEngine::Update();
	}

	void Destroy() override
	{
		physicsManager.Destroy();
		MainEngine::Destroy();
	}

protected:
	neko::SpriteManager spriteManager;
	neko::TextureManager textureManager;
	neko::Transform2dManager transformManager;
	neko::Physics2dManager physicsManager;
};

TEST(Platformer, PlatformerBasic)
{
	PlatformerEngine engine;
	engine.Init();
	engine.EngineLoop();
}