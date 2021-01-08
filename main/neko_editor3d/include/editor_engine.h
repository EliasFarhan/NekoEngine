#pragma once

#include "engine/system.h"
#include "sdl_engine/sdl_engine.h"

#include "engine/entity.h"

#include "editor_entity.h"

#include <map>
#include <memory>
#include <string>



namespace neko::editor
{
class EditorSystem;

class EditorInterface
{
public:
    virtual ~EditorInterface() = default;
    virtual void SetActiveSystem(std::string_view editorSystem, bool isActive) = 0;
};

class NullEditor : public EditorInterface
{
public:
    void SetActiveSystem(std::string_view editorSystem, bool isActive) override {}
};

class Editor :
    public SystemInterface,
    public DrawImGuiInterface,
    public sdl::SdlEventSystemInterface,
    public EditorInterface
{
public:
    Editor();
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void OnEvent(const SDL_Event& event) override;

    void SetActiveSystem(std::string_view editorSystem, bool isActive) override;
private:
    void AddEditorSystem(std::string_view editorSystemName, std::unique_ptr<EditorSystem> editorSystem);
    
    std::vector<std::unique_ptr<EditorSystem>> editorSystems_;
    std::map<std::string, EditorSystem*> editorSystemsMap_;
    
};

using EditorLocator = Locator<EditorInterface, NullEditor>;

}