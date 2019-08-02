#include <graphics/spine.h>
#include "engine/log.h"
#include <graphics/graphics.h>
#include <engine/transform.h>

namespace neko
{


BasicSpineDrawable::BasicSpineDrawable() : skeletonDrawable(nullptr)
{

}

BasicSpineDrawable::~BasicSpineDrawable()
{
    if (skeletonData)
        SkeletonData_dispose(skeletonData);
    if (atlas)
        Atlas_dispose(atlas);
}

void BasicSpineDrawable::SetPosition(const sf::Vector2f& position)
{
    if(skeletonDrawable)
    {
        skeletonDrawable->skeleton->x = position.x;
        skeletonDrawable->skeleton->y = position.y;
        Skeleton_updateWorldTransform(skeletonDrawable->skeleton);
    }
}

sf::Vector2f BasicSpineDrawable::GetPosition()
{
    if(!skeletonDrawable)
        return sf::Vector2f();
    return sf::Vector2f(skeletonDrawable->skeleton->x,skeletonDrawable->skeleton->y);
}


SkeletonData* readSkeletonJsonData(const char* filename, Atlas* atlas, float scale)
{
    SkeletonJson* json = SkeletonJson_create(atlas);
    json->scale = scale;
    SkeletonData* skeletonData = SkeletonJson_readSkeletonDataFile(json, filename);
    if (!skeletonData)
    {
        logDebug(json->error);
        return nullptr;
    }
    SkeletonJson_dispose(json);
    return skeletonData;
}

SkeletonData* readSkeletonBinaryData(const char* filename, Atlas* atlas, float scale)
{
    SkeletonBinary* binary = SkeletonBinary_create(atlas);
    binary->scale = scale;
    SkeletonData* skeletonData = SkeletonBinary_readSkeletonDataFile(binary, filename);
    if (!skeletonData)
    {
        logDebug(binary->error);
        return nullptr;
    }
    SkeletonBinary_dispose(binary);
    return skeletonData;
}


SpineManager::SpineManager() : ComponentManager::ComponentManager()
{
    ResizeIfNecessary(infos_, INIT_ENTITY_NMB - 1, {});
};


void SpineManager::Update(EntityManager& entityManager, float dt)
{
    for (int i = 0; i < components_.size(); i++)
    {
        if (entityManager.HasComponent(i, EntityMask(NekoComponentType::SPINE_ANIMATION)))
        {
            if (components_[i].skeletonDrawable != nullptr)
            {
                components_[i].skeletonDrawable->update(dt);
                components_[i].transform = sf::Transform();
            }
        }
    }
}


bool SpineManager::AddSpineDrawable(Entity entity,
                                    const std::string_view atlasFilename,
                                    const std::string_view skeletonFilename)
{
    auto& spineAnimation = GetComponent(entity);
    if (spineAnimation.atlas)
    {
        Atlas_dispose(spineAnimation.atlas);
        spineAnimation.atlas = nullptr;
    }
    spineAnimation.atlas = Atlas_createFromFile(atlasFilename.data(), 0);
    if (!spineAnimation.atlas)
    {
        logDebug("[Error] Could not load spine atlas file");
        return false;
    }
    if (spineAnimation.skeletonData)
    {
        SkeletonData_dispose(spineAnimation.skeletonData);
        spineAnimation.skeletonData = nullptr;
    }
    spineAnimation.skeletonData = readSkeletonJsonData(skeletonFilename.data(),
                                                       spineAnimation.atlas, 1.0f);
    if (!spineAnimation.skeletonData)
    {
        logDebug("[Error] while trying to load spine skeleton file");
        return false;
    }
    spineAnimation.skeletonDrawable = std::make_shared<spine::SkeletonDrawable>(spineAnimation.skeletonData);
    return true;
}

void SpineManager::ParseComponentJson(json& componentJson, Entity entity)
{
    auto& spineAnimation = GetComponent(entity);
    const std::string atlasFilename = componentJson["atlas"];
    const std::string skeletonDataFilename = componentJson["skeletonData"];
    AddSpineDrawable(entity, atlasFilename, skeletonDataFilename);


}

void SpineManager::CopyAllTransformPositions(EntityManager& entityManager, Position2dManager& position2Manager)
{
    EntityMask entityMask = EntityMask(NekoComponentType::POSITION2D) | EntityMask(NekoComponentType::SPINE_ANIMATION);

    for (Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.HasComponent(entity, entityMask))
        {
            const auto& pos = position2Manager.GetConstComponent(entity);
            components_[entity].SetPosition(pos);
        }
    }
}

void SpineManager::CopyAllTransformScales(EntityManager& entityManager, Scale2dManager& scale2DManager)
{
    EntityMask entityMask = EntityMask(NekoComponentType::SCALE2D) |
            EntityMask(NekoComponentType::SPINE_ANIMATION);

    for (Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.HasComponent(entity, entityMask))
        {
            const auto& scale = scale2DManager.GetConstComponent(entity);
            components_[entity].transform = components_[entity].transform.scale(scale, components_[entity].GetPosition());
        }
    }
}

void SpineManager::CopyAllTransformAngles(EntityManager& entityManager, Angle2dManager& angle2DManager)
{
    EntityMask entityMask = EntityMask(NekoComponentType::ANGLE2D) |
            EntityMask(NekoComponentType::SPINE_ANIMATION);

    for (Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.HasComponent(entity, entityMask))
        {
            const auto& angle = angle2DManager.GetConstComponent(entity);
            components_[entity].transform = components_[entity].transform.rotate(angle, components_[entity].GetPosition());
        }
    }
}


void SpineManager::PushAllCommands(EntityManager& entityManager, GraphicsManager& graphicsManager)
{
    EntityMask entityMask = EntityMask(NekoComponentType::SPINE_ANIMATION);

    for (Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.HasComponent(entity, entityMask))
        {
            if (components_[entity].skeletonDrawable != nullptr)
            {
                sf::RenderStates states(components_[entity].transform);
                graphicsManager.Draw(*components_[entity].skeletonDrawable, components_[entity].layer, states);
            }
        }
    }
}

Index SpineManager::AddComponent(EntityManager& entityManager, Entity entity)
{
    ResizeIfNecessary(infos_, entity, {});
    return ComponentManager::AddComponent(entityManager, entity);
}

json SpineManager::SerializeComponentJson(Entity entity)
{
    json componentJson;
    const auto& componentInfo = infos_[entity];
    componentJson["atlas"] = componentInfo.atlasPath;
    componentJson["skeletonData"] = componentInfo.skeletonDataPath;
    return componentJson;
}

SpineDrawableInfo& SpineManager::GetInfo(Entity entity)
{
    return infos_[entity];
}

}
