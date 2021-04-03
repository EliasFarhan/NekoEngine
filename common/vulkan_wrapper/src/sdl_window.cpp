
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <set>

#include "engine/assert.h"
#include "engine/log.h"
#include "vk/vk_utility.h"

#if defined(NEKO_SDL2)

#include <SDL2/SDL_vulkan.h>
#include <vk/sdl_window.h>

namespace neko::vk
{
VkWindow::VkWindow()
{
    flags_ = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;
#if defined(NEKO_ASSERT)
    enableValidationLayers_ = true;
#else
    enableValidationLayers_ = false;
#endif
}

void VkWindow::Init()
{
    SdlWindow::Init();
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface();
    driver_.physicalDevice_ = PickPhysicalDevice(driver_.instance_, driver_.surface_);
    CreateLogicalDevice();
    CreateSwapChain();
    CreateImageViews();
}

void VkWindow::Destroy()
{
    vkDestroyDevice(driver_.device_, nullptr);
    if (enableValidationLayers_)
    {
        DestroyDebugUtilsMessengerEXT(driver_.instance_, debugMessenger_, nullptr);
    }
    vkDestroySurfaceKHR(driver_.instance_, driver_.surface_, nullptr);

    vkDestroyInstance(driver_.instance_, nullptr);
    SdlWindow::Destroy();
}

void VkWindow::GenerateUiFrame()
{
}

void VkWindow::OnResize(Vec2u newWindowSize)
{
}

void VkWindow::CreateInstance()
{
    std::cout << "[Log] Creating Instance\n";
    if (enableValidationLayers_ && !CheckValidationLayerSupport())
    {
        std::cerr << "[Error] Validation layers requested, but not available!\n";
        assert(false);
    }
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    unsigned int count;
    if (!SDL_Vulkan_GetInstanceExtensions(nullptr, &count, nullptr))
    {
        std::cerr << "[Error] SDL Vulkan, Could not get extensions count\n";
    }

    const char* const additionalExtensions[] =
    {
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME, // example additional extension
#ifndef NDEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME //adding validation layers
#endif
    };
    const size_t additionalExtensionsCount = sizeof(additionalExtensions) / sizeof(additionalExtensions[0]);
    const size_t extensionCount = count + additionalExtensionsCount;
    std::vector<const char*> extensionNames(extensionCount);

    // get names of required extensions
    if (!SDL_Vulkan_GetInstanceExtensions(NULL, &count, &extensionNames[0]))
    {
        logDebug( "[Error] SDL Vulkan, Cannot get instance extensions");
        neko_assert(false, "");
    }

    // copy additional extensions after required extensions
    for (size_t i = 0; i < additionalExtensionsCount; i++)
    {
        extensionNames[i + count] = additionalExtensions[i];
    }
    SDL_Vulkan_GetInstanceExtensions(window_, &count, &extensionNames[0]);

    logDebug( "Vulkan extensions: \n");
    for (auto& extension : extensionNames)
    {
        logDebug(fmt::format("{}",extension));
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size());
    createInfo.ppEnabledExtensionNames = extensionNames.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers_)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &driver_.instance_) != VK_SUCCESS)
    {
        logDebug("[Vulkan] Failed to create instance!\n");
        neko_assert(false, "");
    }
}

void VkWindow::SetupDebugMessenger()
{
    if (!enableValidationLayers_)
    {
        return;
    }
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);
    if (CreateDebugUtilsMessengerEXT(driver_.instance_, &createInfo, nullptr, &debugMessenger_) != VK_SUCCESS)
    {
        logDebug("[Error] failed to set up debug messenger!");
        neko_assert(false, "");
    }
}

void VkWindow::CreateLogicalDevice()
{
    std::cout << "[Log] Creating Logical Device\n";
    QueueFamilyIndices indices = FindQueueFamilies(driver_.physicalDevice_, driver_.surface_);


    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }


    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size());

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<std::uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers_)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(driver_.physicalDevice_, &createInfo, nullptr, &driver_.device_) != VK_SUCCESS)
    {
        std::cerr << ("[Error] Failed to create logical device!");
        assert(false);
    }
    vkGetDeviceQueue(driver_.device_, indices.graphicsFamily.value(), 0, &driver_.graphicsQueue_);
    vkGetDeviceQueue(driver_.device_, indices.presentFamily.value(), 0, &driver_.presentQueue_);


}

void VkWindow::CreateSurface()
{
    logDebug("[Log] Creating Surface");
    if (!SDL_Vulkan_CreateSurface(window_, driver_.instance_, &driver_.surface_))
    {
        logDebug("[Vulkan] Failed to create a window surface!");
        neko_assert(false, "");
    }
}

void VkWindow::CreateSwapChain()
{
    std::cout << "[Vulkan] Create SwapChain\n";
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(driver_.physicalDevice_, driver_.surface_);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);
    std::cout << "[Vulkan] Swapchain support, minImageCount: " << swapChainSupport.capabilities.minImageCount <<
        " maxImageCount: " << swapChainSupport.capabilities.maxImageCount << '\n';
    std::uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    std::cout << "[Vulkan] Image count: " << imageCount << '\n';

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = driver_.surface_;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = FindQueueFamilies(driver_.physicalDevice_, driver_.surface_);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(driver_.device_, &createInfo, nullptr, &vkSwapchain_.swapChain_) != VK_SUCCESS)
    {
        std::cerr << "[Error] failed to create swap chain!\n";
        assert(false);
    }
    //Adding images
    vkGetSwapchainImagesKHR(driver_.device_, vkSwapchain_.swapChain_, &imageCount, nullptr);
    vkSwapchain_.swapChainImages_.resize(imageCount);
    vkGetSwapchainImagesKHR(driver_.device_, vkSwapchain_.swapChain_, &imageCount, vkSwapchain_.swapChainImages_.data());

    vkSwapchain_.swapChainImageFormat_ = surfaceFormat.format;
    vkSwapchain_.swapChainExtent_ = extent;
}

void VkWindow::CreateImageViews()
{
    vkSwapchain_.swapChainImageViews_.resize(vkSwapchain_.swapChainImages_.size());
    for (size_t i = 0; i < vkSwapchain_.swapChainImages_.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = vkSwapchain_.swapChainImages_[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = vkSwapchain_.swapChainImageFormat_;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(driver_.device_, &createInfo, nullptr, &vkSwapchain_.swapChainImageViews_[i]) != VK_SUCCESS)
        {
            logDebug ("[Error] Failed to create image views!");
            neko_assert(false, "");
        }
    }
}

VkExtent2D VkWindow::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    int width, height;
    SDL_GetWindowSize(window_, &width, &height);

    VkExtent2D actualExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;

}
}

#endif