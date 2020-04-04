
#include <optional>

#include "engine/assert.h"
#include <set>
#include "engine/engine.h"
#ifdef NEKO_VULKAN

#include "imgui.h"
#include "imgui_impl_sdl.h"

#include <vk/vulkan_window.h>
#include <imgui_impl_vulkan.h>
#include <SDL_vulkan.h>

namespace neko::sdl
{
void VulkanWindow::Init()
{
	SdlWindow::Init();
	CreateInstance();
	SetupDebugMessenger();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapChain();
	CreateImageViews();
	
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

void VulkanWindow::Destroy()
{
	for (auto imageView : swapChainImageViews_) {
		vkDestroyImageView(device_, imageView, nullptr);
	}
	vkDestroySwapchainKHR(device_, swapChain_, nullptr);
	vkDestroySurfaceKHR(instance_, surface_, nullptr);
	vkDestroyDevice(device_, nullptr);
	if(enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
	}
	vkDestroyInstance(instance_, nullptr);
	SdlWindow::Destroy();
}

void VulkanWindow::GenerateUiFrame()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("ImGuiGenerate");
#endif
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame(window_);
	ImGui::NewFrame();
}

void VulkanWindow::OnResize(Vec2u newWindowSize)
{
}

void VulkanWindow::InitImGui()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("ImGuiInit");
#endif
	SdlWindow::InitImGui();
	
	
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "[Vulkan] Validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void VulkanWindow::CreateInstance()
{
	if (enableValidationLayers && !CheckValidationLayerSupport()) {
		neko_assert(false,"[Vulkan] Validation layers requested, but not available!");
	}
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Neko Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = GetRequiredExtensions();
	std::cout << "Vulkan extensions: \n";
	for(auto& extension : extensions)
	{
		std::cout << extension << '\n';
	}
	createInfo.enabledLayerCount = 0;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance_);
	if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
		neko_assert(false, "[Vulkan] Failed to create instance!");
	}
}

std::vector<const char*> VulkanWindow::GetRequiredExtensions()
{
	unsigned int count;
	// get count of required extensions
	if (!SDL_Vulkan_GetInstanceExtensions(NULL, &count, NULL))
	{
		neko_assert(false, "[Vulkan] Cannot get required extensions count");
	}
	static const char* const additionalExtensions[] =
	{
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME, // example additional extension
#ifdef NEKO_VALIDATION_LAYERS
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME //adding validation layers
#endif
	};
	size_t additionalExtensionsCount = sizeof(additionalExtensions) / sizeof(additionalExtensions[0]);
	size_t extensionCount = count + additionalExtensionsCount;
	std::vector<const char*> names(extensionCount);

	// get names of required extensions
	if (!SDL_Vulkan_GetInstanceExtensions(NULL, &count, &names[0]))
	{
		neko_assert(false, "[Vulkan] Cannot get instance extensions");
	}

	// copy additional extensions after required extensions
	for (size_t i = 0; i < additionalExtensionsCount; i++)
	{
		names[i + count] = additionalExtensions[i];
	}
	SDL_Vulkan_GetInstanceExtensions(window_, &count, &names[0]);
	return names;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VulkanWindow::SetupDebugMessenger()
{
	if(!enableValidationLayers)
		return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;
	createInfo.pUserData = nullptr; // Optional

	if (CreateDebugUtilsMessengerEXT(instance_, &createInfo, nullptr, &debugMessenger_) != VK_SUCCESS) {
		neko_assert(false, "[Vulkan] Failed to set up debug messenger!");
	}
}

void VulkanWindow::PickPhysicalDevice()
{
	
	std::uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
	if(deviceCount == 0)
	{
		neko_assert(false, "[Vulkan] Failed to find GPUs with Vulkan support!");
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());
	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			physicalDevice_ = device;
			break;
		}
	}

	if (physicalDevice_ == VK_NULL_HANDLE) {
		neko_assert(false,"[Vulkan] Failed to find a suitable GPU!");
	}
}

void VulkanWindow::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice_);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<std::uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions_.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions_.data();

	if (enableValidationLayers) 
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers_.size());
		createInfo.ppEnabledLayerNames = validationLayers_.data();
	}
	else 
	{
		createInfo.enabledLayerCount = 0;
	}
	if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_) != VK_SUCCESS) {
		neko_assert(false,"[Vulkan] Failed to create logical device!");
	}
	vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
	vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);
}

void VulkanWindow::CreateSurface()
{
	if(!SDL_Vulkan_CreateSurface(window_, instance_, &surface_))
	{
		neko_assert(false, "[Vulkan] Failed to create a window surface!");
	}
}

void VulkanWindow::CreateSwapChain()
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice_);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	std::uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; // recommended not to wait for driver to complete internal operations
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface_;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice_);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; //To simplify the ownership 
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swapChain_) != VK_SUCCESS) {
		neko_assert(false,"[Vulkan] Failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, nullptr);
	swapChainImages_.resize(imageCount);
	vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, swapChainImages_.data());
	swapChainImageFormat_ = surfaceFormat.format;
	swapChainExtent_ = extent;
}

bool VulkanWindow::CheckValidationLayerSupport() const
{
	std::uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	std::cout << "Available layers: \n";
	for(auto& layerProperties : availableLayers)
	{
		std::cout << layerProperties.layerName << std::endl;
	}
	
	for (const char* layerName : validationLayers_) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}



QueueFamilyIndices VulkanWindow::FindQueueFamilies(VkPhysicalDevice device) const
{

	// Assign index to queue families that could be found
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) 
	{
		if(indices.IsComplete())
			break;
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &presentSupport);
		if (presentSupport) 
		{
			indices.presentFamily = i;
		}
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) //Checking if available queue family supports graphics command.
		{
			indices.graphicsFamily = i;
		}
		i++;
	}
	return indices;
}

bool VulkanWindow::CheckDeviceExtensionSupport(VkPhysicalDevice device) const
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions_.begin(), deviceExtensions_.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

SwapChainSupportDetails VulkanWindow::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, details.presentModes.data());
	}
	return details;
}

VkSurfaceFormatKHR VulkanWindow::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats.front();
}

VkPresentModeKHR VulkanWindow::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	const auto& config = BasicEngine::GetInstance()->config;
	if (config.vSync)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanWindow::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	const auto& config = BasicEngine::GetInstance()->config;
	
	if (capabilities.currentExtent.width != UINT32_MAX) 
	{
		return capabilities.currentExtent;
	}
	else 
	{
		VkExtent2D actualExtent = { config.windowSize.x, config.windowSize.y };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

bool VulkanWindow::IsDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	bool extensionsSupported = CheckDeviceExtensionSupport(device);
	auto indices = FindQueueFamilies(device);
	bool swapChainAdequate = false;
	if (extensionsSupported) 
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	//We only choose discrete GPU
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && indices.IsComplete() && extensionsSupported && swapChainAdequate;
}

void VulkanWindow::CreateImageViews()
{
	swapChainImageViews_.resize(swapChainImages_.size());
	for (size_t i = 0; i < swapChainImages_.size(); i++) 
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages_[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat_;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(device_, &createInfo, nullptr, &swapChainImageViews_[i])
			!= VK_SUCCESS) 
		{
			neko_assert(false,"failed to create image views!");
		}
	}
}

void VulkanWindow::CreateGraphicsPipeline()
{
}
}
#endif