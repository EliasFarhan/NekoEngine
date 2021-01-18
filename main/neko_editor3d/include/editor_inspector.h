#pragma once

#include <editor_system.h>
#include <engine/entity.h>

namespace neko::editor
{
class EntityDataManager;
class EditorInspector : public EditorSystem
{
public:
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    void DrawImGui() override;
};

}
