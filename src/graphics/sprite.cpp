#include <graphics/sprite.h>
#include <engine/transform.h>
#include <graphics/graphics.h>
#include <engine/engine.h>
#include "engine/globals.h"
#include <Remotery.h>

namespace neko
{
SpriteManager::SpriteManager()
{
	sprites[0].reserve(InitEntityNmb);
	sprites[1].reserve(InitEntityNmb);
}

void SpriteManager::AddSprite(sf::Sprite sprite)
{
	sprites[0].push_back(sprite);
	sprites[1].push_back(sprite);
}

void SpriteManager::CopyTransformPosition(Transform2dManager& transformManager, size_t start, size_t length)
{
	rmt_ScopedCPUSample(CopySpritePositions, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
	for(auto i = start; i < start+length; i++)
	{
		sprites[frameIndex][i].setPosition(transformManager.positions[i]);
	}
}

void SpriteManager::CopyTransformScales(Transform2dManager& transformManager, size_t start, size_t length)
{
	rmt_ScopedCPUSample(CopySpriteScales, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
	for (auto i = start; i < start + length; i++)
	{
		sprites[frameIndex][i].setScale(transformManager.scales[i]);
	}
}

void SpriteManager::CopyTransformAngles(Transform2dManager& transformManager, size_t start, size_t length)
{
	rmt_ScopedCPUSample(CopySpriteAngles, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
	for (auto i = start; i < start + length; i++)
	{
		sprites[frameIndex][i].setRotation(transformManager.angles[i]);
	}
}

void SpriteManager::PushCommands(GraphicsManager* graphicsManager, size_t start, size_t length)
{
	rmt_ScopedCPUSample(PushSpriteCommands, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
	for (auto i = start; i < start + length; i++)
	{
		graphicsManager->Draw(sprites[frameIndex][i]);
	}
}
}
