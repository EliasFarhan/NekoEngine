#pragma once
#include <vector>
#include <optional>



#ifdef NEKO_VULKAN
#include <sdl_engine/sdl_window.h>
#include <vulkan/vulkan.h>
#include <utilities/file_utility.h>
namespace neko {
struct BufferFile;
}

namespace neko::sdl
{

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	bool IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
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
	void CreateSurface();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateGraphicsPipeline();
	void CreateRenderPass();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateDescriptorPool();

	//Vulkan utility methods
	std::vector<const char*> GetRequiredExtensions();
	bool CheckValidationLayerSupport() const;
	bool IsDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkShaderModule CreateShaderModule(const BufferFile& shaderFile);
	//Vulkan members
	VkInstance instance_;
	VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
	VkDevice device_;
	VkQueue graphicsQueue_;
	VkSurfaceKHR surface_;
	VkQueue presentQueue_;
	VkSwapchainKHR swapChain_;
	std::vector<VkImage> swapChainImages_;
	VkFormat swapChainImageFormat_;
	VkExtent2D swapChainExtent_;
	std::vector<VkImageView> swapChainImageViews_;
	VkPipelineLayout pipelineLayout_;
	VkRenderPass renderPass_;
	VkPipeline graphicsPipeline_;
	std::vector<VkFramebuffer> swapChainFramebuffers_;
	VkCommandPool commandPool_;
	std::vector<VkCommandBuffer> commandBuffers_;
	VkDescriptorPool descriptorPool_;
	//Vulkan debug members
	VkDebugUtilsMessengerEXT debugMessenger_;
	const std::vector<const char*> validationLayers_ = 
	{
		"VK_LAYER_KHRONOS_validation"
	};
	const std::vector<const char*> deviceExtensions_ = 
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	uint32_t imageCount_ = 0;
#ifdef NEKO_VALIDATION_LAYERS
	const bool enableValidationLayers = true;
    
#else
	const bool enableValidationLayers = false;
#endif
};
}
#endif