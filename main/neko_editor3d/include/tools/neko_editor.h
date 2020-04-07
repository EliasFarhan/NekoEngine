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
#include <sdl_engine/sdl_engine.h>
#include <graphics/texture.h>
#include <gl/texture.h>
#include "entity_viewer.h"
#include "inspector.h"
#include "editor_scene.h"
#include "log_viewer.h"
#include "previewer.h"
#include "editor_prefab.h"
#include "imgui.h"
#include "imfilebrowser.h"
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

class NekoEditor : public neko::sdl::SdlEngine
{
public:
    NekoEditor();
    void Init() override;

    void EditorUpdate(float dt);

    void Destroy() override;

    //void SwitchEditorMode(EditorMode editorMode);
    //void SaveSceneEvent();
    //void SavePrefabEvent();

    void OnEvent(const SDL_Event& event) override;
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
    FileOperation currentFileOperation_ = FileOperation::NONE;

    EditorSystemId currentEditorSystemId_ = INVALID_EDITOR_SYSTEM_ID;
    std::unordered_map<EditorSystemId, BasicEditorSystem*> editorSystemMap_;

};


class BasicEditorSystem : public SystemInterface
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
	explicit NekoEditorSystem();
protected:
	EntityManager entityManager_;
	Position2dManager position2dManager_;
	Scale2dManager scale2dManager_;
	Rotation2dManager rotation2dManager_;
	Transform2dManager transform2dManager_;

	EntityViewer entityViewer_;
	Inspector inspector_;
	EntityNameManager entityNameManager_;

};

}
