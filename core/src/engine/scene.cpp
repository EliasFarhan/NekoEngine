//
// Created by efarhan on 7/31/19.
//

#include <engine/scene.h>
#include <utilities/file_utility.h>
#include <engine/log.h>

namespace neko
{

static const std::string_view sceneExtension = ".scene";

neko::SceneManager::SceneManager(EntityManager& entityManager) :
entityManager_(entityManager)
{

}
void SceneManager::ParseSceneJson(const json& sceneJson)
{
    if (CheckJsonParameter(sceneJson, "sceneName", json::value_t::string))
    {
        currentScene_.sceneName = sceneJson["sceneName"];
    }
    else
    {
        currentScene_.sceneName = "New Scene";
    }

    if(CheckJsonParameter(sceneJson, "scenePath", json::value_t::string))
    {
        std::string scenePath = sceneJson["scenePath"];
        if(FileExists(scenePath))
        {
            currentScene_.scenePath = scenePath;
        }
        else
        {
            std::ostringstream oss;
            oss << "[Warning] Scene Path in scene: "<<currentScene_.sceneName<<" contains a bad scene path";
            LogDebug(oss.str());
        }
    }

    if(CheckJsonParameter(sceneJson, "sceneId", json::value_t::string))
    {
        SceneId sceneId = sole::rebuild(sceneJson["sceneId"]);
        currentScene_.sceneId = sceneId;
    }
    if (CheckJsonParameter(sceneJson, "scenePath", json::value_t::string))
    {
        currentScene_.scenePath = sceneJson["scenePath"];
    }
    if (CheckJsonParameter(sceneJson, "entities", json::value_t::array))
    {
        for (auto& entityJson : sceneJson["entities"])
        {
            ParseEntityJson(entityJson);
        }
    }
}

void SceneManager::SetCurrentScene(const Scene& currentScene)
{
    currentScene_ = currentScene;
}

std::string_view SceneManager::GetExtension()
{
    return sceneExtension;
}

}