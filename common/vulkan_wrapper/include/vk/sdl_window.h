#pragma once

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

#if defined(NEKO_SDL2)

#include <vk/vk_driver.h>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include "sdl_engine/sdl_window.h"
#include "vk_mem_alloc.h"
#include "vk_imgui.h"

namespace neko::vk
{
class VkWindow : public sdl::SdlWindow
{
public:
    VkWindow();
    void Init() override;
    void Destroy() override;
    void GenerateUiFrame() override;
    void OnResize(Vec2u newWindowSize) override;
    VkDriver& GetDriver() {return driver_;}
    VkSwapchain& GetSwapchain(){return vkSwapchain_;}
    VmaAllocator GetAllocator(){return allocator_;}
    SDL_Window* GetWindow() { return window_; }

    void Update(seconds dt) override;

    void OnEvent(const SDL_Event &event) override;


    VkCommandBuffer BeginSingleTimeCommands(VkCommandPool commandPool);
    void EndSingleTimeCommands(VkCommandPool commandPool, VkCommandBuffer commandBuffer);

    VkShaderModule CreateShaderModule(const BufferFile& file);

protected:

    void InitImGui() override;
    void SwapBuffer() override;

    void RenderUi() override;

private:
    VkDriver driver_;
    VkSwapchain vkSwapchain_;
    bool enableValidationLayers_;
    VkDebugUtilsMessengerEXT debugMessenger_;
    VmaAllocator allocator_;
    VkImGUI vkImgui_;

    void CreateInstance();
    void SetupDebugMessenger();
    void CreateLogicalDevice();
    void CreateSurface();
    void CreateSwapChain();
    void CreateImageViews();

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

};

}

#endif