//
// Created by efarhan on 25.01.20.
//
#include <sstream>

#include <utilities/service_locator.h>
#include <engine/log.h>
#include "sdl_engine/sdl_window.h"
#include "engine/engine.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko
{

void sdl::SdlWindow::Init()
{

#ifdef EASY_PROFILE_USE
    EASY_BLOCK("InitSdlWindow");
#endif
    auto& config = BasicEngine::GetInstance()->config;

    auto flags = SDL_WINDOW_RESIZABLE|
#ifdef NEKO_GLES3
            SDL_WINDOW_OPENGL
#endif
                    ;
    auto windowSize = config.windowSize;
    if (config.fullscreen)
    {
        int windowSizeW = 0;
        int windowSizeH = 0;
        SDL_GetWindowSize(window_, &windowSizeW, &windowSizeH);
        windowSize.x = windowSizeW;
        windowSize.y = windowSizeH;
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    window_ = SDL_CreateWindow(
            config.windowName.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowSize.x,
            windowSize.y,
            flags
    );
    // Check that everything worked out okay
    if (window_ == nullptr)
    {
        LogDebug("[Error] Unable to create window\n");
        return;
    }
}

void sdl::SdlWindow::InitImGui()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("InitSdlImGui");
#endif
// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;
	
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Keyboard Gamepad

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
}


void sdl::SdlWindow::ImguiNewFrame()
{
    ImGui_ImplSDL2_NewFrame(window_);
    ImGui::NewFrame();
}


void sdl::SdlWindow::Destroy()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("DestroySdlWindow");
#endif
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    // Destroy our window
    SDL_DestroyWindow(window_);


}

void sdl::SdlWindow::SwapBuffer()
{

}

void sdl::SdlWindow::RenderUi()
{
    ImGui::Render();
}

}