#pragma once

#include <editor_system.h>
#include <editor_entity_name.h>

namespace neko::editor
{
class EditorHierarchy : public EditorSystem
{
public:
    explicit EditorHierarchy(
            EntityManager& entityManager,
            EntityNameManager& entityNameManager);
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

private:
    EntityManager& entityManager_;
    EntityNameManager& entityNameManager_;
};
}