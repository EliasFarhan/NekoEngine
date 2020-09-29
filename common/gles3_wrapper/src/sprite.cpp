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

#include "gl/sprite.h"
#include "engine/transform.h"
#include "graphics/camera.h"

namespace neko::gl
{


void SpriteManager::Init()
{
}

void SpriteManager::Update(seconds dt)
{
    //Update sprite if textureName is INVALID
    for(Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        if(entityManager_.HasComponent(entity, static_cast<EntityMask>(ComponentType::SPRITE2D)))
        {
            auto& sprite = components_[entity];
            if(sprite.textureId != INVALID_TEXTURE_ID && sprite.texture == INVALID_TEXTURE_NAME)
            {
                sprite.texture = textureManager_.GetTexture(sprite.textureId).name;
            }
        }
    }
}

void SpriteManager::Destroy()
{
}

void SpriteManager::Render()
{
	spriteShader_.Bind();
	const auto& camera = CameraLocator::get();
	spriteShader_.SetMat4("view", camera.GenerateViewMatrix());
	spriteShader_.SetMat4("projection", camera.GenerateProjectionMatrix());
	for(Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
	{
		if(entityManager_.HasComponent(entity, static_cast<EntityMask>(ComponentType::SPRITE2D)))
		{
			const auto& transform = transformManager_.GetComponent(entity);
			const auto& sprite = GetComponent(entity);
            spriteShader_.SetMat4("model", transform);
            spriteShader_.SetTexture("spriteTexture", sprite.texture);
            spriteQuad_.Draw();
		}
	}
}
}
