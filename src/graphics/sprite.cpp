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
SpriteManager::SpriteManager()
{
    sprites[0].reserve(InitEntityNmb);
    sprites[1].reserve(InitEntityNmb);
}

int SpriteManager::AddSprite(const sf::Texture* texture)
{
    sf::Sprite sprite;
    if (texture != nullptr)
    {
        sprite.setTexture(*texture);
        sprite.setOrigin(sf::Vector2f(texture->getSize()) / 2.0f);
    }
    const int index = sprites[0].size();
    sprites[0].push_back(sprite);
    sprites[1].push_back(sprite);
    return index;
}

void SpriteManager::CopyTransformPosition(Transform2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpritePositions, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites[frameIndex][i].setPosition(transformManager.positions[i]);
    }
}

void SpriteManager::CopyTransformScales(Transform2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpriteScales, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites[frameIndex][i].setScale(transformManager.scales[i]);
    }
}

void SpriteManager::CopyTransformAngles(Transform2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpriteAngles, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites[frameIndex][i].setRotation(transformManager.angles[i]);
    }
}

void SpriteManager::PushCommands(GraphicsManager* graphicsManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(PushSpriteCommands, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        graphicsManager->Draw(sprites[frameIndex][i]);
    }
}

sf::Sprite* SpriteManager::GetSpriteAt(unsigned int spriteIndex)
{
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    if (spriteIndex >= sprites[frameIndex].size())
    {
        return nullptr;
    }
    else
    {
        return &sprites[frameIndex][spriteIndex];
    }
}


}
