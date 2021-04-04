#include <vk/vk_imgui.h>
#include <vk/vk_window.h>

namespace neko::vk
{

vk::VkImGUI::VkImGUI(VkWindow& window) : window_(window)
{
}

void VkImGUI::Init()
{
    auto& driver = window_.GetDriver();
    auto& swapchain = window_.GetSwapchain();
    auto allocator = window_.GetAllocator();
    // Create Descriptor Pool
    {
        VkDescriptorPoolSize pool_sizes[] =
                {
                        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
                };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        const auto status = vkCreateDescriptorPool(driver.device_, &pool_info, nullptr, &descriptorPool_);
        if(status != VK_SUCCESS)
        {
            logDebug("[Error] Could not create descriptor pool for ImGui");
            neko_assert("", false);
        }
    }


    VkAttachmentDescription attachment = {};
    attachment.format = swapchain.swapChainImageFormat_;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment = {};
    color_attachment.attachment = 0;
    color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;

    if (vkCreateRenderPass(driver.device_, &info, nullptr, &renderPass_) != VK_SUCCESS)
    {
        logDebug("[Error] Could not create Dear ImGui's render pass");
        neko_assert(false, "");
    }
    ImGui_ImplVulkan_InitInfo imguiInfo=
    {
        driver.instance_,
        driver.physicalDevice_,
        driver.device_,
        0,
        driver.graphicsQueue_,
        VK_NULL_HANDLE,
        descriptorPool_,
        swapchain.minImageCount,
        swapchain.imageCount,
        VK_SAMPLE_COUNT_1_BIT,
        nullptr,
        nullptr
    };
    ImGui_ImplVulkan_Init(&imguiInfo, renderPass_);

    VkCommandBuffer commandBuffer = window_.BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    window_.EndSingleTimeCommands(commandBuffer);
}

void VkImGUI::Update(seconds dt)
{

}

void VkImGUI::Destroy()
{
    auto& driver = window_.GetDriver();
    auto& swapchain = window_.GetSwapchain();
    ImGui_ImplVulkan_Shutdown();

    vkDestroyRenderPass(driver.device_, renderPass_, nullptr);
    vkDestroyDescriptorPool(driver.device_, descriptorPool_, nullptr);
}
}