#pragma once

#include <editor_system.h>
#include <editor_entity.h>

namespace neko::editor
{
class EditorHierarchy : public EditorSystem
{
public:
    explicit EditorHierarchy(
            EntityManager& entityManager,
            EntityDataManager& entityNameManager);
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;


    void DrawImGui() override;

    [[nodiscard]] Entity GetSelectedEntity()const { return selectedEntity_; }
private:
    void CreateEmptyEntity(Entity parent) const;

    void SetParentEntity(Entity child, Entity parent) const;

    EntityManager& entityManager_;
    EntityDataManager& entityDataManager_;
    Entity selectedEntity_ = INVALID_ENTITY;
};
}