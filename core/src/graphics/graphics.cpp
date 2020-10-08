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
#include <graphics/graphics.h>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <engine/globals.h>
#include <engine/window.h>
#include <engine/engine.h>
#include "engine/log.h"
#include "engine/component.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko
{
Renderer::Renderer() :
    renderAllJob_([this]
        {
            auto* engine = BasicEngine::GetInstance();
            PreRender();
            ClearScreen();
            engine->GenerateUiFrame();
            RenderAll();
            window_->RenderUi();
        }),
    syncJob_([this] { SyncBuffers(); })
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

void Renderer::BeforeRenderLoop()
{
    window_->BeforeRenderLoop();
}

void Renderer::AfterRenderLoop()
{
    window_->AfterRenderLoop();
}

void Renderer::ScheduleJobs()
{
    auto* engine = BasicEngine::GetInstance();
    engine->ScheduleJob(&syncJob_, JobThreadType::RENDER_THREAD);
    engine->ScheduleJob(&renderAllJob_, JobThreadType::RENDER_THREAD);
}

void Renderer::RegisterSyncBuffersFunction(SyncBuffersInterface* syncBuffersInterface)
{
    syncBuffersAction_.RegisterCallback([syncBuffersInterface]
        {
            syncBuffersInterface->SyncBuffers();
        });
}

void Renderer::SyncBuffers()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Swapping Render Command");
#endif
    std::swap(currentCommandBuffer_, nextCommandBuffer_);
    nextCommandBuffer_.clear();
    syncBuffersAction_.Execute();

}

void Renderer::PreRender()
{
    using namespace std::chrono_literals;
    microseconds availableLoadingTime(8000);
    while (availableLoadingTime < 8001us)
    {
        std::chrono::time_point<std::chrono::system_clock> start =
            std::chrono::system_clock::now();


        Job* job = nullptr;
        {
            std::lock_guard<std::mutex> lock(preRenderJobsMutex_);
            if (!preRenderJobs_.empty())
            {
                job = preRenderJobs_.front();
                preRenderJobs_.erase(preRenderJobs_.begin());
            }
        }

        if (job != nullptr && job->CheckDependenciesStarted())
        {
            job->Execute();
        }
        else
        {
            break;
        }

        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        const auto duration = std::chrono::duration_cast<microseconds>(end - start);
        availableLoadingTime -= duration;
    }
}

void Renderer::Destroy()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("ClosingFromEngine");
#endif

    std::lock_guard<std::mutex> lock(statusMutex_);
    flags_ &= ~IS_RUNNING;
}

void Renderer::SetFlag(Renderer::RendererFlag flag)
{
    std::lock_guard<std::mutex> lock(statusMutex_);
    flags_ |= flag;
}

void Renderer::SetWindow(Window* window)
{
    window_ = window;
}

void Renderer::AddPreRenderJob(Job* job)
{
    std::lock_guard<std::mutex> lock(preRenderJobsMutex_);
    preRenderJobs_.push_back(job);
}

void Renderer::ResetJobs()
{
    syncJob_.Reset();
    renderAllJob_.Reset();
}

std::uint8_t Renderer::GetFlag() const
{
    std::lock_guard<std::mutex> lock(statusMutex_);
    return flags_;
}


}

