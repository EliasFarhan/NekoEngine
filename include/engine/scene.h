#pragma once

#include <utilities/json_utility.h>
#include "entity.h"

namespace neko
{

struct Scene
{
    std::string sceneName = "";
    std::string scenePath = "";
    std::vector<std::string> entitiesNames;
};

class SceneManager
{
public:
    virtual void ParseComponentJson(json& componentJson, Entity entity) = 0;
    virtual void ParseEntityJson(json& entityJson) = 0;
    virtual void ParseSceneJson(json& sceneJson);

    Scene& GetCurrentScene(){ return currentScene_;}
protected:
    Scene currentScene_;
};

}