
#include <hello_triangle.h>

namespace neko::vk
{
void HelloTriangle::Init()
{
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateFramebuffers();
    CreateCommandPool();
    CreateCommandBuffers();
    CreateSemaphores();
}

void HelloTriangle::Update(seconds dt)
{

}

void HelloTriangle::Destroy()
{
    CleanupSwapChain();
    auto& driver = window_.GetDriver();
    vkDestroySemaphore(driver.device_, renderFinishedSemaphore_, nullptr);
    vkDestroySemaphore(driver.device_, imageAvailableSemaphore_, nullptr);
    vkDestroyCommandPool(driver.device_, commandPool_, nullptr);

}

void HelloTriangle::DrawImGui()
{

}

void HelloTriangle::Render()
{

}
}
