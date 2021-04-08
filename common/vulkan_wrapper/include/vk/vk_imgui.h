#pragma once
#include "glm/glm.hpp"

namespace neko::vk
{
class VkWindow;
class VkImGUI : public SystemInterface
{
public:
    VkImGUI(VkWindow& window);

    void Init() override;
    void Destroy() override;

    void Update(seconds dt) override;
    void Render();

private:
    void CreateFramebuffers();
    VkWindow& window_;
    VkDescriptorPool descriptorPool_;
    VkCommandPool commandPool_;
    VkRenderPass renderPass_;
    std::vector<VkFramebuffer> framebuffers_;
};
}