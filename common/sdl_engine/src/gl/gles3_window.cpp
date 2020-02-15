//
// Created by efarhan on 25.01.20.
//

#ifdef NEKO_GLES3

#include "gl/gles3_window.h"

#include <sstream>
#include <engine/engine.h>
#include "engine/log.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

namespace neko::sdl
{

void Gles3Window::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    // Set our OpenGL version.
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

    SDL_GL_SetSwapInterval(config.vSync);
    SdlWindow::Init();
    glContext_ = SDL_GL_CreateContext(window_);
    MakeCurrentContext();
#ifndef __EMSCRIPTEN__
    if (!gladLoadGLES2Loader((GLADloadproc) SDL_GL_GetProcAddress))
    {
        logDebug("Failed to initialize OpenGL context\n");
        assert(false);
    }
#endif

    InitImGui();
    glEnable(GL_DEPTH_TEST);
}

void Gles3Window::InitImGui()
{

    SdlWindow::InitImGui();
    ImGui_ImplSDL2_InitForOpenGL(window_, glContext_);
    ImGui_ImplOpenGL3_Init("#version 300 es");

}


void Gles3Window::GenerateUiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window_);
    ImGui::NewFrame();
}


void Gles3Window::SwapBuffer()
{
    SDL_GL_SwapWindow(window_);
}

void Gles3Window::Destroy()
{
    MakeCurrentContext();
    ImGui_ImplOpenGL3_Shutdown();
    // Delete our OpengL context
    SDL_GL_DeleteContext(glContext_);

    SdlWindow::Destroy();
}

void Gles3Window::RenderUi()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Gles3Window::OnResize(Vec2u newWindowSize)
{
    glViewport(0, 0, newWindowSize.x, newWindowSize.y);
}

void Gles3Window::MakeCurrentContext()
{
    SDL_GL_MakeCurrent(window_, glContext_);
}

void Gles3Window::LeaveCurrentContext()
{
    SDL_GL_MakeCurrent(nullptr, nullptr);
}
}

#endif