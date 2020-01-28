#include <engine/prefab.h>

#include <engine/scene.h>

namespace neko
{
static const std::string_view prefabExtension = ".prefab";
PrefabManager::PrefabManager(EntityManager& entityManager, SceneManager& sceneManager) : ComponentManager(entityManager), sceneManager_(sceneManager)
{

}

PrefabId PrefabManager::LoadPrefab(const std::string_view prefabPath, bool forceReload)
{
	for(auto& prefabPair : prefabMap_)
	{
		if(prefabPair.second.prefabPath == prefabPath)
		{
			if(forceReload)
			{
				prefabPair.second.prefabJson = LoadJson(prefabPath);
			}
			return prefabPair.first;
		}
	}
	Prefab newPrefab;
	newPrefab.prefabPath = prefabPath;
	newPrefab.prefabJson = LoadJson(prefabPath);
	prefabMap_[newPrefab.id] = newPrefab;
	return newPrefab.id;
}



void PrefabManager::InstantiatePrefab(PrefabId prefabIndex, EntityManager& entityManager)
{
    auto prefabJson = prefabMap_[prefabIndex].prefabJson;
    const auto entityBase = entityManager.GetLastEntity() + 1;
    for (auto& entityJson: prefabJson["entities"])
    {
	    const Entity entity = entityJson["entity"];
        entityJson["entity"] = entity + entityBase;
	    const int parent = entityJson["parent"];
        if (parent != -1)
        {
            entityJson["parent"] = Entity(parent) + entityBase;
        }
        sceneManager_.ParseEntityJson(entityJson);
    }
	
}

std::string_view PrefabManager::GetExtension()
{
    return prefabExtension;
}

const Prefab& PrefabManager::GetPrefab(PrefabId prefabId)
{
    return prefabMap_[prefabId];
}

Prefab::Prefab() : id(sole::uuid0())
{

}
}