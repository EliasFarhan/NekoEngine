#include <editor_inspector.h>
#include <imgui.h>
#include <editor_engine.h>
#include <editor_entity.h>

namespace neko::editor
{

void EditorInspector::Init()
{
}

void EditorInspector::Update(seconds dt)
{
}

void EditorInspector::Destroy()
{
}

void EditorInspector::DrawImGui()
{
    ImGui::Begin("Inspector");
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
       
        if(ImGui::BeginMenu("Window"))
        {

            static bool sceneManagerEnable = true;
            if (ImGui::MenuItem("SceneManager", 0, sceneManagerEnable))
            {
                sceneManagerEnable = !sceneManagerEnable;
                EditorLocator::get().SetActiveSystem("SceneManager", sceneManagerEnable);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::End();
}
}
