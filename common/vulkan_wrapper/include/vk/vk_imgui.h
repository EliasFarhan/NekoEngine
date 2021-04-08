#pragma once
#include "glm/glm.hpp"

namespace neko::vk
{
class VkRenderer;
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
    static VkRenderer& GetRenderer();
    VkWindow& window_;
    VkRenderer* renderer_ = nullptr;
    VkDescriptorPool descriptorPool_;
};
}