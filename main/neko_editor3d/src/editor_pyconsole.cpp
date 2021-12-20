#include "editor_pyconsole.h"
#include "utils/time_utility.h"
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
    const bool confirmCommand = ImGui::InputText("Command", currentCommand_.data(), commandBufferSize_, ImGuiInputTextFlags_EnterReturnsTrue);
    //bool pressed_enter = IsItemActivePreviousFrame() && !IsItemActive() && IsKeyPressed(io.KeyMap[ImGuiKey_Enter]);
    ImGui::SameLine();
    if(ImGui::Button("Execute") || confirmCommand)
    {

        try
        {
            py::exec(currentCommand_.data());
        }
        catch (py::error_already_set& e)
        {
            e.discard_as_unraisable(__func__);
        }
        previousCommands_.emplace_back(currentCommand_.data());
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
