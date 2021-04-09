#pragma once

/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "graphics/graphics.h"
#include "vk/vk_window.h"
#include <vulkan/vulkan.h>

namespace neko::vk
{

class VkRenderer : public Renderer, public RecreateSwapchainInterface
{
public:
    VkRenderer(VkWindow& window);
    void Init();
    void Destroy();
    void ClearScreen() override;

    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    VkRenderPass GetRenderPass() { return renderPass_; }
    VkSemaphore GetRenderFinishedSemaphore() { return renderFinishedSemaphore_; }
    std::uint32_t GetImageIndex() const { return imageIndex_; }
    VkSemaphore GetImageAvailableSemaphore() { return imageAvailableSemaphore_; }
    const std::vector<VkFramebuffer>& GetFramebuffers()const { return framebuffers_; }
    VkCommandPool GetCommandPool() { return commandPool_; }
    std::vector<VkCommandBuffer> GetCommandBuffers() { return commandBuffers_; }
    void CleanupSwapChain() override;
    void CreateSwapChain() override;
protected:
    void BeforeRender() override;
    void AfterRender() override;
private:
    void CreateRenderPass();
    void CreateSemaphores();
    void CreateFramebuffers();
    void CreateCommandPool();

    void CreateCommandBuffers();
    VkWindow& window_;
    VkRenderPass renderPass_;
    VkSemaphore imageAvailableSemaphore_;
    VkSemaphore renderFinishedSemaphore_;
    std::uint32_t imageIndex_ = 0;
    std::vector<VkFramebuffer> framebuffers_;
    VkCommandPool commandPool_;
    std::vector<VkCommandBuffer> commandBuffers_;
};

}