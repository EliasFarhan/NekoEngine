#include <engine/prefab.h>

namespace neko
{

PrefabManager::PrefabManager(SceneManager& sceneManager) : sceneManager_(sceneManager)
{

}

void PrefabManager::LoadPrefab(std::string_view prefabPath)
{
    //TODO adding prefab loading with caching
}
}