#pragma once

#include <sample_program.h>
#include <vulkan/vulkan.h>

namespace neko::vk
{

class HelloTriangle : public SampleProgram
{
public:
    using SampleProgram::SampleProgram;
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;

    void CleanupSwapChain() override;
    void CreateSwapChain() override;
private:
    void CreateGraphicsPipeline();
    
    VkPipelineLayout pipelineLayout_;
    VkPipeline graphicsPipeline_;


};

}