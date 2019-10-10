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

#include <tools/entity_viewer.h>
#include <imgui.h>
#include <tools/editor_scene.h>
#include <utilities/vector_utility.h>
#include <engine/transform.h>
#include <engine/log.h>
#include <tools/neko_editor.h>
#include <tools/editor_prefab.h>

namespace editor
{
void EntityViewer::Update(EditorMode editorMode)
{
    auto& sceneManager = nekoEditor_.GetSceneManager();
    auto& entityManager = nekoEditor_.GetEntityManager();
    auto& transformManager = nekoEditor_.GetTransformManager();
    entities_.clear();
    entitiesName_.clear();

    ImGui::Begin("Entity Viewer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    const bool sceneOpen = ImGui::CollapsingHeader(editorMode == EditorMode::PrefabMode?"Prefab Edition Mode":"Scene", ImGuiTreeNodeFlags_DefaultOpen);
    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
        target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", target_flags))
        {
	        const neko::Entity moveFrom = *static_cast<const neko::Entity*>(payload->Data);
	        const neko::Entity moveTo = neko::INVALID_ENTITY;
            transformManager.SetTransformParent(moveFrom, moveTo);
        }
        ImGui::EndDragDropTarget();
    }

    for (neko::Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.EntityExists(entity))
        {
            entities_.push_back(entity);
            entitiesName_.push_back(sceneManager.GetCurrentScene().entitiesNames[entity]);
        }
    }

    std::set<neko::Entity> entitiesSet;
    for (size_t i = 0; i < entities_.size(); i++)
    {

        if(transformManager.GetParentEntity(entities_[i]) == neko::INVALID_ENTITY and
			std::find(entitiesSet.cbegin(), entitiesSet.cend(), entities_[i]) == entitiesSet.end())
        {
            DrawEntityHierarchy(entities_[i], i, entitiesSet, sceneOpen, false);
        }
    }

    if (ImGui::Button("Add Entity"))
    {
        auto entity = entityManager.CreateEntity();
        auto& entitiesName = sceneManager.GetCurrentScene().entitiesNames;
        ResizeIfNecessary(entitiesName, entity, std::string());
        entitiesName[entity] = std::string("Entity ") + std::to_string(entity);
        transformManager.SetTransformParent(entity,
                editorMode == EditorMode::SceneMode?neko::INVALID_ENTITY:0);
    }

    ImGui::End();
}

