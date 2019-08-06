#pragma once

#include <engine/engine.h>
#include <engine/transform.h>
#include <engine/scene.h>
#include "entity_viewer.h"
#include "scene_viewer.h"
#include "inspector.h"
#include "editor_scene.h"
#include "log_viewer.h"
#include "previewer.h"

#include <imgui.h>
#include <imfilebrowser.h>
#include <graphics/sprite.h>

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
    NekoEditor();
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;
    neko::EntityManager& GetEntityManager();
    neko::Position2dManager& GetPositionManager();
    neko::Scale2dManager& GetScaleManager();
    neko::Rotation2dManager& GetRotationManager();
    neko::SceneManager& GetSceneManager();
    neko::BodyDef2dManager& GetBodyDefManager();
    neko::SpriteManager& GetSpriteManager();
    neko::TextureManager& GetTextureManager();
    neko::SpineManager& GetSpineManager();
    neko::Transform2dManager& GetTransformManager();

    Previewer& GetPreviewer();
private:
    neko::EntityManager entityManager_;
    EntityViewer entityViewer_;
    neko::TextureManager textureManager_;

    neko::SpineManager spineManager_;
    neko::SpriteManager spriteManager_;

    neko::GraphicsManager graphicsManager_;
    sf::RenderTexture sceneRenderTexture_;
    SceneViewer sceneViewer_;

    EditorSceneManager sceneManager_;

    neko::BodyDef2dManager bodyDefManager_;
    Inspector inspector_;
    LogViewer logViewer_;
    Previewer previewer_;


    FileOperation fileOperationStatus_ = FileOperation::OPEN;

    neko::Transform2dManager transformManager_;
};

}