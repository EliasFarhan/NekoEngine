#pragma once

#include "engine/system.h"
#include "sdl_engine/sdl_engine.h"

#include <map>
#include <memory>
#include <string>



namespace neko::editor
{
class EditorSystem;

class Editor :
        public SystemInterface,
        public DrawImGuiInterface,
        public sdl::SdlEventSystemInterface
{
public:
    Editor();
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void OnEvent(const SDL_Event& event) override;

private:
    void AddEditorSystem(std::string_view editorSystemName, std::unique_ptr<EditorSystem> editorSystem);
    std::vector<std::unique_ptr<EditorSystem>> editorSystems_;
    std::map<std::string, EditorSystem*> editorSystemsMap_;
};
}