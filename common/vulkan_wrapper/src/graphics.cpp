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

#include <vk/graphics.h>

#include "engine/assert.h"

namespace neko::vk
{

VkRenderer::VkRenderer(VkWindow& window) : window_(window)
{

}

void VkRenderer::Init()
{
    CreateRenderPass();
    CreateSemaphores();
}

void VkRenderer::Destroy()
{
    auto& driver = window_.GetDriver();

    vkDeviceWaitIdle(driver.device);
    vkDestroySemaphore(driver.device, renderFinishedSemaphore_, nullptr);
    vkDestroySemaphore(driver.device, imageAvailableSemaphore_, nullptr);
    vkDestroyRenderPass(driver.device, renderPass_, nullptr);
}

void VkRenderer::ClearScreen()
{

}
void VkRenderer::BeforeRender()
{
    auto& driver = window_.GetDriver();
    auto& swapchain = window_.GetSwapchain();
    const auto result = vkAcquireNextImageKHR(
        driver.device,
        swapchain.swapChain,
        UINT64_MAX,
        imageAvailableSemaphore_,
        VK_NULL_HANDLE,
        &imageIndex_);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        //RecreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        logDebug("[Error] failed to acquire swap chain image!");
        neko_assert(false, "");
    }
}
void VkRenderer::AfterRender()
{
    auto& driver = window_.GetDriver();
    auto& swapchain = window_.GetSwapchain();

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    VkSemaphore signalSemaphores[] = { renderFinishedSemaphore_ };
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = { swapchain.swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex_;
    presentInfo.pResults = nullptr; // Optional
    vkQueuePresentKHR(driver.presentQueue, &presentInfo);

    vkDeviceWaitIdle(driver.device);
}

void VkRenderer::CreateRenderPass()
{
    auto& driver = window_.GetDriver();
    auto& swapchain = window_.GetSwapchain();

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapchain.imageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(driver.device, &renderPassInfo, nullptr, &renderPass_) != VK_SUCCESS)
    {
        logDebug("[Error] failed to create render pass!");
        neko_assert(false, "");
    }
}

void VkRenderer::CreateSemaphores()
{
    auto& driver = window_.GetDriver();
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(driver.device, &semaphoreInfo, nullptr, &imageAvailableSemaphore_) != VK_SUCCESS ||
        vkCreateSemaphore(driver.device, &semaphoreInfo, nullptr, &renderFinishedSemaphore_) != VK_SUCCESS)
    {

        logDebug("[Error] failed to create semaphores!");
        neko_assert(false, "");
    }
}
}
