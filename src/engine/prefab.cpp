#include <engine/prefab.h>

#include <engine/scene.h>

namespace neko
{
static const std::string_view prefabExtension = ".prefab";
PrefabManager::PrefabManager(SceneManager& sceneManager) : sceneManager_(sceneManager)
{

}

PrefabId PrefabManager::LoadPrefab(std::string_view prefabPath, bool forceReload)
{
	/*
    const auto prefabIt = std::find(prefabPaths_.begin(), prefabPaths_.end(), prefabPath);
    if (prefabIt != prefabPaths_.end())
    {
        const auto prefabIndex = Index(prefabIt - prefabPaths_.begin());
        if (forceReload)
        {
            const auto prefabFileJson = LoadJson(prefabPath.data());
            prefabJsons_[prefabIndex] = prefabFileJson;
        }
        return prefabIndex;
    }
    else
    {
        const auto prefabFileJson = LoadJson(prefabPath.data());
        prefabJsons_.push_back(prefabFileJson);
        prefabPaths_.push_back(prefabPath.data());
        return Index(prefabJsons_.size() - 1);
    }*/
	return INVALID_PREFAB_ID;
}

void PrefabManager::ClearPrefabs()
{
	prefabMap_.clear();
}


void PrefabManager::InstantiatePrefab(PrefabId prefabIndex, EntityManager& entityManager)
{
    /*auto prefabJson = prefabJsons_[prefabIndex];
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
	*/
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