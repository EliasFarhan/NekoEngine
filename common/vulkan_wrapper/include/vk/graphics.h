#pragma once
#include <graphics/graphics.h>
#include <vulkan/vulkan.h>
namespace neko::vk
{
class VulkanRenderer : public Renderer
{
protected:
	void ClearScreen() override;
};
}
