#pragma once

#include <graphics/graphics.h>
#include <vk/vk_window.h>
#include <sdl_engine/sdl_engine.h>

namespace neko::vk
{

class SampleProgram : public RenderProgram, public sdl::SdlEventSystemInterface, public DrawImGuiInterface
{
public:
    SampleProgram(VkWindow& window);
    virtual ~SampleProgram() = default;

protected:
    std::mutex updateLock_;
    VkWindow& window_;
};
class SampleBrowser : public SystemInterface
{
public:
    SampleBrowser(VkWindow& window);
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

private:
    std::vector<std::unique_ptr<SampleProgram>> samplePrograms_;

};
}