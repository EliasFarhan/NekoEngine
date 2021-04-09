#include <vk/vk_imgui.h>
#include <vk/vk_window.h>
#include <vk/graphics.h>
#include <engine/assert.h>


namespace neko::vk
{

VkImGUI::VkImGUI(VkWindow& window) : window_(window)
{
}

void VkImGUI::Init()
{
    auto& driver = window_.GetDriver();
    auto& swapchain = window_.GetSwapchain();
    auto& renderer = *VkWindow::GetRenderer();
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
        const auto status = vkCreateDescriptorPool(driver.device, &pool_info, nullptr, &descriptorPool_);
        if(status != VK_SUCCESS)
        {
            logDebug("[Error] Could not create descriptor pool for ImGui");
            neko_assert("", false);
        }
    }

    CreateSwapChain();
}

void VkImGUI::Update(seconds dt)
{

}

void VkImGUI::Render()
{
    
    auto& renderer = *VkWindow::GetRenderer();
    const auto imageIndex = renderer.GetImageIndex();
    auto commandBuffer = renderer.GetCommandBuffers()[imageIndex];
   
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    ImGui::GetDrawData()->Clear();
    
}

void VkImGUI::CleanupSwapChain()
{
    ImGui_ImplVulkan_Shutdown();
}

void VkImGUI::CreateSwapChain()
{
    auto& driver = window_.GetDriver();
    auto& swapchain = window_.GetSwapchain();
    auto& renderer = *VkWindow::GetRenderer();

    ImGui_ImplVulkan_InitInfo imguiInfo{};
    imguiInfo.Instance = driver.instance;
    imguiInfo.PhysicalDevice = driver.physicalDevice;
    imguiInfo.Device = driver.device;
    imguiInfo.Queue = driver.graphicsQueue;
    imguiInfo.DescriptorPool = descriptorPool_;
    imguiInfo.MinImageCount = swapchain.minImageCount;
    imguiInfo.ImageCount = swapchain.imageCount;

    ImGui_ImplVulkan_Init(&imguiInfo, renderer.GetRenderPass());


    //Upload Font to GPU
    VkCommandBuffer commandBuffer = renderer.BeginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    renderer.EndSingleTimeCommands(commandBuffer);

    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void VkImGUI::Destroy()
{
    auto& driver = window_.GetDriver();
    auto& swapchain = window_.GetSwapchain();
    ImGui_ImplVulkan_Shutdown();
    
    vkDestroyDescriptorPool(driver.device, descriptorPool_, nullptr);
}
}
