#pragma once
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

#include <engine/system.h>
#include <utils/action_utility.h>
#include <utils/time_utility.h>
#include "mathematics/vector.h"

#include "worker_system.h"
#include "engine/filesystem.h"
#include "proto/config.pb.h"


namespace neko
{
class Renderer;
class Window;



/**
 * \brief basic engine class with no graphics manager implementation
 */
class BasicEngine : public SystemInterface
{
public:
    explicit BasicEngine(const FilesystemInterface&);
	BasicEngine() = delete;
    ~BasicEngine() override;
    void Init() override;
    void Update(seconds dt) final;
    void Destroy() override;

    //Update functions
    virtual void ManageEvent() = 0;
    virtual void GenerateUiFrame();

    void EngineLoop();

    void SetWindowAndRenderer(Window* window, Renderer* renderer);
    
    const FilesystemInterface& GetFilesystem() const;

    void RegisterSystem(SystemInterface& system);
    void RegisterOnDrawUi(DrawImGuiInterface& drawUi);

    [[nodiscard]] float GetDeltaTime() const { return dt_; }
    [[nodiscard]] Renderer* GetRenderer() const { return renderer_; }

    static BasicEngine* GetInstance(){return instance_;}

    void ScheduleTask(const std::shared_ptr<Task>& task, std::string_view queueName);

    [[nodiscard]] std::uint8_t GetWorkersNumber() const { return workerManager_.GetWorkersCount(); }


    [[nodiscard]] Vec2u GetWindowSize() const;
    void SetWindowSize(Vec2u windowSize) const;
    [[nodiscard]] const core::pb::Config& GetConfig() const { return config_; }
    [[nodiscard]] core::pb::Config& MutableConfig() { return config_; }
    [[nodiscard]] static core::pb::Config GenerateDefaultConfig();
protected:
    static BasicEngine* instance_;
    const FilesystemInterface& filesystem_;
    core::pb::Config config_;
    Renderer* renderer_ = nullptr;
    Window* window_ = nullptr;
    WorkerManager workerManager_;
	bool isRunning_ = false;
    float dt_ = 0.0f;

    core::pb::WorkerManager workerManagerPb_;

    Action<> initAction_;
    Action<seconds> updateAction_;
    Action<> drawImGuiAction_;
    Action<> destroyAction_;

};

}
