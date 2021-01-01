//
// Created by efarhan on 01/01/2021.
//

#include <editor_hierarchy.h>
#include <imgui.h>

namespace neko::editor
{

void EditorHierarchy::Init()
{

}

void EditorHierarchy::Update(seconds dt)
{

}

void EditorHierarchy::Destroy()
{

}

void EditorHierarchy::DrawImGui()
{
    ImGui::Begin("Entity Hierarchy");
    ImGui::End();
}

EditorHierarchy::EditorHierarchy(
        EntityManager& entityManager,
        EntityNameManager& entityNameManager) :
    entityNameManager_(entityNameManager),
    entityManager_(entityManager)
{

}
}