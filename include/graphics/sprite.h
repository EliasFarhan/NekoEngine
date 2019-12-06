#pragma once

#include <engine/component.h>
#include "graphics/texture.h"

namespace neko
{
template<class T, class S>
class SpriteManager : public ComponentManager<S, EntityMask(NekoComponentType::SPRITE2D)>
{
public:
    explicit SpriteManager(EntityManager& entityManager, TextureManager<T>& textureManager) :
            ComponentManager<S, EntityMask(NekoComponentType::SPRITE2D)>(entityManager),textureManager_(textureManager)
    {}

protected:
    TextureManager<T>& textureManager_;
};
}