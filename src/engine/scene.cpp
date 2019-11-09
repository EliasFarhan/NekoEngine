//
// Created by efarhan on 7/31/19.
//

#include <engine/scene.h>
#include <engine/engine_export.h>

namespace neko
{

static const std::string_view sceneExtension = ".scene";

neko::SceneManager::SceneManager(neko::EngineExport& engineExport) :
entityManager_(engineExport.entityManager)
{

}
void SceneManager::ParseSceneJson(json& sceneJson)
{
    if (CheckJsonParameter(sceneJson, "sceneName", json::value_t::string))
    {
        currentScene_.sceneName = sceneJson["sceneName"];
    }
    else
    {
        currentScene_.sceneName = "New Scene";
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