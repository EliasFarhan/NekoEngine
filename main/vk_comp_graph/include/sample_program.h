#pragma once

#include <graphics/graphics.h>
#include <vk/vk_window.h>
#include <sdl_engine/sdl_engine.h>

#include "vk/graphics.h"

namespace neko::vk
{

class SampleProgram : public RenderProgram,
    public sdl::SdlEventSystemInterface,
    public DrawImGuiInterface,
    public RecreateSwapchainInterface
{
public:
    SampleProgram(VkWindow& window, VkRenderer& renderer);
    virtual ~SampleProgram() = default;
    virtual void CleanupSwapChain() = 0;
    virtual void CreateSwapChain() = 0;
protected:
    std::mutex updateLock_;
    VkWindow& window_;
    VkRenderer& renderer_;
};
class SampleBrowser :
    public SystemInterface,
    public DrawImGuiInterface,
    public RecreateSwapchainInterface
{
public:
    SampleBrowser(VkWindow& window, VkRenderer& renderer);
    void Init() override;

    void Update(seconds dt) override;
    void DrawImGui() override;
    void Destroy() override;
    void CleanupSwapChain() override;
    void CreateSwapChain() override;
private:
    std::vector<std::unique_ptr<SampleProgram>> samplePrograms_;
    std::size_t currentIndex_ = 0;

};
}
