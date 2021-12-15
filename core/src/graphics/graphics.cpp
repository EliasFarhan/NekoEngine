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

#ifdef TRACY_ENABLE
#include "Tracy.hpp"
#endif

namespace neko
{
Renderer::Renderer() :
    renderAllTask_(std::make_shared<Task>([this]
        {
#ifdef TRACY_ENABLE
            ZoneNamedN(rendererUpdate, "Renderer Update", true);
#endif
            auto* engine = BasicEngine::GetInstance();
            BeforeRender();
            PreRender();
            ClearScreen();
            engine->GenerateUiFrame();
            RenderAll();
            window_->RenderUi();
            AfterRender();
        })),
    syncTask_(std::make_shared<Task>([this] { SyncBuffers(); }))
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
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif
    for (auto* renderCommand : currentCommandBuffer_)
    {
        renderCommand->Render();
    }
}

void Renderer::ScheduleTasks() const
{
    auto* engine = BasicEngine::GetInstance();
    engine->ScheduleTask(syncTask_, WorkerQueue::RENDER_QUEUE_NAME);
    engine->ScheduleTask(renderAllTask_, WorkerQueue::RENDER_QUEUE_NAME);
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
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif
    std::swap(currentCommandBuffer_, nextCommandBuffer_);
    nextCommandBuffer_.clear();
    syncBuffersAction_.Execute();

}

void Renderer::PreRender()
{
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif
    using namespace std::chrono_literals;
    microseconds availableLoadingTime(8000);
    bool preRenderTasksEmpty = false;
    {
        std::lock_guard<std::mutex> lock(preRenderTasksMutex_);
        if (preRenderTasks_.empty())
        {
            preRenderTasksEmpty = true;
        }
    }
    while (!preRenderTasksEmpty && availableLoadingTime < 8001us)
    {
        std::chrono::time_point<std::chrono::system_clock> start =
            std::chrono::system_clock::now();


        std::shared_ptr<Task> task = nullptr;
        {
            std::lock_guard lock(preRenderTasksMutex_);
            if (!preRenderTasks_.empty())
            {
                task = preRenderTasks_.front().lock();
                preRenderTasks_.erase(preRenderTasks_.begin());
            }
            else
            {
                preRenderTasksEmpty = true;
            }
        }

        if (task != nullptr && task->CheckDependenciesStarted())
        {
            task->Execute();
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
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif
    std::lock_guard lock(statusMutex_);
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

void Renderer::AddPreRenderTask(std::weak_ptr<Task> task)
{
    std::lock_guard lock(preRenderTasksMutex_);
    preRenderTasks_.push_back(task);
}

void Renderer::ResetTasks() const
{
    syncTask_->Reset();
    renderAllTask_->Reset();
}

std::uint8_t Renderer::GetFlag() const
{
    std::lock_guard<std::mutex> lock(statusMutex_);
    return flags_;
}


}

