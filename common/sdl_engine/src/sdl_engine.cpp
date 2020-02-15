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
    initAction_.Execute();
}

void SdlEngine::Destroy()
{
    BasicEngine::Destroy();
    destroyAction_.Execute();
    window_->Destroy();

    // Shutdown SDL 2
    SDL_Quit();
}

void SdlEngine::ManageEvent()
{
    std::lock_guard<std::mutex> lock(renderer_->GetRenderMutex());
    window_->MakeCurrentContext();
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
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

        auto& config = BasicEngine::GetInstance()->config;
        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                config.windowSize = Vec2i(event.window.data1, event.window.data2);
                window_->OnResize(Vec2u(event.window.data1, event.window.data2));
            }
        }
    }
    window_->LeaveCurrentContext();
}

void SdlEngine::GenerateUiFrame()
{
    window_->GenerateUiFrame();
    BasicEngine::GenerateUiFrame();
}

}
