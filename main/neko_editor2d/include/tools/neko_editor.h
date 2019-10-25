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
#include "editor_prefab.h"

#include <imgui.h>
#include "imfilebrowser.h"
#include "sfml_engine/sprite.h"
#include "sfml_engine/texture.h"
#include <engine/prefab.h>
#include "sfml_engine/engine.h"

namespace sf {
class Event;
}

namespace neko::editor
{
enum class FileOperation
{
    OPEN_SCENE,
    OPEN_PREFAB,
    SAVE_SCENE,
    SAVE_PREFAB,
    NONE,
};
enum class EditorMode : std::uint8_t
{
    SceneMode,
    PrefabMode
};

struct NekoEditorExport
{
	EntityManager& entityManager;
	Position2dManager& position2dManager;
	Scale2dManager& scale2dManager;
	Rotation2dManager& rotation2dManager;
	sfml::Transform2dManager& transform2dManager;
	EditorSceneManager& sceneManager;
	box2d::BodyDef2dManager& bodyDef2dManager;
	sfml::SpriteManager& spriteManager;
	sfml::TextureManager& textureManager;
	sfml::SpineManager& spineManager;
	BoxColliderDefManager& boxColliderDefManager_;
	CircleColliderDefManager& circleColliderDefManager_;
	PolygonColldierDefManager& polygonColldierDefManager_;
	ColliderDefManager& coll;
	EditorPrefabManager& prefabManager;
	NekoEditor& editor;
};


class NekoEditor : public neko::sfml::SfmlBasicEngine
{
public:
    NekoEditor();
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

    void SwitchEditorMode(EditorMode editorMode);


    EditorPrefabManager& GetPrefabManager();
    Previewer& GetPreviewer();

    void OnEvent(sf::Event& event) override;
    void SaveSceneEvent();
    void SavePrefabEvent();

private:
    neko::EntityManager entityManager_;
    EntityViewer entityViewer_;
    neko::sfml::TextureManager textureManager_;
    EditorPrefabManager prefabManager_;

    sfml::SpineManager spineManager_;
    sfml::SpriteManager spriteManager_;

    sfml::SfmlGraphicsManager graphicsManager_;
    sf::RenderTexture sceneRenderTexture_;
    SceneViewer sceneViewer_;

    EditorSceneManager sceneManager_;

    box2d::BodyDef2dManager bodyDefManager_;
	BoxColliderDefManager boxColliderDefManager_;
	CircleColliderDefManager circleColliderDefManager_;
	PolygonColldierDefManager polygonColldierDefManager_;
    ColliderDefManager colliderDefManager_;


    Inspector inspector_;
    LogViewer logViewer_;
    Previewer previewer_;

    ImGui::FileBrowser fileDialog_;
    FileOperation fileOperationStatus_ = FileOperation::NONE;
    EditorMode editorMode_ = EditorMode::SceneMode;

	Position2dManager position2dManager_;
	Scale2dManager scale2dManager_;
	Rotation2dManager rotation2dManager_;
    sfml::Transform2dManager transformManager_;

	NekoEditorExport editorExport_
	{
		entityManager_,
		position2dManager_,
		scale2dManager_,
		rotation2dManager_,
		transformManager_,
		sceneManager_,
		bodyDefManager_,
		spriteManager_,
		textureManager_,
		spineManager_,
		boxColliderDefManager_,
		circleColliderDefManager_,
		polygonColldierDefManager_,
		colliderDefManager_,
		prefabManager_,
		*this
	};
};

}
