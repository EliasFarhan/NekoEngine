#include <engine/log.h>
#include "sfml_engine/scene.h"

#include "sfml_engine/transform.h"
#include "sfml_engine/sprite.h"
#include "sfml_engine/spine.h"
#include "sfml_engine/physics.h"

namespace neko::sfml
{
SfmlBasicSceneManager::SfmlBasicSceneManager(SfmlBasicEngineExport& engineExport) :
	SceneManager(engineExport),
	transformManager_(engineExport.transform2dManager),
	spineManager_(engineExport.spineManager),
	spineBoneFollowerManager_(engineExport.spineBoneFollowerManager),
	spriteManager_(engineExport.spriteManager),
	position2dManager_(engineExport.position2dManager),
	scale2dManager_(engineExport.scale2dManager),
	rotation2dManager_(engineExport.rotation2dManager)
{
	componentParsingFuncMap_[NekoComponentType::POSITION2D] =
		[this](Entity entity, json& componentJson)
	{
		auto pos = GetVectorFromJson(componentJson, "position");
		position2dManager_.AddComponent(entityManager_, entity);
		position2dManager_.SetComponent(entity, pos);
	};
	componentParsingFuncMap_[NekoComponentType::SCALE2D] =
		[this](Entity entity, json& componentJson)
	{
		auto scale = neko::GetVectorFromJson(componentJson, "scale");
		scale2dManager_.AddComponent(entityManager_, entity);
		scale2dManager_.SetComponent(entity, scale);
	};
	componentParsingFuncMap_[NekoComponentType::ROTATION2D] =
		[this](Entity entity, json& componentJson)
	{
		rotation2dManager_.AddComponent(entityManager_, entity);
		rotation2dManager_.SetComponent(entity, componentJson["angle"]);
	};
	componentParsingFuncMap_[NekoComponentType::SPRITE2D] =
		[this](Entity entity, json& componentJson)
	{
		spriteManager_.AddComponent(entityManager_, entity);
		spriteManager_.ParseComponentJson(componentJson, entity);
	};
	componentParsingFuncMap_[NekoComponentType::SPINE_ANIMATION] =
		[this](Entity entity, json& componentJson)
	{
		spineManager_.AddComponent(entityManager_, entity);
		spineManager_.ParseComponentJson(componentJson, entity);
	};
	componentParsingFuncMap_[NekoComponentType::SPINE_FOLLOW_BONE] =
		[this](Entity entity, json& componentJson)
	{
		spineBoneFollowerManager_.AddComponent(entityManager_, entity);
		spineBoneFollowerManager_.ParseComponentJson(componentJson, entity);
		//TODO check if the hierarchy is in a prefab

		//Link bone

        auto tmpBoneFollower = spineBoneFollowerManager_.GetComponent(entity);
		const auto& spineAnimation = spineManager_.GetComponent(tmpBoneFollower.followingEntity);
		if(spineAnimation.skeletonDrawable == nullptr or spineAnimation.skeletonData == nullptr)
        {
		    std::ostringstream oss;
		    oss << "[Error] Could not link Spine Animation from entity " << tmpBoneFollower.followingEntity<<
		    " through Spine Bone Follower on entity "<<entity;
		    logDebug(oss.str());
		    return;
        }
		for(int i = 0; i < spineAnimation.skeletonData->bonesCount;i++)
		{
			if(tmpBoneFollower.boneName == spineAnimation.skeletonData->bones[i]->name)
			{
				tmpBoneFollower.followingBone = spineAnimation.skeletonDrawable->skeleton->bones[i];
				break;
			}
		}
		spineBoneFollowerManager_.SetComponent(entity, tmpBoneFollower);
		
	};
}

void SfmlBasicSceneManager::ParseComponentJson(json& componentJson, neko::Entity entity)
{
	const NekoComponentType componentType = componentJson["component"];
	const auto parsingFunc = componentParsingFuncMap_.find(componentType);
	if (parsingFunc != componentParsingFuncMap_.end())
	{
		parsingFunc->second(entity, componentJson);
	}
}

void SfmlBasicSceneManager::ParseEntityJson(json& entityJson)
{
	neko::Entity entity = neko::INVALID_ENTITY;
	if (neko::CheckJsonExists(entityJson, "entity") and neko::IsJsonValueNumeric(entityJson["entity"]))
	{
		entity = entityJson["entity"];
	}
	if (entity == neko::INVALID_ENTITY)
	{
		logDebug("[Error] Scene loader entity with no entity nmb");
		return;
	}



	if (neko::CheckJsonNumber(entityJson, "parent"))
	{
		const int parentEntity = entityJson["parent"];

		logDebug("Parsing entity: " + std::to_string(entity) + " with parent: " + std::to_string(parentEntity));
		if (parentEntity >= 0)
		{
			transformManager_.SetTransformParent(entity, neko::Entity(parentEntity));
		}
	}
	if (neko::CheckJsonParameter(entityJson, "name", json::value_t::string))
	{
		//TODO scene entity name manager

	}
	if (neko::CheckJsonParameter(entityJson, "prefab", json::value_t::boolean))
	{
		if (entityJson["prefab"])
		{
			if (neko::CheckJsonNumber(entityJson, "prefabIndex"))
			{
				//const neko::Index prefabIndex = entityJson["prefabIndex"];
				//prefabManager_.InstantiatePrefab(prefabIndex, entityManager_);
			}
			return;
		}
	}
	entityManager_.CreateEntity(entity);
	if (neko::CheckJsonExists(entityJson, "entityNameHash") and neko::IsJsonValueNumeric(entityJson["entityNameHash"]))
	{
		const EntityHash entityHash = entityJson["entityNameHash"];
		entityManager_.SetEntityNameHash(entity, entityHash);
	}

	if (neko::CheckJsonParameter(entityJson, "components", json::value_t::array))
	{
		for (auto& componentJson : entityJson["components"])
		{
			ParseComponentJson(componentJson, entity);
		}
	}
}

SfmlFullSceneManager::SfmlFullSceneManager(SfmlFullEngineExport& engineExport) :
	SfmlBasicSceneManager(engineExport),
	bodyManager_(engineExport.body2dManager)
{
}



}