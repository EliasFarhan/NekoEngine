#pragma once

#include <optional>
#include <vector>
#include <array>
#include <vulkan/vulkan.h>

#include "engine/filesystem.h"

namespace neko::vk
{


struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool IsComplete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


bool CheckValidationLayerSupport();

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator);

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

int RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

VkPhysicalDevice PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

VkShaderModule CreateShaderModule(const neko::BufferFile& bufferFile, VkDevice device);


constexpr std::array<const char*, 1> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

constexpr std::array<const char*, 1> deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
}