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
#include "sfml_engine/sprite.h"
#include <engine/transform.h>
#include <graphics/graphics.h>
#include <engine/engine.h>
#include "engine/globals.h"
#include <Remotery.h>
#include <sfml_engine/graphics.h>
#include "sfml_engine/texture.h"
#include "sfml_engine/json_utility.h"
#include "sfml_engine/vector.h"

namespace neko::sfml
{

void SpriteManager::CopyTexture(const TextureId textureId, size_t start, size_t length)
{

    auto* texture = textureManager_.GetTexture(textureId);
    for (auto i = start; i < start + length; i++)
    {
        components_[i].textureId = textureId;
        if(textureId != INVALID_INDEX)
        {
            components_[i].sprite.setOrigin(sf::Vector2f(texture->origin *
            Vec2f(sf::Vector2f(texture->texture.getSize()))));
            components_[i].sprite.setTexture( texture->texture, true);
        }
        else
        {
            components_[i].sprite.setTexture( sf::Texture(), true);
        }

    }
}

void SpriteManager::CopyLayer(int layer, size_t start, size_t length)
{
    for (auto i = start; i < start + length; i++)
    {
        components_[i].layer = layer;
    }
}

SpriteManager::SpriteManager(TextureManager& textureManager) :
        ComponentManager::ComponentManager(),
        textureManager_(textureManager)
{

}

void SpriteManager::CopyAllTransformPositions(EntityManager& entityManager, Position2dManager& positionManager)
{
    const auto entityMask = EntityMask(NekoComponentType::POSITION2D) | EntityMask(NekoComponentType::SPRITE2D);
    for(Entity entity = 0; entity < entityManager.GetEntitiesSize();entity++)
    {
        if(entityManager.HasComponent(entity, entityMask) and
        !entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::TRANSFORM2D)))
        {
            components_[entity].sprite.setPosition(unit2pixel(positionManager.GetComponent(entity)));
        }
    }
}

void SpriteManager::CopyAllTransformScales(EntityManager& entityManager, Scale2dManager& scaleManager)
{
    const auto entityMask = EntityMask(NekoComponentType::SCALE2D) | EntityMask(NekoComponentType::SPRITE2D);
    for(Entity entity = 0; entity < entityManager.GetEntitiesSize();entity++)
    {
        if(entityManager.HasComponent(entity, entityMask)and
           !entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::TRANSFORM2D)))
        {
            components_[entity].sprite.setScale(sf::Vector2f(scaleManager.GetComponent(entity)));
        }
    }
}

void SpriteManager::CopyAllTransformRotations(EntityManager& entityManager, Rotation2dManager& angleManager)
{
    const auto entityMask = EntityMask(NekoComponentType::ROTATION2D) | EntityMask(NekoComponentType::SPRITE2D);
    for(Entity entity = 0; entity < entityManager.GetEntitiesSize();entity++)
    {
        if(entityManager.HasComponent(entity, entityMask)and
           !entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::TRANSFORM2D)))
        {
            components_[entity].sprite.setRotation(angleManager.GetComponent(entity));
        }
    }
}

void SpriteManager::PushAllCommands(EntityManager& entityManager, GraphicsManager& graphicsManager)
{
    const auto entityMask = EntityMask(NekoComponentType::SPRITE2D);
    for(Entity entity = 0; entity < entityManager.GetEntitiesSize();entity++)
    {
        if(entityManager.HasComponent(entity, entityMask))
        {
            sf::RenderStates states = sf::RenderStates::Default;
            states.transform = components_[entity].transform;
            components_[entity].SetStates(states);
            components_[entity].SetDrawable(&components_[entity].sprite);
            graphicsManager.Render(&components_[entity]);
        }
    }
}

void SpriteManager::ParseComponentJson(json& componentJson, Entity entity)
{
    std::string textureName = componentJson["texture"];
    const auto textureId = textureManager_.LoadTexture(textureName);
    CopyTexture(textureId, entity, 1);
    CopyLayer(componentJson["layer"], entity, 1);
}

json SpriteManager::SerializeComponentJson(Entity entity)
{
    const auto& sprite = GetComponent(entity);
    json componentJson;
    componentJson["layer"] = sprite.layer;
    componentJson["textureId"] = sprite.textureId;
    componentJson["texture"] = textureManager_.GetTexturePath(sprite.textureId);
    return componentJson;
}

void SpriteManager::CopyAllTransforms(EntityManager& entityManager, Transform2dManager& transformManager)
{
    const auto entityMask = EntityMask(NekoComponentType::TRANSFORM2D) | EntityMask(NekoComponentType::SPRITE2D);
    for(Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if(entityManager.HasComponent(entity, entityMask))
        {
            const auto transform = transformManager.CalculateTransform(entity);
            components_[entity].transform = transform;
        }
    }
}

}
