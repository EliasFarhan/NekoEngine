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


#include <chrono>
#include <sstream>

#include <engine/engine.h>

#include "imgui.h"

#include "engine/filesystem.h"
#include <engine/log.h>
#include <utils/file_utility.h>
#include "graphics/graphics.h"
#include <engine/window.h>
#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko
{
BasicEngine* BasicEngine::instance_ = nullptr;

BasicEngine::BasicEngine(const FilesystemInterface& filesystem, std::optional<Configuration> config) :
    filesystem_(filesystem)
{
    instance_ = this;
    if (config.has_value())
    {
        config_ = config.value();
    }

    workerManagerPb_ = WorkerManager::CreateWorkerManagerDefinition();

#ifdef EASY_PROFILE_USE
    EASY_PROFILER_ENABLE;
#endif
}

BasicEngine::~BasicEngine()
{
    logDebug("Destroy Basic Engine");

#ifdef EASY_PROFILE_USE
    profiler::dumpBlocksToFile("Neko_Profile.prof");
#endif
}
#ifdef __ANDROID__

#include <sys/stat.h>

extern "C"
JNIEXPORT void JNICALL
Java_swiss_sae_gpr5300_MainActivity_finalize([[maybe_unused]] JNIEnv * env, [[maybe_unused]] jclass clazz, [[maybe_unused]] jstring directory)
{

#ifdef EASY_PROFILE_USE
    if (env == nullptr)
    {
        logDebug("[Error] Android environment is null");
        return;
    }

    std::string path = env->GetStringUTFChars(directory, nullptr);

    path += "/Neko_Profile.prof";
    logDebug("Android data profile data path: " + path);
    auto blockNumber = profiler::dumpBlocksToFile(path.c_str());
    if (blockNumber == 0)
    {
        logDebug("[Error] Could not save profile data");
    }
    else
    {
        logDebug("Easy Profile with several blocks: " + std::to_string(blockNumber));
    }
#endif
}
#endif

void BasicEngine::Init()
{

#ifdef EASY_PROFILE_USE
    EASY_FUNCTION(profiler::colors::Magenta);
#endif
    logDebug(fmt::format("Current path: {}", GetCurrentPath()));
    workerManager_.Init(workerManagerPb_);
    initAction_.Execute();
}

void BasicEngine::Update(seconds dt)
{
    dt_ = dt.count();
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Main Thread Update");
#endif
    if (renderer_)
        renderer_->ResetTasks();
    if (window_)
        window_->ResetTasks();

    const auto eventTask = std::make_shared<Task>([this]
        {
            ManageEvent();
        });
    const auto swapBufferTask = window_->GetSwapBufferTask().lock();
    const auto updateTask = std::make_shared<Task>([this, &dt] { updateAction_.Execute(dt); });
    updateTask->AddDependency(eventTask);
    if (renderer_)
    {
        const auto rendererSyncTask = renderer_->GetSyncTask().lock();
        updateTask->AddDependency(rendererSyncTask);

        const auto renderTask = renderer_->GetRenderAllTask().lock();
        renderTask->AddDependency(eventTask);

        
        swapBufferTask->AddDependency(renderTask);
        //swapBufferTask->AddDependency(&updateTask);

        renderer_->ScheduleTasks();
        workerManager_.AddTask(swapBufferTask, WorkerQueue::RENDER_QUEUE_NAME);


    }
    workerManager_.AddTask(eventTask, WorkerQueue::MAIN_QUEUE_NAME);
    workerManager_.AddTask(updateTask, WorkerQueue::MAIN_QUEUE_NAME);
    workerManager_.ExecuteMainThread();
#ifdef EASY_PROFILE_USE
    EASY_END_BLOCK
    EASY_BLOCK("Waiting for Swap Buffer");
#endif
    if (swapBufferTask)
        swapBufferTask->Join();
}

void BasicEngine::Destroy()
{
    destroyAction_.Execute();
    if (renderer_)
    {
        renderer_->Destroy();
    }
    if (window_)
    {
        window_->Destroy();
    }
    workerManager_.Destroy();
    instance_ = nullptr;
}

static std::chrono::time_point<std::chrono::system_clock> clock;


void BasicEngine::EngineLoop()
{
    isRunning_ = true;
    clock = std::chrono::system_clock::now();

    while (isRunning_)
    {
        const auto start = std::chrono::system_clock::now();
        const auto dt = std::chrono::duration_cast<seconds>(start - clock);
        clock = start;
        Update(dt);
    }
    Destroy();
}

void BasicEngine::SetWindowAndRenderer(Window* window, Renderer* renderer)
{
    window_ = window;
    renderer_ = renderer;
    renderer_->SetWindow(window);
    RendererLocator::provide(renderer);
}

void BasicEngine::GenerateUiFrame()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Generate ImGui Frame");
#endif
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::Begin("Neko Window");

    const auto fpsText = fmt::format("App FPS: {}", 1.0f / dt_);
    ImGui::Text("%s", fpsText.c_str());
    ImGui::End();
    drawImGuiAction_.Execute();
}

void BasicEngine::RegisterSystem(SystemInterface& system)
{
    initAction_.RegisterCallback([&system] { system.Init(); });
    updateAction_.RegisterCallback([&system](seconds dt) { system.Update(dt); });
    destroyAction_.RegisterCallback([&system] { system.Destroy(); });
}

void BasicEngine::RegisterOnDrawUi(DrawImGuiInterface& drawUi)
{
    drawImGuiAction_.RegisterCallback([&drawUi] { drawUi.DrawImGui(); });
}

void BasicEngine::ScheduleTask(const std::shared_ptr<Task>& task, std::string_view queueName)
{
    workerManager_.AddTask(task, queueName);
}

const Configuration& BasicEngine::GetConfig()
{
    return config_;
}

const FilesystemInterface& BasicEngine::GetFilesystem() const
{
    return filesystem_;
}


}
