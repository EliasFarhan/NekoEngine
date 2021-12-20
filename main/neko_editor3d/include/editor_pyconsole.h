#pragma once
#include "editor_system.h"
#include <vector>

namespace neko::editor
{

class EditorPyConsole : public EditorSystem
{
public:
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    void DrawImGui() override;
private:
    std::vector<std::string> previousCommands_;
    static constexpr size_t commandBufferSize_ = 128;
    std::array<char, commandBufferSize_> currentCommand_{};
};

}
