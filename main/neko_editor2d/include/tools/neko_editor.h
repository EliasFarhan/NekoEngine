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
#include "entity_viewer.h"
#include "scene_viewer.h"
#include "inspector.h"
#include "editor_scene.h"
#include "log_viewer.h"
#include "previewer.h"
#include "physics_editor.h"
#include "editor_prefab.h"
#include "imgui.h"
#include "imfilebrowser.h"
#include "sfml_engine/sprite.h"
#include "sfml_engine/texture.h"
#include "sfml_engine/engine.h"
#include "tools/engine_export.h"
#include "tools/editor_entity_name.h"

namespace sf {
class Event;
}

namespace neko::editor
{
class BasicEditorSystem;
enum class FileOperation
{
    OPEN,
    SAVE,
    NONE,
};
enum class EditorSystemMode  : std::uint8_t
{
    SceneMode,
    PrefabMode,
    TextureMode,
    AnimMode,
    SpineMode,
    None
};


using EditorSystemId = xxh::hash64_t;
const EditorSystemId INVALID_EDITOR_SYSTEM_ID = EditorSystemId(0);

class NekoEditor : public neko::sfml::SfmlBasicEngine
{
public:
    NekoEditor();
    void Init() override;

    void EditorUpdate(float dt);

    void Destroy() override;

    //void SwitchEditorMode(EditorMode editorMode);
    //void SaveSceneEvent();
    //void SavePrefabEvent();

    void OnEvent(sf::Event& event) override;
    void OpenAsset(std::string_view assetPath);

	void RemoveEditorSystem(EditorSystemId editorSystemId, bool destroy);
	void SetCurrentEditorSystem(BasicEditorSystem* editorSystem);
protected:

    static EditorSystemMode GetEditorSystemModeFrom(std::string_view extension);
    BasicEditorSystem* GetCurrentEditorSystem();
    void CreateNewScene();
    void OpenFileDialog();
    void Save(bool saveAs);
    void SaveAsset(std::string_view assetPath);
    LogViewer logViewer_;

    ImGui::FileBrowser fileDialog_;
    EditorSystemMode currentEditorMode_ = EditorSystemMode::SceneMode;
	std::vector<std::unique_ptr<BasicEditorSystem>> editorSystems_;
	sfml::TextureManager textureManager_;
    FileOperation currentFileOperation_ = FileOperation::NONE;

    EditorSystemId currentEditorSystemId_ = INVALID_EDITOR_SYSTEM_ID;
    std::unordered_map<EditorSystemId, BasicEditorSystem*> editorSystemMap_;

};


class BasicEditorSystem : public System
{
public:
	explicit BasicEditorSystem(Configuration& config);
	std::string GetSystemName() const { return editorSystemName_ + (IsTmpResource() ? "*" : ""); }
	void SetSystemName(const std::string& name) { editorSystemName_ = name; }

	virtual void OnListingView() = 0;
	virtual void OnMainView() = 0;
	virtual void OnInspectorView() = 0;

	virtual void OnSave() = 0;

	virtual void OnEvent([[maybe_unused]]sf::Event& event){};
	virtual void OnLostFocus(){};

    EditorSystemMode GetEditorMode() const
    {
        return editorMode_;
    }

    virtual EditorSystemId GetEditorSystemId() const = 0;

    const std::string& GetResourcePath() const;
    virtual void SetResourcePath(const std::string& resourcePath);
    bool IsTmpResource() const;
protected:
    std::string resourcePath_;
    EditorSystemMode editorMode_ = EditorSystemMode::None;
    std::string editorSystemName_;
	Configuration& config_;
};

class NekoEditorSystem : public BasicEditorSystem
{
public:
	explicit NekoEditorSystem(NekoEditor& nekoEditor, sfml::TextureManager& textureManager);
protected:
	sf::RenderTexture screenRenderTexture_;
	NekoEditorExport editorExport_;
	EntityManager entityManager_;
	Position2dManager position2dManager_;
	Scale2dManager scale2dManager_{Vec2f(1.0f,1.0f)};
	Rotation2dManager rotation2dManager_;
	sfml::Transform2dManager transform2dManager_;
	EditorSceneManager sceneManager_;
	box2d::BodyDef2dManager bodyDef2DManager_;
	sfml::SpriteManager spriteManager_;
	sfml::SpineManager spineManager_;
	sfml::SpineBoneFollowerManager spineBoneFollowerManager_;
	BoxColliderDefManager boxColliderDefManager_;
	CircleColliderDefManager circleColliderDefManager_;
	PolygonColldierDefManager polygonColldierDefManager_;
	ColliderDefManager colliderManagerDefManager_;
	EditorPrefabManager prefabManager_;
	GraphicsManager graphicsManager_;
	EntityViewer entityViewer_;
	Inspector inspector_;
	SceneViewer sceneViewer_;
	EntityNameManager entityNameManager_;

};

}
