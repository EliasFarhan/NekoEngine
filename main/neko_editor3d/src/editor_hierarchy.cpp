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
    for(Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        if(entityManager_.EntityExists(entity))
        {
            ImGui::Text("%s", entityNameManager_.GetComponent(entity).c_str());
        }
    }
    ImGui::End();
}

EditorHierarchy::EditorHierarchy(
        EntityManager& entityManager,
        EntityNameManager& entityNameManager) :
    entityManager_(entityManager),
    entityNameManager_(entityNameManager)
{

}
}