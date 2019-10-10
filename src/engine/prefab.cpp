#include <engine/prefab.h>

#include <engine/scene.h>

namespace neko
{

PrefabManager::PrefabManager(SceneManager& sceneManager) : sceneManager_(sceneManager)
{

}

Index PrefabManager::LoadPrefab(std::string_view prefabPath, bool forceReload)
{
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
    }
}

void PrefabManager::ClearPrefabs()
{
	prefabPaths_.clear();
	prefabJsons_.clear();
}

const std::vector<std::string>& PrefabManager::GetConstPrefabPaths() const
{
	return prefabPaths_;
}

void PrefabManager::InstantiatePrefab(Index prefabIndex, EntityManager& entityManager)
{
    auto prefabJson = prefabJsons_[prefabIndex];
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
}