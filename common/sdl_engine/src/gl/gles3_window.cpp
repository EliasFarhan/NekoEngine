//
// Created by efarhan on 25.01.20.
//

#ifdef NEKO_GLES3

#include "gl/gles3_window.h"

#include <sstream>
#include <engine/engine.h>
#include "engine/log.h"
#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
namespace neko::sdl
{
void SetupGlAttributes()
{


}
void InitContext()
{
    if (!gladLoadGLES2Loader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::stringstream ss;
        ss << "[Error] Failed to initialize OpenGL context" << std::endl;
        logDebug(ss.str());
        return;
    }
}
void InitGlImGui()
{
    // Setup Platform/Renderer bindings

}

void Gles3Window::Init()
{
    // Set our OpenGL version.
// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);


    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SdlWindow::Init();
    glContext_ = SDL_GL_CreateContext(window_);

}

void Gles3Window::InitImGui()
{
    const auto& config = BasicEngine::GetInstance()->config;

    SdlWindow::InitImGui();
    ImGui_ImplSDL2_InitForOpenGL(window_, glContext_);
    ImGui_ImplOpenGL3_Init("#version 300 gles");

    SDL_GL_SetSwapInterval(config.vSync);
}

void Gles3Window::OnEvent(const SDL_Event& event)
{
    SdlWindow::OnEvent(event);
    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            Vec2f newWindowSize = Vec2f(event.window.data1, event.window.data2);
            glViewport(0, 0, newWindowSize.x, newWindowSize.y);
        }
    }
}
}

#endif