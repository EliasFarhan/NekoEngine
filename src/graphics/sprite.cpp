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
#include <graphics/sprite.h>
#include <engine/transform.h>
#include <graphics/graphics.h>
#include <engine/engine.h>
#include "engine/globals.h"
#include <Remotery.h>
#include <SFML/Graphics/Texture.hpp>

namespace neko
{

Index BasicSpriteManager::AddSprite(const sf::Texture* texture)
{
    return 0;
}

sf::Sprite* BasicSpriteManager::GetSpriteAt(unsigned int spriteIndex)
{
    return nullptr;
}

void BasicSpriteManager::CopyTransformPosition(Position2dManager& positionManager, size_t start, size_t length)
{
	rmt_ScopedCPUSample(CopySpritePositions, 0);
	for (auto i = start; i < start + length; i++)
	{
		components_[i].sprite.setPosition(positionManager.GetComponent(neko::Entity(i)));
	}
}

void BasicSpriteManager::CopyTransformScales(Scale2dManager& scaleManager, size_t start, size_t length)
{
	rmt_ScopedCPUSample(CopySpritePositions, 0);
	for (auto i = start; i < start + length; i++)
	{
		components_[i].sprite.setScale(scaleManager.GetComponent(neko::Entity(i)));
	}
}

void BasicSpriteManager::CopyTransformAngles(Angle2dManager& angleManager, size_t start, size_t length)
{
	rmt_ScopedCPUSample(CopySpritePositions, 0);
	for (auto i = start; i < start + length; i++)
	{
		components_[i].sprite.setRotation(angleManager.GetComponent(neko::Entity(i)));
	}
}

void BasicSpriteManager::PushCommands(neko::GraphicsManager* graphicsManager, size_t start, size_t length)
{

	if (graphicsManager == nullptr) return;
	for(size_t i = start; i < start+length;i++)
	{
		graphicsManager->Draw(components_[i].sprite, components_[i].layer);
	}
}

void BasicSpriteManager::CopySpriteOrigin(const sf::Vector2f& origin, size_t start, size_t length)
{
    for(size_t i = start; i < start+length;i++)
    {
        const auto localBounds = components_[i].sprite.getLocalBounds();
        components_[i].origin = origin;
        components_[i].sprite.setOrigin(sf::Vector2f(localBounds.width*origin.x, localBounds.height*origin.y));
    }
}

void BasicSpriteManager::CopyTexture(const sf::Texture* texture, size_t start, size_t length)
{
    if(texture == nullptr)
        return;
    for(auto i = start; i < start+length;i++)
    {
        components_[i].sprite.setTexture(*texture);
    }
}

void BasicSpriteManager::CopyLayer(int layer, size_t start, size_t length)
{
    for(auto i = start; i < start+length;i++)
    {
        components_[i].layer = layer;
    }
}
}
