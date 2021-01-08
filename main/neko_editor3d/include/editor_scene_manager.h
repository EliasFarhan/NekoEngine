#pragma once

#include <editor_system.h>
#include <editor_entity.h>
#include <editor_hierarchy.h>

namespace neko::editor
{
class EditorSceneManager : public EditorSystem
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

private:
    EntityManager entityManager_;
    EntityDataManager entityNameManager_{entityManager_};
    EditorHierarchy hierarchy_{entityManager_, entityNameManager_};
};
}