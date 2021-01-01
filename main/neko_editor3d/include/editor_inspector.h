#pragma once

#include <editor_system.h>

namespace neko::editor
{

class EditorInspector : public EditorSystem
{
public:
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    void DrawImGui() override;
};

}
