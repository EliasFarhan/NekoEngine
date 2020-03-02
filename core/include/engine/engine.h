#pragma once

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
#include <mutex>
#include <string>
#include <engine/system.h>
#include <utilities/action_utility.h>
#include <graphics/color.h>
#include <utilities/time_utility.h>
#include <mathematics/vector.h>
#include <atomic>


namespace neko
{
class Renderer;
class Window;

/**
 * \brief store various Engine constant or global values
 */
struct Configuration
{
	std::string windowName = "NekoEngine 0.1";
    Vec2i windowSize = Vec2i(1024, 1024);
    Vec2i gameWindowSize{1280, 720};
    bool fullscreen = false;
    bool vSync = true;
    unsigned int framerateLimit = 0u;
#ifdef EMSCRIPTEN
    std::string dataRootPath = "./";
#else
    std::string dataRootPath = "../../";
#endif
};


/**
 * \brief basic engine class with no graphics manager implementation
 */
class BasicEngine : public SystemInterface
{
public:
    explicit BasicEngine(Configuration* config = nullptr);
	BasicEngine() = delete;
    ~BasicEngine();
    void Init() override;
    void Update(seconds dt) final;
    void Destroy() override;

    //Update functions
    virtual void ManageEvent() = 0;
    virtual void GenerateUiFrame();

    void EngineLoop();

    void SetWindowAndRenderer(Window* window, Renderer* renderer);

    Configuration config;

    void RegisterSystem(SystemInterface& system);
    void RegisterOnDrawUi(DrawImGuiInterface& drawUi);

    float GetDeltaTime() const { return dt_; };
	
    static BasicEngine* GetInstance(){return instance_;}

    //template <typename T = BasicEngine>
    //static T* GetInstance(){ return dynamic_cast<T*>(instance_);};
protected:
    static BasicEngine* instance_;
    Renderer* renderer_ = nullptr;
    Window* window_ = nullptr;
	bool isRunning_;
    std::atomic<float> dt_;
    Action<> initAction_;
    Action<seconds> updateAction_;
    Action<> drawImGuiAction_;
    Action<> destroyAction_;

};

}
