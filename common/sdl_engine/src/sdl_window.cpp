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

#include <utils/service_locator.h>
#include <engine/log.h>
#include "sdl_engine/sdl_window.h"
#include "engine/engine.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko::sdl
{

void SdlWindow::Init()
{

#ifdef EASY_PROFILE_USE
    EASY_BLOCK("InitSdlWindow");
#endif
    auto* engine = (SdlEngine*)BasicEngine::GetInstance();
    engine->RegisterOnEvent(*this);
    const auto& config = BasicEngine::GetInstance()->GetConfig();


    
#if defined(__ANDROID__)
    //config.fullscreen = true;
    config.windowSize = Vec2u(1280, 720);
    config.fullscreen = true;
#endif
    ;
    auto windowSize = config.windowSize;
    if (config.flags & Configuration::FULLSCREEN)
    {
        windowSize = Vec2u::zero;
        flags_ |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    window_ = SDL_CreateWindow(
        config.windowName.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowSize.x,
        windowSize.y,
        flags_
    );
    if (config.flags & Configuration::FULLSCREEN)
    {
        int windowSizeW = 0;
        int windowSizeH = 0;
        SDL_GetWindowSize(window_, &windowSizeW, &windowSizeH);
        windowSize.x = windowSizeW;
        windowSize.y = windowSizeH;
        //config.windowSize = windowSize;
    }
    // Check that everything worked out okay
    if (window_ == nullptr)
    {
        logError("Unable to create window\n");
        return;
    }
}

void SdlWindow::InitImGui()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("InitSdlImGui");
#endif
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Keyboard Gamepad
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
}

void SdlWindow::Destroy()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("DestroySdlWindow");
#endif
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    // Destroy our window
    SDL_DestroyWindow(window_);


}

void SdlWindow::SwapBuffer()
{

}

void SdlWindow::RenderUi()
{
    ImGui::Render();
}

void SdlWindow::OnEvent(const SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

}