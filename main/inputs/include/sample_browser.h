#pragma once

#include <vector>
#include <memory>
#include <SDL_events.h>
#include "sample_program.h"
#include "sdl_engine/sdl_engine.h"

namespace neko
{
class SampleBrowser : public SystemInterface, public DrawImGuiInterface, public sdl::SdlEventSystemInterface
{
public:
    void Init() override;

    void Update(seconds dt) override;
    void DrawImGui() override;

    void Destroy() override;

    void SwitchToProgram(size_t index);
    void OnEvent(const SDL_Event& event) override;
protected:

    size_t RegisterRenderProgram(const std::string_view name, std::unique_ptr<SampleProgram> program);

    size_t currentProgramIndex_ = 0;
    std::vector<std::unique_ptr<SampleProgram>> programs_;
    std::vector<std::string> programsNames_;
};
}