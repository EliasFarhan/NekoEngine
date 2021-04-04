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

private:
    VkRenderPass renderPass_;
    VkWindow& window_;
    VkDescriptorPool descriptorPool_;
    VkCommandPool commandPool_;
};
}