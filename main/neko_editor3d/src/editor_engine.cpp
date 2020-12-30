#include <editor_engine.h>
#include "model_viewer.h"

namespace neko::editor
{

Editor::Editor()
{
    AddEditorSystem("ModelViewer", std::make_unique<ModelViewer>());
}
void Editor::Init()
{
    for(auto& editorSystem : editorSystems_)
    {
        if(editorSystem->IsActive())
        {
            editorSystem->Init();
        }
    }
}

void Editor::Update(seconds dt)
{
    for(auto& editorSystem : editorSystems_)
    {
        if(editorSystem->IsActive())
        {
            editorSystem->Update(dt);
        }
    }
}

void Editor::Destroy()
{
    for(auto& editorSystem : editorSystems_)
    {
        editorSystem->Destroy();
    }
}

void Editor::DrawImGui()
{
    for(auto& editorSystem : editorSystems_)
    {
        if(editorSystem->IsActive())
        {
            editorSystem->DrawImGui();
        }
    }
}

void Editor::OnEvent(const SDL_Event& event)
{

}

void Editor::AddEditorSystem(std::string_view editorSystemName, std::unique_ptr<EditorSystem> editorSystem)
{
    editorSystems_.push_back(std::move(editorSystem));
    editorSystemsMap_[editorSystemName.data()] = editorSystems_.back().get();
}


}
