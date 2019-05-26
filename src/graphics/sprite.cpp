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

void SpriteManager::CopyTransformPosition(Transform2dManager& transformManager)
{
	rmt_ScopedCPUSample(CopySpritePositions, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
	for(auto i = 0u; i < transformManager.positions.size();i++)
	{
		sprites[frameIndex][i].setPosition(transformManager.positions[i]);
	}
}

void SpriteManager::CopyTransformScales(Transform2dManager& transformManager)
{
	rmt_ScopedCPUSample(CopySpriteScales, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
	for (auto i = 0u; i < transformManager.positions.size(); i++)
	{
		sprites[frameIndex][i].setScale(transformManager.scales[i]);
	}
}

void SpriteManager::CopyTransformAngles(Transform2dManager& transformManager)
{
	rmt_ScopedCPUSample(CopySpriteAngles, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
	for (auto i = 0u; i < transformManager.positions.size(); i++)
	{
		sprites[frameIndex][i].setRotation(transformManager.angles[i]);
	}
}

void SpriteManager::PushCommands(GraphicsManager* graphicsManager)
{
	rmt_ScopedCPUSample(PushSpriteCommands, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
	for (auto& sprite : sprites[frameIndex])
	{
		graphicsManager->Draw(sprite);
	}
}
}
