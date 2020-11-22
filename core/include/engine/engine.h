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
#include <mutex>
#include <string>
#include <optional>

#include <engine/system.h>
#include <utils/action_utility.h>
#include <graphics/color.h>
#include <utils/time_utility.h>
#include <engine/configuration.h>
#include <engine/jobsystem.h>
#include "engine/filesystem.h"


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
    explicit BasicEngine(const FilesystemInterface&,std::optional<Configuration> config = std::nullopt);
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

    const Configuration& GetConfig();
    const FilesystemInterface& GetFilesystem();

    void RegisterSystem(SystemInterface& system);
    void RegisterOnDrawUi(DrawImGuiInterface& drawUi);

    float GetDeltaTime() const { return dt_; };

    static BasicEngine* GetInstance(){return instance_;}

    void ScheduleJob(Job* job, JobThreadType threadType);
    //template <typename T = BasicEngine>
    //static T* GetInstance(){ return dynamic_cast<T*>(instance_);};
protected:
    static BasicEngine* instance_;
    const FilesystemInterface& filesystem_;
    Configuration config_;
    Renderer* renderer_ = nullptr;
    Window* window_ = nullptr;
    JobSystem jobSystem_;
	bool isRunning_ = false;
    float dt_ = 0.0f;
    Action<> initAction_;
    Action<seconds> updateAction_;
    Action<> drawImGuiAction_;
    Action<> destroyAction_;

};

}
