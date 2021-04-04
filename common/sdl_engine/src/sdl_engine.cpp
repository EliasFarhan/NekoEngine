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
#include <sstream>
#include <mathematics/vector.h>
#include "sdl_engine/sdl_engine.h"
#include "engine/log.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"

#include <fmt/format.h>

#include "utils/file_utility.h"
#include "engine/window.h"

#ifdef NEKO_GLES3
#include "gl/gles3_window.h"
#endif

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko::sdl
{

void SdlEngine::Init()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Init Sdl Engine");
#endif
    logDebug("Current path: " + GetCurrentPath());
    jobSystem_.Init();

    assert(window_ != nullptr);
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    window_->Init();

    initAction_.Execute();
}

void SdlEngine::Destroy()
{
    BasicEngine::Destroy();

    // Shutdown SDL 2
    SDL_Quit();
}

void SdlEngine::ManageEvent()
{
    
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Manage Event");
#endif
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning_ = false;
        }

        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                logDebug(fmt::format("Windows resized with new size: ({},{})", 
                    event.window.data1, event.window.data2));
                config_.windowSize = Vec2u(event.window.data1, event.window.data2);
                window_->OnResize(config_.windowSize);
            }
        }
        onEventAction_.Execute(event);
    }
}

void SdlEngine::GenerateUiFrame()
{
    window_->GenerateUiFrame();
    BasicEngine::GenerateUiFrame();
}

void SdlEngine::RegisterOnEvent(SdlEventSystemInterface& eventInterface)
{
    onEventAction_.RegisterCallback([&eventInterface](const SDL_Event& event){eventInterface.OnEvent(event);});
}

}
