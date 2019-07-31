#pragma once

#include <engine/scene.h>
namespace editor
{
class NekoEditor;
class EditorSceneManager : public neko::SceneManager
{
public:
    explicit EditorSceneManager(NekoEditor& nekoEditor) : nekoEditor_(nekoEditor){}

    void ParseEntityJson(json& entityJson) override;
    void ParseComponentJson(json& componentJson, neko::Entity entity) override;


    virtual json SerializeEntity(neko::Entity entity);
    virtual json SerializeScene();
private:
    NekoEditor& nekoEditor_;
};

}