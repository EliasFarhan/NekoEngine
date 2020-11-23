//
// Created by efarhan on 9/29/20.
//

#include "graphics/sprite.h"

namespace neko
{
void SpriteManager::SetTexture(neko::Entity entity, neko::TextureId textureId)
{
    auto& sprite = components_[entity];
    sprite.textureId = textureId;

}

void SpriteManager::Update([[maybe_unused]]neko::seconds dt)
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
}