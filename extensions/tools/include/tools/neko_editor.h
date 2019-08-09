#pragma once

/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <engine/engine.h>
#include <engine/transform.h>
#include <engine/scene.h>
#include "entity_viewer.h"
#include "scene_viewer.h"
#include "inspector.h"
#include "editor_scene.h"
#include "log_viewer.h"
#include "previewer.h"
#include "physics_editor.h"
#include "prefab_viewer.h"

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
    ColliderDefManager& GetColliderDefManager();

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
    PrefabViewer prefabViewer_;

    EditorSceneManager sceneManager_;

    neko::BodyDef2dManager bodyDefManager_;
    ColliderDefManager colliderDefManager_;


    Inspector inspector_;
    LogViewer logViewer_;
    Previewer previewer_;


    FileOperation fileOperationStatus_ = FileOperation::OPEN;

    neko::Transform2dManager transformManager_;
};

}