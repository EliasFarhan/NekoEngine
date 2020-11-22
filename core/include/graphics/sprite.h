#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

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
#include <engine/component.h>
#include "engine/system.h"
#include "graphics/graphics.h"
#include "graphics/texture.h"
#include "graphics/color.h"

namespace neko
{
class Transform2dManager;

struct Sprite
{
    Sprite() = default;
    ~Sprite() = default;
    Color4 color = Color4(Color::white, 1.0f);
    TextureId textureId = INVALID_TEXTURE_ID;
    Texture texture{};
};

class SpriteManager :
	public ComponentManager<Sprite, EntityMask(ComponentType::SPRITE2D)>,
	public SystemInterface,
	public RenderCommandInterface
{
public:
    explicit SpriteManager(
        EntityManager& entityManager,
        TextureManagerInterface& textureManager,
        Transform2dManager& transformManager) :
    ComponentManager(entityManager),
	textureManager_(textureManager),
	transformManager_(transformManager)
	{}

	void Update(seconds dt) override;
	void SetTexture(Entity entity, TextureId textureId);
protected:
    TextureManagerInterface& textureManager_;
    Transform2dManager& transformManager_;
};
}