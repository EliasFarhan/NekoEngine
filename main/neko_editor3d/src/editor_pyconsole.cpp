#include "editor_pyconsole.h"

#include <imgui.h>
#include <pybind11/embed.h> // everything needed for embedding
namespace py = pybind11;

namespace neko::editor
{
void EditorPyConsole::Init()
{
}

void EditorPyConsole::Update(seconds dt)
{
}

void EditorPyConsole::Destroy()
{
}

void EditorPyConsole::DrawImGui()
{
    ImGui::Begin("PyConsole");
    ImGui::InputText("Command", currentCommand_, commandBufferSize_);
    ImGui::SameLine();
    if(ImGui::Button("Execute"))
    {

        try
        {
            py::exec(currentCommand_);
        }
        catch (py::error_already_set& e)
        {
            e.discard_as_unraisable(__func__);
        }
        previousCommands_.emplace_back(currentCommand_);
        currentCommand_[0] = 0;
        
    }
    ImGui::BeginChild("Commands");
    for (auto it = previousCommands_.rbegin(); it != previousCommands_.rend(); it++)
    {
        ImGui::Text("%s", it->c_str());
    }
    ImGui::EndChild();
    ImGui::End();
}
}
