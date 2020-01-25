/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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
#include <sstream>
#include <mathematics/vector.h>
#include "sdl_engine/sdl_engine.h"
#include "engine/log.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"

#ifdef NEKO_GLES3

#include "gl/gles3_window.h"

#endif

namespace neko::sdl
{
SdlEngine::SdlEngine(Configuration* config) : BasicEngine(config)
{
}

void SdlEngine::Init()
{
    BasicEngine::Init();

    assert(window != nullptr);
    window->Init();
    window->InitImGui();
    initAction_.Execute();
}

void SdlEngine::Update([[maybe_unused]]seconds dt)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        window->OnEvent(event);
        OnEvent(event);
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
        {
            isRunning_ = false;
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
                default:
                    break;
            }
        }
    }

    updateAction_.Execute(dt);
#ifdef NEKO_GLES3
    SDL_GL_MakeCurrent(window_, glContext_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawDelegate_.Execute();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window_);
    ImGui::NewFrame();

    drawUiAction_.Execute(dt);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window_);
#endif
}

void SdlEngine::Destroy()
{
    destroyDelegate_.Execute();
#ifdef NEKO_GLES3
    ImGui_ImplOpenGL3_Shutdown();
#endif
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
#ifdef NEKO_GLES3
    // Delete our OpengL context
    SDL_GL_DeleteContext(glContext_);
#endif
    // Destroy our window
    SDL_DestroyWindow(window_);

    // Shutdown SDL 2
    SDL_Quit();

}

void SdlEngine::SetWindow(SdlWindow* window)
{
    SdlEngine::window = window;
}
}
