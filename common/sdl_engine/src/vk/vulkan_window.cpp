
#include <optional>

#include "engine/assert.h"
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
	PickPhysicalDevice();
	CreateLogicalDevice();
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

void VulkanWindow::Destroy()
{
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

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1; float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;

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
}

bool VulkanWindow::CheckValidationLayerSupport() const
{
	std::uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

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



QueueFamilyIndices VulkanWindow::FindQueueFamilies(VkPhysicalDevice device)
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
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) //Checking if available queue family supports graphics command.
		{
			indices.graphicsFamily = i;
		}
		i++;
	}
	return indices;
}

bool VulkanWindow::IsDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	auto indices = FindQueueFamilies(device);
	//We only choose discrete GPU
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && indices.IsComplete();
}
}
#endif