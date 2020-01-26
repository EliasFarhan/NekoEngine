#pragma once

#include <vector>
#include <memory>
#include <SDL_events.h>
#include "comp_graph/sample_program.h"

namespace neko
{
class SampleBrowser : public System
{
public:
    void Init() override;

    void Update(seconds dt) override;
    void Render();
    void DrawGui(seconds dt);

    void Destroy() override;

    void SwitchToProgram(size_t index);
    void OnEvent(const SDL_Event& event);
protected:

    size_t RegisterRenderProgram(const std::string_view name, std::unique_ptr<SampleProgram> program);

    size_t currentProgramIndex_ = 0;
    std::vector<std::unique_ptr<SampleProgram>> programs_;
    std::vector<std::string> programsNames_;
};
}