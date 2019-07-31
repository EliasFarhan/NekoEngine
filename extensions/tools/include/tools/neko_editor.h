#pragma once

#include <engine/engine.h>
#include <engine/transform.h>
#include <engine/scene.h>
#include "entity_viewer.h"
#include "scene_viewer.h"
#include "inspector.h"
#include "editor_scene.h"

#include <imgui.h>
#include <imfilebrowser.h>

namespace editor
{
enum class FileOperation
{
    OPEN,
    SAVE
};
class NekoEditor : public neko::BasicEngine
{
public:
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

    neko::EntityManager& GetEntityManager() { return entityManager_;}
    neko::Position2dManager& GetPositionManager(){ return positionManager_;}

private:
    neko::EntityManager entityManager_;
    EntityViewer entityViewer_;

    neko::BasicGraphicsManager graphicsManager_;
    sf::RenderTexture sceneRenderTexture_;
    SceneViewer sceneViewer_;

    EditorSceneManager sceneManager_{*this};

    neko::Position2dManager positionManager_;

    Inspector inspector_{*this};

    ImGui::FileBrowser fileDialog;

    FileOperation fileOperationStatus_ = FileOperation::OPEN;

};

}