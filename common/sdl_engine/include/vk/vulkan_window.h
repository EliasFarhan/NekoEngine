#pragma once
#include <vector>
#include <optional>


#ifdef NEKO_VULKAN
#include <sdl_engine/sdl_window.h>
#include <vulkan/vulkan.h>
namespace neko::sdl
{

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	bool IsComplete() const
	{
		return graphicsFamily.has_value();
	}
};

class VulkanWindow : public SdlWindow
{
public:
	void Init() override;
	void Destroy() override;
	void GenerateUiFrame() override;
	void OnResize(Vec2u newWindowSize) override;
protected:
	void InitImGui() override;

	//Vulkan initialization methods
	void CreateInstance();
	void SetupDebugMessenger();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	//Vulkan utility methods
	std::vector<const char*> GetRequiredExtensions();
	bool CheckValidationLayerSupport() const;
	static bool IsDeviceSuitable(VkPhysicalDevice device);
	static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	//Vulkan members
	VkInstance instance_;
	VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
	VkDevice device_;
	VkQueue graphicsQueue_;

	//Vulkan debug members
	VkDebugUtilsMessengerEXT debugMessenger_;
	const std::vector<const char*> validationLayers_ = {
	"VK_LAYER_KHRONOS_validation"
	};


#ifdef NEKO_VALIDATION_LAYERS
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif
};
}
#endif