void EntityViewer::DrawEntityHierarchy(neko::Entity entity,
        size_t index,
        std::set<neko::Entity>& entitySet,
        bool draw,
        bool destroy)
{
    auto& transformManager = nekoEditor_.GetTransformManager();
    bool nodeOpen = draw;
    bool destroyEntity = destroy;
    bool createEntity = false;
    bool leaf = transformManager.FindNextChild(entity) == neko::INVALID_ENTITY;

    if(draw)
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
        if(!leaf)
        {
            nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
        }
        else
        {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }
        if (entity == selectedEntity_)
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.7f);
        nodeOpen = ImGui::TreeNodeEx((void*) (intptr_t) entities_[index], nodeFlags, "%s",
                                     entitiesName_[index].c_str());
        ImGui::PopItemWidth();
        if (ImGui::IsItemClicked())
            selectedEntity_ = entity;

        const std::string entityPopupName = "Entity Popup "+std::to_string(entity);
        if(ImGui::IsItemClicked(1))
        {
            logDebug("Left Clicked on Entity: "+std::to_string(entity));

            ImGui::OpenPopup(entityPopupName.c_str());
        }

        if(ImGui::BeginPopup(entityPopupName.c_str()))
        {
            const std::string entityMenuName = "Entity Menu "+std::to_string(entity);
            enum class EntityMenuComboItem
            {
                ADD_EMPTY_ENTITY,
                DELETE_ENTITY,
                MAKE_PREFAB,
                LENGTH
            };
            const char* entityMenuComboItemName[int(EntityMenuComboItem::LENGTH)] = {
                    "Add Empty Entity",
                    "Delete Entity",
                    "Make Prefab"
            };

            const auto entityComboName = entityMenuName + " Combo";
            for(int i = 0 ; i < int(EntityMenuComboItem::LENGTH);i++)
            {
                const auto key = entityComboName+" "+entityMenuComboItemName[i];
                ImGui::PushID(key.c_str());
                if(ImGui::Selectable(entityMenuComboItemName[i]))
                {
                    const auto item = EntityMenuComboItem(i);
                    switch (item)
                    {
                        case EntityMenuComboItem::ADD_EMPTY_ENTITY:
                        {
                            createEntity = true;
                            break;
                        }
                        case EntityMenuComboItem::DELETE_ENTITY:
                        {
                            destroyEntity = true;
                            break;
                        }
                        case EntityMenuComboItem::MAKE_PREFAB:
                        {
                            auto& prefabManager = nekoEditor_.GetPrefabManager();
							auto& sceneManager = nekoEditor_.GetSceneManager();
							auto& entityManager = nekoEditor_.GetEntityManager();
                        	const auto newIndex = prefabManager.CreatePrefabFromEntity(entity);
							sceneManager.AddComponent(entityManager, entity); //adding prefab
							sceneManager.SetComponent(entity, newIndex);
                            nekoEditor_.SwitchEditorMode(EditorMode::PrefabMode);
                            prefabManager.SetCurrentPrefabIndex(newIndex);
                            nekoEditor_.GetSceneManager().ClearScene();
                            prefabManager.InstantiatePrefab(newIndex, nekoEditor_.GetEntityManager());
                            break;
                        }
                        default:
                            break;
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::PopID();
            }

            ImGui::EndPopup();
        }
        ImGuiDragDropFlags srcFlags = 0;
        srcFlags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
        srcFlags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging

        if (ImGui::BeginDragDropSource(srcFlags))
        {
            if (!(srcFlags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
            {
                ImGui::Text("Moving Entity \"%s\"", entitiesName_[index].c_str());
            }
            ImGui::SetDragDropPayload("DND_DEMO_NAME", &entities_[index], sizeof(neko::Entity));
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            ImGuiDragDropFlags targetFlags = 0;
            //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
            targetFlags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", targetFlags))
            {
                neko::Entity moveFrom = *(const neko::Entity*) payload->Data;
                neko::Entity moveTo = entities_[index];
                transformManager.SetTransformParent(moveFrom, moveTo);
            }
            ImGui::EndDragDropTarget();
        }
    }
    entitySet.emplace(entity);
    if(destroyEntity)
    {
        auto& entityManager = nekoEditor_.GetEntityManager();
        entityManager.DestroyEntity(entity);
    }
    auto entityChild = neko::INVALID_ENTITY;
    do
    {
        auto& entityManager = nekoEditor_.GetEntityManager();
        entityChild = transformManager.FindNextChild(entity, entityChild);
        if(entityChild != neko::INVALID_ENTITY and entityManager.EntityExists(entityChild))
        {
            size_t childIndex = std::find(entities_.cbegin(), entities_.cend(), entityChild) - entities_.begin();
            DrawEntityHierarchy(entityChild, childIndex, entitySet, nodeOpen, destroyEntity);
        }
    }
    while (entityChild != neko::INVALID_ENTITY);

    if(createEntity)
    {
        auto& sceneManager = nekoEditor_.GetSceneManager();
        auto& entityManager = nekoEditor_.GetEntityManager();

        auto newEntity = entityManager.CreateEntity();
        auto& entitiesName = sceneManager.GetCurrentScene().entitiesNames;
        ResizeIfNecessary(entitiesName, newEntity, std::string());
        entitiesName[newEntity] = std::string("Entity ") + std::to_string(newEntity);
        transformManager.SetTransformParent(newEntity, entity);
    }
    if(nodeOpen and !leaf)
        ImGui::TreePop();
}

EntityViewer::EntityViewer(NekoEditor& editor) : nekoEditor_(editor)
{

}
}