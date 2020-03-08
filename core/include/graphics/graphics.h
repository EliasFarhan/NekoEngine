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

#include <condition_variable>
#include <array>
#include <vector>
#include <thread>
#include <atomic>
#include <utilities/service_locator.h>

#include "engine/system.h"
#include "engine/log.h"

namespace neko
{
class Window;
const size_t MAX_COMMAND_NMB = 8'192;


/**
 * \brief abstraction of a graphic command send to the render thread
 */
class RenderCommandInterface
{
public:
    RenderCommandInterface() = default;
    virtual ~RenderCommandInterface() = default;

    virtual void Render() = 0;
};

/**
 * \brief Abstraction of a a graphic command that can be initialized, destroyed and update
 * Warning! Update can be run concurrently to Render!
 */
class RenderProgram : public RenderCommandInterface, public SystemInterface
{
};

class RendererInterface
{
public:
    virtual void Render(RenderCommandInterface* command) = 0;
};

class NullRenderer final : public RendererInterface
{
public:
    void Render([[maybe_unused]]RenderCommandInterface* command) override
    {};
};

class Renderer : public RendererInterface
{
public:
    enum RendererFlag : std::uint8_t
    {
        IS_RUNNING = 1u << 0u,
        IS_APP_WAITING = 1u << 1u,
        IS_RENDERING_UI = 1u << 2u,
    	IS_APP_SWAPPING = 1u << 3u
    };

    Renderer();

    virtual ~Renderer() = default;

    /**
     * \brief Send the RenderCommand to the queue for next frame
     * @param command
     */
    void Render(RenderCommandInterface* command) override;

    /**
     * \brief Called from Engine Loop to sync with the Render Loop
     */
    void Sync();

    virtual void Update();
    void RenderLoop();

    void Destroy();

    void SetFlag(RendererFlag flag);
    void SetWindow(Window* window);
    float GetDeltaTime() const { return dt_; }
protected:
    virtual void ClearScreen() = 0;

    virtual void RenderAll();

    virtual void BeforeRenderLoop();
    virtual void AfterRenderLoop();

    Window* window_ = nullptr;
    std::thread renderThread_;

    //Used to synchronize the EngineLoop with the RenderLoop
    std::mutex renderMutex_;
    std::atomic<std::uint8_t> flags_{IS_RENDERING_UI};
    std::condition_variable cv_;
    std::atomic<float> dt_{0.0f};

    std::vector<RenderCommandInterface*> currentCommandBuffer_ = {};
    std::vector<RenderCommandInterface*> nextCommandBuffer_ = {};
};

using RendererLocator = Locator<RendererInterface, NullRenderer>;

}
