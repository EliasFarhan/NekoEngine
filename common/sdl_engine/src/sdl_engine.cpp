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

    assert(window_ != nullptr);
    SDL_Init(SDL_INIT_VIDEO);
    window_->Init();
    window_->InitImGui();
    initAction_.Execute();
}

void SdlEngine::Update([[maybe_unused]]seconds dt)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        window_->OnEvent(event);
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
    window_->ClearScreen();
    drawAction_.Execute();

    // Start the Dear ImGui frame
    window_->ImguiNewFrame();
    ImGui::NewFrame();

    drawUiAction_.Execute(dt);
    window_->ImguiRender();

    window_->FinishFrame();
}

void SdlEngine::Destroy()
{
    destroyAction_.Execute();
    window_->Destroy();

    BasicEngine::Destroy();
    // Shutdown SDL 2
    SDL_Quit();
}

void SdlEngine::SetWindow(SdlWindow* window)
{
    window_ = window;
}

void SdlEngine::SetRenderer(Renderer* renderer)
{
    renderer_ = renderer;
}
}
