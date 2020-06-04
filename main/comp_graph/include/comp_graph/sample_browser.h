#pragma once

#include <vector>
#include <memory>
#include <SDL_events.h>
#include "comp_graph/sample_program.h"
#include "sdl_engine/sdl_engine.h"

namespace neko
{
class SampleBrowser : public SystemInterface, public DrawImGuiInterface, public sdl::SdlEventSystemInterface
{
public:
	/**
	 * \brief Executed on the render thread
	 */
    void Init() override;
    /**
     * \brief Executed on the main thread
     */
    void Update(seconds dt) override;
    /**
     * \brief Executed on the render thread
     */
    void DrawImGui() override;
    /**
     * \brief Executed on the render thread
     */
    void Destroy() override;
    /**
     * \brief Executed on the render thread
	 */
    void SwitchToProgram(size_t index);
    /**
     * \brief Executed on the main thread
     */
    void OnEvent(const SDL_Event& event) override;
protected:

    size_t RegisterRenderProgram(const std::string_view name, std::unique_ptr<SampleProgram> program);

    size_t currentProgramIndex_ = 0;
    std::vector<std::unique_ptr<SampleProgram>> programs_;
    std::vector<std::string> programsNames_;
};
}