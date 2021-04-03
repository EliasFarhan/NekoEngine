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
#include "engine/engine.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko::gl
{

void SpriteManagerInterface::SetTexture(neko::Entity entity, TextureId textureId)
{
    auto& sprite = components_[entity];
    sprite.textureId = textureId;

}

void SpriteManagerInterface::Update([[maybe_unused]]neko::seconds dt)
{
    //Update sprite if textureName is INVALID
    for(Entity entity = 0; entity < entityManager_.get().GetEntitiesSize(); entity++)
    {
        if(entityManager_.get().HasComponent(entity, static_cast<EntityMask>(ComponentType::SPRITE2D)))
        {
            auto& sprite = components_[entity];
            if(sprite.textureId != INVALID_TEXTURE_ID && sprite.texture.name == INVALID_TEXTURE_NAME)
            {
                const auto* texture = textureManager_.GetTexture(sprite.textureId);
                if(texture)
                    sprite.texture = *texture;
            }
        }
    }
}

void SpriteManager::Init()
{
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    spriteShader_.LoadFromFile(config.dataRootPath + "shaders/engine/sprite.vert",
        config.dataRootPath + "shaders/engine/sprite.frag");
    spriteQuad_.Init();
}

void SpriteManager::Destroy()
{
    for (auto& sprite : components_)
    {
        sprite.textureId = INVALID_TEXTURE_ID;
        sprite.texture.name = INVALID_TEXTURE_NAME;
    }
    spriteQuad_.Destroy();
    spriteShader_.Destroy();
}

void SpriteManager::Render()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Render Sprite Manager");
#endif
    //TODO batch sprite with the same texture together
    spriteShader_.Bind();
    const auto& camera = CameraLocator::get();
    spriteShader_.SetMat4("view", camera.GenerateViewMatrix());
    spriteShader_.SetMat4("projection", camera.GenerateProjectionMatrix());
    for (Entity entity = 0; entity < entityManager_.get().GetEntitiesSize(); entity++)
    {
        if (entityManager_.get().HasComponent(entity,EntityMask(ComponentType::SPRITE2D)))
        {
            if (!entityManager_.get().HasComponent(entity, EntityMask(ComponentType::TRANSFORM2D)))
            {
                spriteShader_.SetMat4("model", Mat4f::Identity);
            }
            else
            {
                const auto& transform = transformManager_.GetComponent(entity);
                spriteShader_.SetMat4("model", transform);
            }
            const auto& sprite = GetComponent(entity);
            spriteShader_.SetTexture("spriteTexture", sprite.texture.name);
            spriteShader_.SetVec4("spriteColor", sprite.color);
            spriteQuad_.Draw();
        }
    }
}

SpriteManager::SpriteManager(EntityManager& entityManager, TextureManagerInterface& textureManager,
                             Transform2dManager& transformManager) :
    SpriteManagerInterface(entityManager, textureManager,transformManager)
{

}
}


