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

#include <vector>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <engine/globals.h>
#include "engine/component.h"
#include <engine/transform.h>


namespace neko
{

class GraphicsManager;

class SpriteManager
{
public:
    SpriteManager() = default;

    virtual neko::Index AddSprite(const sf::Texture* texture) = 0;

    virtual sf::Sprite* GetSpriteAt(unsigned int spriteIndex) = 0;

    virtual void CopyTransformPosition(Position2dManager& transformManager, size_t start = 0, size_t length = neko::INIT_ENTITY_NMB)= 0;

    virtual void CopyTransformScales(Scale2dManager& transformManager, size_t start = 0, size_t length = neko::INIT_ENTITY_NMB) = 0;

    virtual void CopyTransformAngles(Angle2dManager& transformManager, size_t start = 0, size_t length = neko::INIT_ENTITY_NMB) = 0;
    /**
* \brief push basic graphic command to the render thread to be processed next frame
* @param graphicsManager
* @param start
* @param length
*/
    virtual void PushCommands(neko::GraphicsManager* graphicsManager, size_t start = 0, size_t length = neko::INIT_ENTITY_NMB) = 0;
};

class BasicSpriteManager : public SpriteManager, public ComponentManager<sf::Sprite, ComponentType(NekoComponentType::SPRITE2D)>
{
public:
    using ComponentManager::ComponentManager;
    Index AddSprite(const sf::Texture* texture) override;

    sf::Sprite* GetSpriteAt(unsigned int spriteIndex) override;

    void CopySpriteOrigin(const sf::Vector2f& origin, size_t start, size_t length);
    void CopyTransformPosition(Position2dManager& positionManager, size_t start, size_t length) override;

    void CopyTransformScales(Scale2dManager& scaleManager, size_t start, size_t length) override;

    void CopyTransformAngles(Angle2dManager& angleManager, size_t start, size_t length) override;

    void PushCommands(neko::GraphicsManager* graphicsManager, size_t start, size_t length) override;

};

}
