#define SPINE_SHORT_NAMES
#include <sfml_engine/spine.h>

#include "engine/log.h"
#include <graphics/graphics.h>
#include <sfml_engine/transform.h>
#include "engine/engine.h"
#include "sfml_engine/vector.h"


namespace neko::sfml
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

void BasicSpineDrawable::SetSkinByName(std::string_view skinName)
{
    if(skeletonDrawable == nullptr)
        return;
    Skeleton_setSkinByName(skeletonDrawable->skeleton, skinName.data());
    Skeleton_setSlotsToSetupPose(skeletonDrawable->skeleton);
}

void BasicSpineDrawable::SetAnimationByName(std::string_view animName)
{
    if(skeletonDrawable == nullptr)
        return;
    AnimationState_setAnimationByName(skeletonDrawable->state, 0, animName.data(), 1);
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


sf::Transform SpineBoneFollowerManager::CalculateTransformFromBone(Bone* bone)
{
	sf::Transform transform = sf::Transform::Identity;
	transform.translate(bone->x, bone->y);
	transform.scale(bone->scaleX, bone->scaleY);
	transform.rotate(bone->rotation);
	return transform;
}

void SpineBoneFollowerManager::ParseComponentJson(json& componentJson, Entity entity)
{
	auto& boneFollower = components_[entity];
	boneFollower.followingEntity = componentJson["followingEntity"];
	boneFollower.boneName = componentJson["boneName"];
	
}

json SpineBoneFollowerManager::SerializeComponentJson(Entity entity)
{
	auto& boneFollower = components_[entity];
	json componentJson;
	componentJson["followingEntity"] = boneFollower.followingEntity;
	componentJson["boneName"] = boneFollower.boneName;
	return componentJson;
}

SpineManager::SpineManager() 
{
    ResizeIfNecessary(infos_, INIT_ENTITY_NMB - 1, {});
};


void SpineManager::Update(EntityManager& entityManager, float dt)
{
    for (size_t i = 0; i < components_.size(); i++)
    {
        if (entityManager.HasComponent(neko::Entity(i), EntityMask(NekoComponentType::SPINE_ANIMATION)))
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
    auto& spineAnimation = components_[entity];
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
    spineAnimation.SetDrawable(spineAnimation.skeletonDrawable.get());
    return true;
}

void SpineManager::ParseComponentJson(json& componentJson, Entity entity)
{
    const std::string atlasFilename = componentJson["atlas"];
    const std::string skeletonDataFilename = componentJson["skeletonData"];
	const std::string spineFilename = componentJson["spine"];
    AddSpineDrawable(entity, atlasFilename, skeletonDataFilename);
	ResizeIfNecessary(infos_, entity, {});
	auto& spineInfo = infos_[entity];
	spineInfo.atlasPath = atlasFilename;
	spineInfo.skeletonDataPath = skeletonDataFilename;
	spineInfo.spinePath = spineFilename;
	
}

void SpineManager::CopyAllTransformPositions(EntityManager& entityManager, Position2dManager& position2Manager)
{
	const EntityMask entityMask = EntityMask(NekoComponentType::POSITION2D) | EntityMask(NekoComponentType::SPINE_ANIMATION);

    for (Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.HasComponent(entity, entityMask))
        {
            const auto& pos = position2Manager.GetComponent(entity);
            components_[entity].SetPosition(unit2pixel(pos));
        }
    }
}

void SpineManager::CopyAllTransformScales(EntityManager& entityManager, Scale2dManager& scale2DManager)
{
	const EntityMask entityMask = EntityMask(NekoComponentType::SCALE2D) |
            EntityMask(NekoComponentType::SPINE_ANIMATION);

    for (Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.HasComponent(entity, entityMask))
        {
            const auto& scale = scale2DManager.GetComponent(entity);
            components_[entity].transform = components_[entity].transform.scale(
                    sf::Vector2f(scale),
                    components_[entity].GetPosition());
        }
    }
}

void SpineManager::CopyAllTransformAngles(EntityManager& entityManager, Rotation2dManager& angle2DManager)
{
	const EntityMask entityMask = EntityMask(NekoComponentType::ROTATION2D) |
                            EntityMask(NekoComponentType::SPINE_ANIMATION);

    for (Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.HasComponent(entity, entityMask))
        {
            const auto& angle = angle2DManager.GetComponent(entity);
            components_[entity].transform = components_[entity].transform.rotate(angle, sf::Vector2f(components_[entity].GetPosition()));
        }
    }
}


void SpineManager::PushAllCommands(EntityManager& entityManager, GraphicsManager& graphicsManager)
{
	const auto entityMask = EntityMask(NekoComponentType::SPINE_ANIMATION);

    for (Entity entity = 0; entity < Entity(entityManager.GetEntitiesSize()); entity++)
    {
        if (entityManager.HasComponent(entity, entityMask))
        {
            if (components_[entity].skeletonDrawable != nullptr)
            {
                sf::RenderStates states(components_[entity].transform);
                components_[entity].SetStates(states);
                graphicsManager.Render(&components_[entity]);
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
	componentJson["spine"] = componentInfo.spinePath;
    return componentJson;
}

SpineDrawableInfo& SpineManager::GetInfo(Entity entity)
{
    return infos_[entity];
}

void SpineManager::CopyAllTransforms(EntityManager& entityManager, Transform2dManager& transformManager)
{
    const auto entityMask = EntityMask(NekoComponentType::TRANSFORM2D) | EntityMask(NekoComponentType::SPINE_ANIMATION);
    for(Entity entity = 0; entity < Entity(entityManager.GetEntitiesSize()); entity++)
    {
        if(entityManager.HasComponent(entity, entityMask))
        {
            const auto transform = transformManager.CalculateTransform(entity);
            components_[entity].transform = transform;
        }
    }
}

void SpineManager::DestroyComponent(EntityManager& entityManager, Entity entity)
{
    ComponentManager::DestroyComponent(entityManager, entity);
    components_[entity] = BasicSpineDrawable();
}

void SpineManager::CopyLayer(int layer, size_t start, size_t length)
{
    for (auto i = start; i < start + length; i++)
    {
        components_[i].layer = layer;
    }
}

void SpineManager::SetAnimationByName(Entity entity, const std::string_view animName)
{
    components_[entity].SetAnimationByName(animName);
}

void SpineManager::SetSkinByName(Entity entity, const std::string_view skinName)
{
    components_[entity].SetSkinByName(skinName);
}

}
