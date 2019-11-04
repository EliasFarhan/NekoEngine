#pragma once

#include <engine/system.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include "engine_export.h"
#include <tools/neko_editor.h>

namespace neko::editor
{
class NekoEditor;
class EditorPrefabSystem : public System
{
public:
    explicit EditorPrefabSystem(NekoEditor& editor);

    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

private:
    sf::RenderTexture screenRenderTexture_;
    NekoEditorExport editorExport_;
    EntityManager entityManager_;
    Position2dManager position2dManager_;
    Scale2dManager scale2dManager_;
    Rotation2dManager rotation2dManager_;
    sfml::Transform2dManager transform2dManager_;
    EditorSceneManager sceneManager_;
    box2d::BodyDef2dManager bodyDef2dManager_;
    sfml::SpriteManager spriteManager_;
    sfml::TextureManager textureManager_;
    sfml::SpineManager spineManager_;
    BoxColliderDefManager boxColliderDefManager_;
    CircleColliderDefManager circleColliderDefManager_;
    PolygonColldierDefManager polygonColldierDefManager_;
    ColliderDefManager colliderManagerDefManager_ ;
    EditorPrefabManager prefabManager_;
};
}