//
// Created by efarhan on 7/31/19.
//

#include <engine/scene.h>

void neko::SceneManager::ParseSceneJson(json& sceneJson)
{
    if(CheckJsonParameter(sceneJson, "entities", json::value_t::array))
    {
        for(auto& entityJson : sceneJson["entities"])
        {
            ParseEntityJson(entityJson);
        }
    }
}


