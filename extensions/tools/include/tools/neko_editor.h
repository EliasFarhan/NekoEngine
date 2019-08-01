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
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

    neko::EntityManager& GetEntityManager();
    neko::Position2dManager& GetPositionManager();
    neko::Scale2dManager& GetScaleManager();
    neko::Angle2dManager& GetAngleManager();
    neko::SceneManager& GetSceneManager();
    neko::BodyDef2dManager& GetBodyDefManager();
    neko::BasicSpriteManager& GetSpriteManager();
    neko::TextureManager& GetTextureManager();
private:
    neko::EntityManager entityManager_;
    EntityViewer entityViewer_;
    neko::TextureManager textureManager_;


    neko::BasicSpriteManager spriteManager_{this->textureManager_};



    neko::BasicGraphicsManager graphicsManager_;
    sf::RenderTexture sceneRenderTexture_;
    SceneViewer sceneViewer_;

    EditorSceneManager sceneManager_{*this};

    neko::Position2dManager positionManager_;
    neko::Scale2dManager scaleManager_{sf::Vector2f(1.0f,1.0f)};
    neko::Angle2dManager angleManager_;

    neko::BodyDef2dManager bodyDefManager_;
    Inspector inspector_{*this};

    FileOperation fileOperationStatus_ = FileOperation::OPEN;

};

}