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
    void Init() override { initialized_ = true; }
    void Destroy() override { initialized_ = false; }
protected:
    std::mutex updateLock_;
    VkWindow& window_;
    VkRenderer& renderer_;
    bool initialized_ = false;
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
    void RegisterSample(std::string_view sampleName, std::unique_ptr<SampleProgram> sample);
private:
    std::vector<std::unique_ptr<SampleProgram>> samplePrograms_;
    std::vector<std::string> sampleNames_;
    std::size_t currentIndex_ = 0;

};
}
