/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <graphics/graphics.h>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <engine/globals.h>
#include <engine/window.h>
#include <engine/engine.h>
#include "engine/log.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko
{
Renderer::Renderer()
{
    currentCommandBuffer_.reserve(MAX_COMMAND_NMB);
    nextCommandBuffer_.reserve(MAX_COMMAND_NMB);
}


void Renderer::Render(RenderCommandInterface* command)
{
    nextCommandBuffer_.push_back(command);
}

void Renderer::RenderAll()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("RenderAllCPU");
#endif
    for (auto* renderCommand : currentCommandBuffer_)
    {
        renderCommand->Render();
    }
}

void Renderer::Sync()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("EngineRenderSync");
#endif

    std::unique_lock lock(renderMutex_);
    flags_ |= IS_APP_WAITING;
#ifndef EMSCRIPTEN
    cv_.wait(lock);
#endif
    std::swap(currentCommandBuffer_, nextCommandBuffer_);
    nextCommandBuffer_.clear();
    flags_ &= ~IS_APP_WAITING;
    //TODO copy all the new transform3d?
}

void Renderer::RenderLoop()
{
#ifndef EMSCRIPTEN
    flags_ |= IS_RUNNING;
    window_->LeaveCurrentContext();
    renderThread_ = std::thread([this]{
        BeforeRenderLoop();

        std::chrono::time_point<std::chrono::system_clock> clock = std::chrono::system_clock::now();
        while (flags_ & IS_RUNNING)
        {
            const auto start = std::chrono::system_clock::now();
            const auto dt = std::chrono::duration_cast<seconds>(start - clock);
            dt_ = dt.count();
        	clock = start;
        	
           Update();
        }
        AfterRenderLoop();
    });
#endif
}

void Renderer::Destroy()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("ClosingFromEngine");
#endif
    flags_ &= ~IS_RUNNING;
    renderThread_.join();
}

void Renderer::SetFlag(Renderer::RendererFlag flag)
{
    flags_ |= flag;
}

void Renderer::SetWindow(Window* window)
{
    window_ = window;
}

void Renderer::Update()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("RenderFullUpdateCPU");
#endif
    size_t countWait = 0;
    while((flags_ & IS_APP_WAITING) && (flags_ & IS_RUNNING))
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);

        cv_.notify_one();
        countWait++;
    }
    {
        std::ostringstream oss;
        oss << "Count Wait: "<<countWait<<'\n';
        logDebug(oss.str());
    }

    auto* engine = BasicEngine::GetInstance();
    {
        std::lock_guard<std::mutex> lock(renderMutex_);
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("RenderUpdateCPU");
#endif
        window_->MakeCurrentContext();
        ClearScreen();
        engine->GenerateUiFrame();
        RenderAll();
        window_->RenderUi();

        window_->SwapBuffer();

        window_->LeaveCurrentContext();
    }




}
void Renderer::BeforeRenderLoop()
{
    window_->MakeCurrentContext();
}
void Renderer::AfterRenderLoop()
{
    window_->LeaveCurrentContext();
}



}

