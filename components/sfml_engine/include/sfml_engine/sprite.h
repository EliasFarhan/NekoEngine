#pragma once

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

#include <vector>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <engine/globals.h>
#include "engine/component.h"
#include "graphics.h"
#include <sfml_engine/transform.h>
#include <graphics/graphics.h>
#include <graphics/sprite.h>
#include "texture.h"


namespace neko::sfml
{
class TextureManager;

struct Sprite : public SfmlRenderCommand
{

    sf::Sprite sprite;
    int layer = 0;
    TextureId textureId = INVALID_TEXTURE_ID;
    sf::Transform transform = sf::Transform::Identity;
};

class SpriteManager : public neko::SpriteManager<Texture, Sprite>
{
public:
    explicit SpriteManager(EntityManager& entityManager, TextureManager& textureManager);

    void CopyTexture(const TextureId textureId, size_t start, size_t length=1);

    void CopyLayer(int layer, size_t start, size_t length=1);

    void CopyAllTransforms(EntityManager& entityManager, Transform2dManager& transformManager);
    void CopyAllTransformPositions(EntityManager& entityManager, Position2dManager& positionManager);

    void CopyAllTransformScales(EntityManager& entityManager, Scale2dManager& scaleManager);

    void CopyAllTransformRotations(EntityManager& entityManager, Rotation2dManager& angleManager);

    void PushAllCommands(EntityManager& entityManager, GraphicsManager& graphicsManager);

    void ParseComponentJson(const json& componentJson, Entity entity) override;

    json SerializeComponentJson(Entity entity) override;


protected:
    TextureManager& textureManager_;
};

}
