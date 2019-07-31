//
// Created by efarhan on 7/31/19.
//

#include <tools/entity_viewer.h>
#include <imgui.h>
namespace editor
{
void EntityViewer::Update(neko::EntityManager& entityManager)
{
    entities_.clear();
    entitiesName_.clear();
    for(neko::Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if(entityManager.EntityExists(entity))
        {
            entities_.push_back(entity);
            entitiesName_.push_back("Entity "+std::to_string(entity));
        }
    }
    ImGui::Begin("Entity Viewer");
    for(int i = 0; i < entities_.size();i++)
    {
        if(ImGui::Selectable(entitiesName_[i].c_str(), selectedEntity_ == i))
        {
            selectedEntity_ = i;
        }
    }
    ImGui::End();
}
}