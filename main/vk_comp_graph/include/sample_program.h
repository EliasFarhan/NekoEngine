#pragma once

#include <graphics/graphics.h>
#include <vk/vk_window.h>
#include <sdl_engine/sdl_engine.h>

#include "vk/graphics.h"

namespace neko::vk
{

class SampleProgram : public RenderProgram, public sdl::SdlEventSystemInterface, public DrawImGuiInterface
{
public:
    SampleProgram(VkWindow& window, VkRenderer& renderer);
    virtual ~SampleProgram() = default;

protected:
    std::mutex updateLock_;
    VkWindow& window_;
    VkRenderer& renderer_;
};
class SampleBrowser : public SystemInterface
{
public:
    SampleBrowser(VkWindow& window, VkRenderer& renderer);
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

private:
    std::vector<std::unique_ptr<SampleProgram>> samplePrograms_;

};
}
