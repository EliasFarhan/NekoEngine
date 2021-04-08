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

private:
    void CreateGraphicsPipeline();
    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateCommandBuffers();
    void RecreateSwapChain();
    void CleanupSwapChain();
    
    VkPipelineLayout pipelineLayout_;
    VkPipeline graphicsPipeline_;
    std::vector<VkFramebuffer> swapChainFramebuffers_;
    VkCommandPool commandPool_;
    std::vector<VkCommandBuffer> commandBuffers_;

};

}