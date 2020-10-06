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

#include <graphics/sprite.h>

#include "gl/sprite.h"
#include "engine/transform.h"
#include "graphics/camera.h"

namespace neko::gl
{


void SpriteManager::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    spriteShader_.LoadFromFile(config.dataRootPath+"shaders/engine/sprite.vert",
                               config.dataRootPath+"shaders/engine/sprite.frag");
    spriteQuad_.Init();
}

void SpriteManager::Destroy()
{
    spriteQuad_.Destroy();
    spriteShader_.Destroy();
}

void SpriteManager::Render()
{
	spriteShader_.Bind();
	const auto& camera = CameraLocator::get();
	spriteShader_.SetMat4("view", camera.GenerateViewMatrix());
	spriteShader_.SetMat4("projection", camera.GenerateProjectionMatrix());
	for(Entity entity = 0; entity < entityManager_.get().GetEntitiesSize(); entity++)
	{
		if(entityManager_.get().HasComponent(entity, EntityMask (ComponentType::SPRITE2D)|EntityMask(ComponentType::TRANSFORM2D)))
		{
			const auto& transform = transformManager_.GetComponent(entity);
			const auto& sprite = GetComponent(entity);
            spriteShader_.SetMat4("model", transform);
            spriteShader_.SetTexture("spriteTexture", sprite.texture.name);
            spriteQuad_.Draw();
		}
	}
}
}


