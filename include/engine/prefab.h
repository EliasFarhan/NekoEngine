#pragma once
#include <vector>
#include <utilities/json_utility.h>

namespace neko
{
class SceneManager;
class PrefabManager
{
public:
    explicit PrefabManager(SceneManager& sceneManager);

    void LoadPrefab(std::string_view prefabPath);
protected:
    std::vector<std::string> prefabPaths;
    std::vector<json> prefabJson;
    SceneManager& sceneManager_;
};
}