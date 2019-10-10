#pragma once
#include <engine/scene.h>

namespace neko
{

class BasicSceneManager : public SceneManager
{
public:
    void ParseComponentJson(json& componentJson, Entity entity) override;

    void ParseEntityJson(json& entityJson) override;

};

}